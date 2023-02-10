/*
memory.h
*/
#pragma once

#include <string>

using namespace std;

class memory {
public:
    memory(string str) {
        name = str;
    }
    string get_info() {
        string info = name;

        info.append("\n");
        info.append(to_string(capacity));
        info.append(" GB ");
        info.append(to_string(frequency));
        info.append(" MHz ");
        info.append(type);
        info.append("\nTIMINGS: ");
        info.append(timings);
        info.append("\nVOLTAGE: ");
        info.append(to_string(voltage));
        info.erase(info.length() - 4);

        return info;
    }
    void set_type(string str) {
        type = str;
    }
    void set_timings(string str) {
        timings = str;
    }
    void set_capacity(int value) {
         capacity = value;
    }
    void set_frequency(int value) {
         frequency = value;
     }
    void set_voltage(float value) {
         voltage = value;
     }
    string get_name() {
        return name;
    }
    string get_type() {
        return type;
    }
    string get_timings() {
        return timings;
    }
    int get_capacity() {
        return capacity;
    }
    int get_frequency() {
        return frequency;
    }
    float get_voltage() {
        return voltage;
    }
private:
    string name;
    string type;
    string timings;
    int capacity;
    int frequency;
    float voltage;
};