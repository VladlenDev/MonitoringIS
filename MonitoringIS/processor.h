/*
processor.h
*/
#pragma once

#include <string>

using namespace std;

class processor {
public:
    string get_info() {
        string info = name;

        info.append("\nSOCKET: ");
        info.append(socket);
        info.append("\n");
        info.append(to_string(cores));
        info.append(" CORES, ");
        info.append(to_string(SMTs));
        info.append(" THREADS\nFREQUENCY: ");
        info.append(to_string(frequency));
        info.append(" MHz\nTECHPROCESS: ");
        info.append(to_string(techprocess));
        info.append(" nm\nTDP: ");
        info.append(to_string(TDP));
        info.append(" W");

        return info;
    }
    void set_name(string str) {
        name = str;
    }
    void set_socket(string str) {
        socket = str;
    }
    void set_frequency(int value) {
        frequency = value;
    }
    void set_techprocess(int value) {
        techprocess = value;
    }
    void set_cores(int value) {
        cores = value;
    }
    void set_SMTs(int value) {
        SMTs = value;
    }
    void set_TDP(int value) {
        TDP = value;
    }
    string get_name() {
        return name;
    }
    string get_socket() {
        return socket;
    }
    int get_frequency() {
        return frequency;
    }
    int get_techprocess() {
        return techprocess;
    }
    int get_cores() {
        return cores;
    }
    int get_SMTs() {
        return SMTs;
    }
    int get_TDP() {
        return TDP;
    }
private:
    string name;
    string socket;
    unsigned int frequency;
    unsigned int techprocess;
    unsigned int cores;
    unsigned int SMTs;
    unsigned int TDP;
};