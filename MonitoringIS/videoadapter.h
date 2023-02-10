/*
videoadapter.h
*/
#pragma once

#include <string>

using namespace std;

class videoadapter {
public:
    string get_info() {
        string info = name;

        info.append("\nBUS: ");
        info.append(bus);
        info.append("\nVIDEO MEMORY: ");
        info.append(to_string(mem_count));
        info.append(" GB ");
        info.append(mem_type);
        info.append("\nTECHPROCESS: ");
        info.append(to_string(techprocess));
        info.append(" nm");

        return info;
    }
    void set_name(string str) {
        name = str;
    }
    void set_bus(string str) {
        bus = str;
    }
    void set_mem_type(string str) {
        mem_type = str;
    }
    void set_mem_count(int value) {
        mem_count = value;
    }
    void set_techprocess(int value) {
        techprocess = value;
    }
    string get_name() {
        return name;
    }
    string get_bus() {
        return bus;
    }
    string get_mem_type() {
        return mem_type;
    }
    int get_mem_count() {
        return mem_count;
    }
    int get_techprocess() {
        return techprocess;
    }
private:
    string name;
    string bus;
    string mem_type;
    unsigned int mem_count;
    unsigned int techprocess;
};
