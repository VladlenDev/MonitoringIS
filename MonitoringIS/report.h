/*
report.h
*/
#pragma once

#include <fstream>
#include <string>

using namespace std;

class report {
public:
    void update() {
        *this = report();
    }
	string get_data() {
		return data;
	}
	report() {
        const char* reportPath = "C:/Users/Vestmud/source/repos/MonitoringIS/MonitoringIS/Report.txt";
        path = reportPath;
        data = "DATA_IS_CLEAR";

        string buff = path;
        int i = 0;
        for (i = 0; buff[i] != '.'; i++) {
            if (buff[i] == '/') {
                buff[i] = '\\';
            }
        }
        buff.resize(i);

        const string aidaPath = "C:\\aida64\\aida64.exe";
        const string aidaMode = " /SILENT";
        const string reportType = " /R ";
        const string rPath = buff;
        const string reportFormat = " /TEXT";
        const string reportMode = " /CUSTOM ";
        const string rpfPath = "C:\\Users\\Vestmud\\source\\repos\\MonitoringIS\\MonitoringIS\\report_preferences";

        string command = "call ";
        command.append(aidaPath);
        command.append(aidaMode);
        command.append(reportType);
        command.append(rPath);
        command.append(reportFormat);
        command.append(reportMode);
        command.append(rpfPath);

        system(command.c_str());

        ifstream reportf(reportPath, ios::in | ios::binary | ios::ate);

        if (!reportf.is_open()) {
            data = "READ_REPORT_ERROR";
        }
        else {
            const ifstream::pos_type file_size = reportf.tellg();
            vector<char> reportData(file_size);
            reportf.seekg(0, ios::beg);
            reportf.read(&reportData[0], file_size);

            data.resize(file_size);

            for (i = 0; i < file_size; i++) {
                data[i] = reportData[i];
            }
        }

        reportf.close();
	}
private:
    string path;
	string data;
};