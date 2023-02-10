/*
hwExpert.h
*/
#pragma once

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include "sensors.h"

constexpr auto NOT_LOADED = 0;
constexpr auto LIGHTLY_LOADED = 1;
constexpr auto HEAVILY_LOADED = 2;
constexpr auto FULLY_LOADED = 3;

class hwExpert {
public:
	void db_update_hw_status(const string connection_params, sensors indicators) {
		string ass = update_assessment(indicators);

		string query = "UPDATE pc SET hw_status=\'" + ass;
		query += "\' WHERE name=\'";
		query += pc_name;
		query += "\';";

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
	string update_assessment(sensors indicators) {
		fuzzy_init(indicators);
		return assessment();
	}
	string assessment() {
		string ass = "hardware is ";
		switch (fuzzy_pc) {
		case(NOT_LOADED):
			ass += "not loaded ";
			break;
		case(LIGHTLY_LOADED):
			ass += "lightly loaded";
			break;
		case(HEAVILY_LOADED):
			ass += "heavily loaded";
			break;
		case(FULLY_LOADED):
			ass += "fully loaded";
			break;
		}
		return ass;
	}
	void show() {
		string info = "\n-[ HARDWARE EXPERT'S QUALITATIVE ASSESSMENT ]-\n";

		info += "\nPC [ ";
		info += pc_name;
		info += " ] IS ";
		switch (fuzzy_pc) {
		case(NOT_LOADED):
			info += "NOT LOADED";
			break;
		case(LIGHTLY_LOADED):
			info += "LIGHTLY LOADED";
			break;
		case(HEAVILY_LOADED):
			info += "HEAVILY LOADED";
			break;
		case(FULLY_LOADED):
			info += "FULLY LOADED";
			break;
		}

		info += "\nCPU IS ";
		switch (fuzzy_cpu) {
		case(NOT_LOADED):
			info += "NOT LOADED";
			break;
		case(LIGHTLY_LOADED):
			info += "LIGHTLY LOADED";
			break;
		case(HEAVILY_LOADED):
			info += "HEAVILY LOADED";
			break;
		case(FULLY_LOADED):
			info += "FULLY LOADED";
			break;
		}

		info += "\nGPU IS ";
		switch (fuzzy_gpu) {
		case(NOT_LOADED):
			info += "NOT LOADED";
			break;
		case(LIGHTLY_LOADED):
			info += "LIGHTLY LOADED";
			break;
		case(HEAVILY_LOADED):
			info += "HEAVILY LOADED";
			break;
		case(FULLY_LOADED):
			info += "FULLY LOADED";
			break;
		}

		info += "\nRAM IS ";
		switch (fuzzy_ram) {
		case(NOT_LOADED):
			info += "NOT LOADED";
			break;
		case(LIGHTLY_LOADED):
			info += "LIGHTLY LOADED";
			break;
		case(HEAVILY_LOADED):
			info += "HEAVILY LOADED";
			break;
		case(FULLY_LOADED):
			info += "FULLY LOADED";
			break;
		}

		info += "\nDRIVES ARE ";
		switch (fuzzy_drives) {
		case(NOT_LOADED):
			info += "NOT LOADED";
			break;
		case(LIGHTLY_LOADED):
			info += "LIGHTLY LOADED";
			break;
		case(HEAVILY_LOADED):
			info += "HEAVILY LOADED";
			break;
		case(FULLY_LOADED):
			info += "FULLY LOADED";
			break;
		}
		info += "\n\n----------------------------------------------\n";
		cout << info << endl;
	}
	void fuzzy_init(sensors indicators) {
		int cpu_load = indicators.get_cpu_load();
		int gpu_load = indicators.get_gpu_load();
		int ram_load = indicators.get_ram_load();
		int drives_load = indicators.get_drive_load();

		if (cpu_load <= 10) {
			fuzzy_cpu = NOT_LOADED;
		}
		else if (cpu_load <= 50) {
			fuzzy_cpu = LIGHTLY_LOADED;
		}
		else if (cpu_load <= 90) {
			fuzzy_cpu = HEAVILY_LOADED;
		}
		else {
			fuzzy_cpu = FULLY_LOADED;
		}

		if (gpu_load <= 10) {
			fuzzy_gpu = NOT_LOADED;
		}
		else if (gpu_load <= 50) {
			fuzzy_gpu = LIGHTLY_LOADED;
		}
		else if (gpu_load <= 90) {
			fuzzy_gpu = HEAVILY_LOADED;
		}
		else {
			fuzzy_gpu = FULLY_LOADED;
		}

		if (ram_load <= 10) {
			fuzzy_ram = NOT_LOADED;
		}
		else if (ram_load <= 50) {
			fuzzy_ram = LIGHTLY_LOADED;
		}
		else if (ram_load <= 90) {
			fuzzy_ram = HEAVILY_LOADED;
		}
		else {
			fuzzy_ram = FULLY_LOADED;
		}

		if (drives_load <= 10) {
			fuzzy_drives = NOT_LOADED;
		}
		else if (drives_load <= 50) {
			fuzzy_drives = LIGHTLY_LOADED;
		}
		else if (drives_load <= 90) {
			fuzzy_drives = HEAVILY_LOADED;
		}
		else {
			fuzzy_drives = FULLY_LOADED;
		}

		if (fuzzy_cpu == FULLY_LOADED || fuzzy_gpu == FULLY_LOADED || fuzzy_ram == FULLY_LOADED || fuzzy_drives == FULLY_LOADED) {
			fuzzy_pc = FULLY_LOADED;
		}
		else if (fuzzy_cpu == HEAVILY_LOADED || fuzzy_gpu == HEAVILY_LOADED || fuzzy_ram == FULLY_LOADED || fuzzy_drives == FULLY_LOADED) {
			fuzzy_pc = HEAVILY_LOADED;
		}
		else if (fuzzy_cpu == LIGHTLY_LOADED || fuzzy_gpu == LIGHTLY_LOADED || fuzzy_ram == HEAVILY_LOADED || fuzzy_drives == HEAVILY_LOADED) {
			fuzzy_pc = LIGHTLY_LOADED;
		}
		else if (fuzzy_ram == LIGHTLY_LOADED && fuzzy_drives == LIGHTLY_LOADED) {
			fuzzy_pc = LIGHTLY_LOADED;
		}
		else {
			fuzzy_pc = NOT_LOADED;
		}
	}
	void set_pc_name(string str) {
		pc_name = str;
	}
	hwExpert(string name, sensors indicators) {
		set_pc_name(name);
		fuzzy_cpu = NOT_LOADED;
		fuzzy_gpu = NOT_LOADED;
		fuzzy_ram = NOT_LOADED;
		fuzzy_drives = NOT_LOADED;
		fuzzy_pc = NOT_LOADED;
		fuzzy_init(indicators);
	}
	hwExpert() {
		pc_name = "!!!UNDEFINED PC!!!";
		fuzzy_cpu = NOT_LOADED;
		fuzzy_gpu = NOT_LOADED;
		fuzzy_ram = NOT_LOADED;
		fuzzy_drives = NOT_LOADED;
		fuzzy_pc = NOT_LOADED;
	}
private:
	string pc_name;
	int fuzzy_pc;
	int fuzzy_cpu;
	int fuzzy_gpu;
	int fuzzy_ram;
	int fuzzy_drives;
};