/*
disk.h
*/
#pragma once

#include <string>

using namespace std;

class disk {
public:
    disk(string str) {
        name = str;
    }
    string get_info() {
        string info = name;

        info.append("\nCAPACITY: ");
        info.append(to_string(capacity));
        info.append(" GB\nTYPE: ");
        info.append(type);
        info.append(" ");
        info.append(bus);

        return info;
    }
    void set_type(string str) {
        type = str;
    }
    void set_bus(string str) {
        bus = str;
    }
    void set_capacity(int value) {
        capacity = value;
    }
    string get_name() {
        return name;
    }
    string get_type() {
        return type;
    }
    string get_bus() {
        return bus;
    }
    int get_capacity() {
        return capacity;
    }
private:
    string name;
    string type;
    string bus;
    int capacity;
};
