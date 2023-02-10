/*
pc.h
*/
#pragma once

#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include "processor.h"
#include "videoadapter.h"
#include "memory.h"
#include "disk.h"
#include "sensors.h"

using namespace std;

class pc {
public:
    bool db_does_pc_exist(const string connection_params) {
        bool is_found = false;
        string query;
        query = "SELECT * FROM pc WHERE name=\'" + name;
        query.append("\';");
        try {
            pqxx::connection conn(connection_params.c_str());
            pqxx::work worker(conn);
            pqxx::result res = worker.exec(query.c_str());
            if (!res.empty()) {
                is_found = true;
            }
            query.clear();
            res.clear();
            conn.close();
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
        }
        return is_found;
    }
    void db_init_id(const string connection_params) {
        bool pc_exist = db_does_pc_exist(connection_params);
        id = 0;
        string query;
        try {
            pqxx::connection conn(connection_params.c_str());
            pqxx::work worker(conn);
            pqxx::result res;
            if (pc_exist) {
                query = "SELECT * FROM pc WHERE name=\'";
                query.append(name);
                query.append("\';");
                res = worker.exec(query.c_str());
                id = atoi(res[0][0].c_str());
            }
            else {
                query = "SELECT id FROM pc;";
                res = worker.exec(query.c_str());
                id = res.size() + 1;
            }
            query.clear();
            res.clear();
            conn.close();
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }
    void db_add_config(const string connection_params) {
        int i;
        string query;

        query = "INSERT INTO pc (name, ip, mac, manufacturer, chipset, os_version, ram, drive_space) VALUES (\'";
        query.append(name);
        query.append("\', \'");
        query.append(ip);
        query.append("\', \'");
        query.append(mac);
        query.append("\', \'");
        query.append(manufacturer);
        query.append("\', \'");
        query.append(chipset);
        query.append("\', \'");
        query.append(os_version);
        query.append("\', ");
        query.append(to_string(total_ram));
        query.append(", ");
        query.append(to_string(total_drive_space));
        query.append(" );");

        try {
            pqxx::connection conn(connection_params.c_str());
            pqxx::work worker_pc(conn);
            worker_pc.exec(query.c_str());
            worker_pc.commit();
            query.clear();

            query.append("INSERT INTO cpu (pc_id, name, socket, frequency, techprocess, cores, smts, tdp) VALUES (");
            query.append(to_string(id));
            query.append(", \'");
            query.append(cpu.get_name());
            query.append("\', \'");
            query.append(cpu.get_socket());
            query.append("\', ");
            query.append(to_string(cpu.get_frequency()));
            query.append(", ");
            query.append(to_string(cpu.get_techprocess()));
            query.append(", ");
            query.append(to_string(cpu.get_cores()));
            query.append(", ");
            query.append(to_string(cpu.get_SMTs()));
            query.append(", ");
            query.append(to_string(cpu.get_TDP()));

            query.append(" ); INSERT INTO gpu (pc_id, name, bus, mem_type, mem_count, techprocess) VALUES (");
            query.append(to_string(id));
            query.append(", \'");
            query.append(gpu.get_name());
            query.append("\', \'");
            query.append(gpu.get_bus());
            query.append("\', \'");
            query.append(gpu.get_mem_type());
            query.append("\', ");
            query.append(to_string(gpu.get_mem_count()));
            query.append(", ");
            query.append(to_string(gpu.get_techprocess()));

            for (i = 0; i < ram.size(); i++) {
                query.append(" ); INSERT INTO ram (pc_id, name, type, timings, capacity, frequency, voltage) VALUES (");
                query.append(to_string(id));
                query.append(", \'");
                query.append(ram[i].get_name());
                query.append("\', \'");
                query.append(ram[i].get_type());
                query.append("\', \'");
                query.append(ram[i].get_timings());
                query.append("\', ");
                query.append(to_string(ram[i].get_capacity()));
                query.append(", ");
                query.append(to_string(ram[i].get_frequency()));
                query.append(", ");
                query.append(to_string(ram[i].get_voltage()));
                query[query.length() - 7] = '.';
            }

            for (i = 0; i < drive.size(); i++) {
                query.append(" ); INSERT INTO drive (pc_id, name, type, bus, capacity) VALUES (");
                query.append(to_string(id));
                query.append(", \'");
                query.append(drive[i].get_name());
                query.append("\', \'");
                query.append(drive[i].get_type());
                query.append("\', \'");
                query.append(drive[i].get_bus());
                query.append("\', ");
                query.append(to_string(drive[i].get_capacity()));
            }

            query += " );";

            pqxx::work worker_config(conn);
            worker_config.exec(query.c_str());
            worker_config.commit();
            query.clear();
            conn.close();
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            exit(1);
        }
    }
    void db_update_config(const string connection_params) {
        int i;
        string query;

        query = "UPDATE pc SET ip=\'";
        query.append(ip);
        query.append("\', mac=\'");
        query.append(mac);
        query.append("\', manufacturer=\'");
        query.append(manufacturer);
        query.append("\', chipset=\'");
        query.append(chipset);
        query.append("\', os_version=\'");
        query.append(os_version);
        query.append("\', ram=");
        query.append(to_string(total_ram));
        query.append(", drive_space=");
        query.append(to_string(total_drive_space));
        query.append(" WHERE id=");
        query.append(to_string(id));
        query.append("; ");

        query.append("UPDATE cpu SET name=\'");
        query.append(cpu.get_name());
        query.append("\', socket=\'");
        query.append(cpu.get_socket());
        query.append("\', frequency=");
        query.append(to_string(cpu.get_frequency()));
        query.append(", techprocess=");
        query.append(to_string(cpu.get_techprocess()));
        query.append(", cores=");
        query.append(to_string(cpu.get_cores()));
        query.append(", smts=");
        query.append(to_string(cpu.get_SMTs()));
        query.append(", tdp=");
        query.append(to_string(cpu.get_TDP()));
        query.append(" WHERE pc_id=");
        query.append(to_string(id));
        query.append("; ");

        query.append("UPDATE gpu SET name=\'");
        query.append(gpu.get_name());
        query.append("\', bus=\'");
        query.append(gpu.get_bus());
        query.append("\', mem_type=\'");
        query.append(gpu.get_mem_type());
        query.append("\', mem_count=");
        query.append(to_string(gpu.get_mem_count()));
        query.append(", techprocess=");
        query.append(to_string(gpu.get_techprocess()));
        query.append(" WHERE pc_id=");
        query.append(to_string(id));
        query.append("; ");

        query.append("DELETE FROM ram WHERE pc_id=");
        query.append(to_string(id));
        query.append("; ");
        for (i = 0; i < ram.size(); i++) {
            query.append("INSERT INTO ram (pc_id, name, type, timings, capacity, frequency, voltage) VALUES (");
            query.append(to_string(id));
            query.append(", \'");
            query.append(ram[i].get_name());
            query.append("\', \'");
            query.append(ram[i].get_type());
            query.append("\', \'");
            query.append(ram[i].get_timings());
            query.append("\', ");
            query.append(to_string(ram[i].get_capacity()));
            query.append(", ");
            query.append(to_string(ram[i].get_frequency()));
            query.append(", ");
            query.append(to_string(ram[i].get_voltage()));
            query[query.length() - 7] = '.';
            query.append(" ); ");
        }

        query.append("DELETE FROM drive WHERE pc_id=");
        query.append(to_string(id));
        query.append("; ");
        for (i = 0; i < drive.size(); i++) {
            query.append("INSERT INTO drive (pc_id, name, type, bus, capacity) VALUES (");
            query.append(to_string(id));
            query.append(", \'");
            query.append(drive[i].get_name());
            query.append("\', \'");
            query.append(drive[i].get_type());
            query.append("\', \'");
            query.append(drive[i].get_bus());
            query.append("\', ");
            query.append(to_string(drive[i].get_capacity()));
            query.append(" ); ");
        }

        try {
            pqxx::connection conn(connection_params.c_str());
            pqxx::work worker(conn);
            worker.exec(query.c_str());
            worker.commit();
            query.clear();
            conn.close();
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            exit(1);
        }
    }
    void db_updade_or_add_config(const string connection_params) {
        db_init_id(connection_params);
        if (db_does_pc_exist(connection_params)) {
            db_update_config(connection_params);
        }
        else {
            db_add_config(connection_params);
        }
    }
    void db_add_sensors_data(const string connection_params) {
        string query, buff;

        query = "INSERT INTO indicators (pc_id, session, date, time, smart, fans, "
            "drives_temp, motherboard_temp, cpu_temp, gpu_temp, cpu_tdp_percent, gpu_tdp_percent, "
            "cpu_load, ram_load, gpu_load, vram_load, drive_load) VALUES (";
        query.append(to_string(id));
        query.append(", \'");
        query.append(indicators.get_session());
        query.append("\', \'");
        query.append(indicators.get_date());
        query.append("\', \'");
        query.append(indicators.get_time());
        query.append("\', \'");
        query.append(indicators.get_smart());
        query.append("\', \'");
        query.append(indicators.get_fans());
        query.append("\', \'");
        buff = indicators.get_drives_temp();
        while ((int)buff.find('°') != -1) {
            buff[buff.find('°')] = '`';
        }
        query.append(buff);
        query.append("\', ");
        query.append(to_string(indicators.get_mboard_temp()));
        query.append(", ");
        query.append(to_string(indicators.get_cpu_temp()));
        query.append(", ");
        query.append(to_string(indicators.get_gpu_temp()));
        query.append(", ");
        query.append(to_string(indicators.get_cpu_tdp_percent()));
        query.append(", ");
        query.append(to_string(indicators.get_gpu_tdp_percent()));
        query.append(", ");
        query.append(to_string(indicators.get_cpu_load()));
        query.append(", ");
        query.append(to_string(indicators.get_ram_load()));
        query.append(", ");
        query.append(to_string(indicators.get_gpu_load()));
        query.append(", ");
        query.append(to_string(indicators.get_vram_load()));
        query.append(", ");
        query.append(to_string(indicators.get_drive_load()));
        query.append(" );");

        try {
            pqxx::connection conn(connection_params.c_str());
            pqxx::work worker_sens(conn);
            worker_sens.exec(query.c_str());
            worker_sens.commit();
            query.clear();
            conn.close();
        }
        catch (const exception& e) {
            cerr << e.what() << endl;
            exit(1);
        }
    }
    void update(string report) {
        int i = 0, j = 0, pos = 0;
        string currdata;
        int currint = 0;
        string buff, keyword;

        //sensors data
        //date
        keyword = "Дата";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i + 1] != '\n'; i++) {}
        currdata.append(report, pos, i);
        indicators.set_date(currdata);
        currdata.clear();
        //time
        keyword = "Время";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i + 1] != '\n'; i++) {}
        currdata.append(report, pos, i);
        indicators.set_time(currdata);
        currdata.clear();
        //smart
        keyword = "SMART-статус жёстких дисков";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i + 1] != '\n'; i++) {}
        currdata.append(report, pos, i);
        indicators.set_smart(currdata);
        currdata.clear();
        //fans
        keyword = "Вентиляторы:";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '\n' || report[pos + i + 1] != '\r'; i++) {}
        buff.append(report, pos, i - 1);
        keyword = "RPM";
        while ((int)(pos = buff.find(keyword)) != -1) {
            for(pos; buff[pos - 2] != ' ' || buff[pos - 1] != ' '; pos--) {}
            for (i = 0; buff[pos + i + 1] != '\n' && buff[pos + i + 2] != '('; i++) {}
            currdata.append(buff, pos, i);
            currdata.append(", ");
            pos = buff.find(keyword) + keyword.size();
            for (i = 0; i + pos < buff.size(); i++) {
                buff[i] = buff[i + pos];
            }
        buff.resize(i);
        }
        currdata.erase(currdata.length() - 2);
        indicators.set_fans(currdata);
        currdata.clear();
        buff.clear();
        //drives_temp
        keyword = "[ Датчики ]";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '-' || report[pos + i + 1] != '['; i++) {}
        buff.append(report, pos, i - 1);
        j = 0;
        while (j < drive.size()) {
            keyword = drive[j].get_name();
            for (pos = buff.find(keyword) + keyword.size(); buff[pos] == ' '; pos++) {}
            for (i = 0; buff[pos + i + 1] != '\n'; i++) {}
            currdata.append(buff, pos, i);
            currdata.append(", ");
            pos = buff.find(keyword);
            buff.erase(pos);
            j++;
        }
        currdata.erase(currdata.length() - 2);
        indicators.set_drives_temp(currdata);
        currdata.clear();
        buff.clear();
        //other temperatures
        keyword = "Температуры:";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '\n' || report[pos + i + 1] != '\r'; i++) {}
        buff.append(report, pos, i - 1);
        //mboard_temp
        keyword = "Системная плата";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currdata.append(buff, pos, i);
        currint = stoi(currdata);
        indicators.set_mboard_temp(currint);
        currdata.clear();
        //cpu_temp
        keyword = "CPU IA Cores";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currdata.append(buff, pos, i);
        currint = stoi(currdata);
        indicators.set_cpu_temp(currint);
        currdata.clear();
        //gpu_temp
        keyword = "Диод ГП";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currdata.append(buff, pos, i);
        currint = stoi(currdata);
        indicators.set_gpu_temp(currint);
        currdata.clear();
        buff.clear();
        //power values
        keyword = "Значения мощности:";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '\n' || report[pos + i + 1] != '\r'; i++) {}
        buff.append(report, pos, i - 1);
        //cpu_tdp_percent
        keyword = "CPU Package";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currdata.append(buff, pos, i);
        currdata[currdata.find('.')] = ',';
        currint = (int) (100.0 * (stof(currdata) / (float) cpu.get_TDP()));
        indicators.set_cpu_tdp_percent(currint);
        currdata.clear();
        //gpu_tdp_percent
        keyword = "GPU TDP%";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != '%'; i++) {}
        currdata.append(buff, pos, i);
        currint = stoi(currdata);
        indicators.set_gpu_tdp_percent(currint);
        currdata.clear();
        buff.clear();
        //cpu_load
        buff = report;
        j = 0;
        currint = 0;
        keyword = "/ SMT ";
        while (j < cpu.get_SMTs()) {
            for (pos = buff.find(keyword) + keyword.size() + 1; buff[pos] == ' '; pos++) {}
            for (i = 0; buff[pos + i + 1] != '\n'; i++) {}
            currdata.append(buff, pos, i);
            currint += stoi(currdata);
            currdata.clear();
            pos = buff.find(keyword) + keyword.size();
            for (i = 0; i + pos < buff.size(); i++) {
                buff[i] = buff[i + pos];
            }
            buff.resize(i);
            j++;
        }
        currint = currint / cpu.get_SMTs();
        indicators.set_cpu_load(currint);
        buff.clear();
        //ram_load
        keyword = "Физическая память:";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '\n' || report[pos + i + 1] != '\r'; i++) {}
        buff.append(report, pos, i - 1);
        keyword = "Загрузка";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currdata.append(buff, pos, i);
        currint = stoi(currdata);
        indicators.set_ram_load(currint);
        currdata.clear();
        buff.clear();
        //gpu_load
        keyword = "[ Графический процессор ]";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '-' || report[pos + i + 1] != '['; i++) {}
        buff.append(report, pos, i - 1);
        keyword = "Графический процессор";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != '%'; i++) {}
        currdata.append(buff, pos, i);
        currint = stoi(currdata);
        indicators.set_gpu_load(currint);
        currdata.clear();
        //vram_load
        keyword = "Контроллер памяти";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != '%'; i++) {}
        currdata.append(buff, pos, i);
        currint = stoi(currdata);
        indicators.set_vram_load(currint);
        currdata.clear();
        buff.clear();
        //drive_load
        keyword = "Общий размер";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos-1] != '('; pos++) {}
        for (i = 0; report[pos + i + 1] != ' '; i++) {}
        currdata.append(report, pos, i);
        currint = 100 - (100 * stoi(currdata) / total_drive_space);
        indicators.set_drive_load(currint);
        currdata.clear();
    }
    void show_data() {
        cout << endl << "----------------[ INDICATORS ]----------------" << endl << endl;
        cout << indicators.get_info() << endl;
        cout << endl << "----------------------------------------------" << endl;
    }
    void show_config() {
        int i;
        cout << "--------------[ PC INFORMATION ]--------------" << endl << endl;
        cout << "NAME: " << name << endl;
        cout << "IP: " << ip << endl;
        cout << "MAC: " << mac << endl;
        cout << "MANUFACTURER: " << manufacturer << endl;
        cout << "CHIPSET: " << chipset << endl;
        cout << "OS VERSION: " << os_version << endl;
        cout << "TOTAL RAM: " << total_ram << " MB" << endl;
        cout << "TOTAL DRIVE SPACE: " << total_drive_space << " GB" << endl;
        cout << endl << "-------------------[ CPU ]--------------------" << endl << endl;
        cout << cpu.get_info() << endl;
        cout << endl << "-------------------[ GPU ]--------------------" << endl << endl;
        cout << gpu.get_info() << endl;
        cout << endl << "-------------------[ RAM ]--------------------" << endl << endl;
        for (i = 0; i < ram.size(); i++) {
            cout << "RAM " << i + 1 << ": " << ram[i].get_info() << endl << endl;
        }
        cout << "-----------------[ STORAGE ]------------------" << endl << endl;
        for (i = 0; i < drive.size(); i++) {
            cout << "DRIVE " << i + 1 << ": " << drive[i].get_info() << endl << endl;
        }
        cout << "----------------------------------------------" << endl;
    }
    string get_name() {
        return name;
    }
    sensors get_indicators() {
        return indicators;
    }
    pc(string report) {
        int i = 0, j = 0, pos = 0;
        string keyword;
        string currinf;
        int currnum = 0;
        float currfloat = 0;
        string buff, subbuf;

        //pc info
        keyword = "Имя компьютера";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != '\n'; i++) {}
        currinf.append(report, pos, i - 1);
        name = currinf;
        currinf.clear();
        //pc ip
        keyword = "Первичный адрес IP";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != '\n'; i++) {}
        currinf.append(report, pos, i - 1);
        ip = currinf;
        currinf.clear();
        //pc mac
        keyword = "Первичный адрес MAC";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != ' '; i++) {}
        currinf.append(report, pos, i);
        mac = currinf;
        currinf.clear();
        //pc manufacturer
        keyword = "производитель системы";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != '\n'; i++) {}
        currinf.append(report, pos, i - 1);
        manufacturer = currinf;
        currinf.clear();
        //pc chipset
        keyword = "Чипсет системной платы";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != '\n'; i++) {}
        currinf.append(report, pos, i - 1);
        chipset = currinf;
        currinf.clear();
        //pc os_version
        keyword = "Операционная система";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != '\n'; i++) {}
        currinf.append(report, pos, i - 1);
        os_version = currinf;
        currinf.clear();
        //pc total ram
        keyword = "Системная память";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != ' '; i++) {}
        currinf.append(report, pos, i);
        currnum = stoi(currinf);
        total_ram = currnum;
        currinf.clear();
        //pc total drive space
        keyword = "Общий размер";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i] != ' '; i++) {}
        currinf.append(report, pos, i);
        currnum = stoi(currinf);
        total_drive_space = currnum;
        currinf.clear();

        //cpu info
        keyword = "[ ЦП ]";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '-' || report[pos + i + 1] != '['; i++) {}
        buff.resize(i);
        buff.append(report, pos, i);
        //cpu name
        keyword = "Тип ЦП";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ','; i++) {}
        currinf.append(buff, pos, i);
        cpu.set_name(currinf);
        currinf.clear();
        //cpu socket
        keyword = "Тип корпуса";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != '\n'; i++) {}
        currinf.append(buff, pos, i - 1);
        cpu.set_socket(currinf);
        currinf.clear();
        //cpu frequency
        keyword = "Исходная частота";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currinf.append(buff, pos, i);
        currnum = stoi(currinf);
        cpu.set_frequency(currnum);
        currinf.clear();
        //cpu techprocess
        keyword = "Технологический процесс";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos - 1] != ' ' || buff[pos - 2] != ','; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currinf.append(buff, pos, i);
        currnum = stoi(currinf);
        cpu.set_techprocess(currnum);
        currinf.clear();
        //cpu cores
        keyword = "Ядро ";
        pos = buff.rfind(keyword) + keyword.size();
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currinf.append(buff, pos, i);
        currnum = stoi(currinf);
        cpu.set_cores(currnum);
        currinf.clear();
        //cpu SMTs
        keyword = "CPU #";
        pos = buff.rfind(keyword) + keyword.size();
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currinf.append(buff, pos, i);
        currnum = stoi(currinf);
        cpu.set_SMTs(currnum);
        currinf.clear();
        //cpu TDP
        keyword = "Типичная мощность";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currinf.append(buff, pos, i);
        currnum = stoi(currinf);
        cpu.set_TDP(currnum);
        currinf.clear();
        buff.clear();

        //gpu info
        keyword = "[ Графический процессор ]";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '-' || report[pos + i + 1] != '['; i++) {}
        buff.resize(i);
        buff.append(report, pos, i);
        //gpu name
        keyword = "Видеоадаптер";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != '\n'; i++) {}
        currinf.append(buff, pos, i - 1);
        gpu.set_name(currinf);
        currinf.clear();
        //gpu bus
        keyword = "Тип шины";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != '@'; i++) {}
        currinf.append(buff, pos, i);
        gpu.set_bus(currinf);
        currinf.clear();
        //video memory type
        keyword = "Тип шины";
        pos = buff.rfind(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != '('; i++) {}
        currinf.append(buff, pos, i);
        gpu.set_mem_type(currinf);
        currinf.clear();
        //video memory capacity
        keyword = "Объем видеоОЗУ";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currinf.append(buff, pos, i);
        currnum = stoi(currinf);
        gpu.set_mem_count(currnum);
        currinf.clear();
        //gpu techprocess
        keyword = "Технологический процесс";
        pos = buff.find(keyword) + keyword.size();
        for (pos; buff[pos] == ' '; pos++) {}
        for (i = 0; buff[pos + i] != ' '; i++) {}
        currinf.append(buff, pos, i);
        currnum = stoi(currinf);
        gpu.set_techprocess(currnum);
        currinf.clear();
        buff.clear();

        //ram info
        buff = report;
        keyword = "[ SPD ]";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '-' || report[pos + i + 1] != '['; i++) {}
        buff.resize(i);
        buff.append(report, pos, i);
        subbuf = buff;
        buff.clear();
        j = 0;
        //first of ram search
        keyword = "[ DIMM";
        while ((int)(pos = subbuf.find(keyword)) != -1) {
            //ram name
            pos += (keyword.size() + 3);
            for (i = 0; subbuf[pos + i] != ' ' || subbuf[pos + i + 1] != ']'; i++) {}
            currinf.append(subbuf, pos, i);
            ram.push_back(currinf);
            currinf.clear();
            //ram type           
            keyword = "Тип памяти";
            pos = subbuf.find(keyword) + keyword.size();
            for (pos; subbuf[pos] == ' '; pos++) {}
            for (i = 0; subbuf[pos + i] != '\n'; i++) {}
            currinf.append(subbuf, pos, i - 1);
            ram[j].set_type(currinf);
            currinf.clear();
            //ram capacity
            keyword = "Размер модуля";
            pos = subbuf.find(keyword) + keyword.size();
            for (pos; subbuf[pos] == ' '; pos++) {}
            for (i = 0; subbuf[pos + i] != ' '; i++) {}
            currinf.append(subbuf, pos, i);
            currnum = stoi(currinf);
            ram[j].set_capacity(currnum);
            currinf.clear();
            //ram frequency
            keyword = "Скорость памяти  ";
            pos = subbuf.find(keyword) + keyword.size();
            for (pos; subbuf[pos - 1] != '-'; pos++) {}
            for (i = 0; subbuf[pos + i] != ' '; i++) {}
            currinf.append(subbuf, pos, i);
            currnum = stoi(currinf);
            ram[j].set_frequency(currnum);
            currinf.clear();
            //ram voltage
            keyword = "Напряжение модуля  ";
            pos = subbuf.find(keyword) + keyword.size();
            for (pos; subbuf[pos] == ' '; pos++) {}
            for (i = 0; subbuf[pos + i] != ' '; i++) {}
            currinf.append(subbuf, pos, i);
            for (i = 0; i < currinf.length(); i++) {
                if (currinf[i] == '.')
                    currinf[i] = ',';
            }
            currfloat = stof(currinf);
            ram[j].set_voltage(currfloat);
            currinf.clear();
            //ram timings
            keyword = "@ ";
            keyword.append(to_string(ram[j].get_frequency()/2));
            keyword.append(" МГц ");
            pos = subbuf.find(keyword) + keyword.size();
            for (pos; subbuf[pos] == ' '; pos++) {}
            for (i = 0; subbuf[pos + i] != ' '; i++) {}
            currinf.append(subbuf, pos, i);
            ram[j].set_timings(currinf);
            currinf.clear();
            //delete from a buffer current ram info for next ram info search
            keyword = "Производитель модуля памяти:";
            pos = subbuf.find(keyword) + keyword.size();
            for (i = 0; i + pos < subbuf.size(); i++) {
                subbuf[i] = subbuf[i + pos];
            }
            subbuf.resize(i);
            keyword = "[ DIMM";
            j++;
        }
        subbuf.clear();

        //drives info
        buff.resize(report.size());
        buff = report;
        j = 0;
        keyword = "Дисковый накопитель";
        while ((int) (pos = buff.find(keyword)) != - 1) {
            //drive name
            for (pos += keyword.size(); buff[pos] == ' '; pos++) {}
            for (i = 0; buff[pos + i + 1] != '('; i++) {}
            currinf.append(buff, pos, i);
            drive.push_back(currinf);
            currinf.clear();
            //drive type
            pos += i;
            for (i = 0; buff[pos + i] != ')'; i++) {}
            currinf.append(buff, pos, i);
            string type("SSD");
            if (currinf.find("RPM") != -1) {
                type = "HDD";
            }
            drive[j].set_type(type);
            //drive bus
            if ((int)(currnum = currinf.find("SATA")) != -1) {
                i = currnum;
                currinf.erase(0, (size_t)i);
            }
            else if ((int)(currnum = currinf.find("PCI-E")) != -1) {
                i = currnum;
                currinf.erase(0, (size_t)i);
            }
            else if ((int)(currnum = currinf.find("IDE")) != -1) {
                i = currnum;
                currinf.erase(0, (size_t)i);
            }
            else if ((int)(currnum = currinf.find("M.2")) != -1) {
                i = currnum;
                currinf.erase(0, (size_t)i);
            }
            else {
                currinf = "UNIDENTIFIED";
            }
            drive[j].set_bus(currinf);
            currinf.clear();
            //delete from a buffer current drive info for next drive info search
            for (i = 0; i + pos < buff.size(); i++) {
                buff[i] = buff[i + pos];
            }
            buff.resize(i);
            j++;
        }
        buff.clear();
        //drives capacity
        buff = report;
        keyword = "[ Физические диски ]";
        pos = report.find(keyword) + keyword.size();
        for (i = 0; report[pos + i] != '-' || report[pos + i + 1] != '['; i++) {}
        buff.clear();
        buff.append(report, pos, i);
        for (j = 0; j < drive.size(); j++) {
            currinf = drive[j].get_name();
            pos = buff.find(currinf) + currinf.length();
            currinf.clear();
            for(pos; buff[pos - 1] != '['; pos++) {}
            for(i = 0; buff[pos + i] != ']'; i++) {}
            currinf.append(buff, pos, i);
            currnum = stoi(currinf);
            drive[j].set_capacity(currnum);
            currinf.clear();
        }
        buff.clear();

        //sensors info
        //session ([date]-[time])
        keyword = "Дата";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i + 1] != '\n'; i++) {}
        currinf.append(report, pos, i);
        currinf.append("-");
        keyword = "Время";
        pos = report.find(keyword) + keyword.size();
        for (pos; report[pos] == ' '; pos++) {}
        for (i = 0; report[pos + i + 1] != '\n'; i++) {}
        currinf.append(report, pos, i);
        indicators.set_session(currinf);
        currinf.clear();
        //other data
        update(report);
    }
private:
    int id;
    string name;
    string ip;
    string mac;
    string manufacturer;
    string chipset;
    string os_version;
    int total_ram;
    int total_drive_space;

    processor cpu;
    videoadapter gpu;
    vector<memory> ram;
    vector<disk> drive;
    sensors indicators;
};
