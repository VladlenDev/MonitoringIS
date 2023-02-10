/*
sensors.h
*/
#pragma once

#include <string>

using namespace std;

class sensors {
public:
	string get_info() {
		string info;

		info.append("SESSION: ");
		info.append(session);
		info.append("\nDATE / TIME: ");
		info.append(date);
		info.append(" / ");
		info.append(time);
		info.append("\nS.M.A.R.T STATUS: ");
		info.append(smart);
		info.append("\nFANS SPEED: ");
		info.append(fans);
		info.append("\n\nTEMPERATURES:\nDRIVES: ");
		info.append(drives_temp);
		info.append(", MOTHERBOARD: ");
		info.append(to_string(mboard_temp));
		info.append(" °C, CPU: ");
		info.append(to_string(cpu_temp));
		info.append(" °C, GPU: ");
		info.append(to_string(gpu_temp));
		info.append(" °C\n\nLOAD:\nCPU: ");
		info.append(to_string(cpu_load));
		info.append(" %, RAM: ");
		info.append(to_string(ram_load));
		info.append(" %, GPU: ");
		info.append(to_string(gpu_load));
		info.append(" %, VIDEO RAM: ");
		info.append(to_string(vram_load));
		info.append(" %, DRIVES: ");
		info.append(to_string(drive_load));
		info.append(" %");

		return info;

	}
	void set_session(string str) {
		session = str;
	}
	void set_date(string str) {
		date = str;
	}
	void set_time(string str) {
		time = str;
	}
	void set_smart(string str) {
		smart = str;
	}
	void set_fans(string str) {
		fans = str;
	}
	void set_drives_temp(string str) {
		drives_temp = str;
	}
	void set_mboard_temp(int value) {
		mboard_temp = value;
	}
	void set_cpu_temp(int value) {
		cpu_temp = value;
	}
	void set_gpu_temp(int value) {
		gpu_temp = value;
	}
	void set_cpu_tdp_percent(int value) {
		cpu_tdp_percent = value;
	}
	void set_gpu_tdp_percent(int value) {
		gpu_tdp_percent = value;
	}
	void set_cpu_load(int value) {
		cpu_load = value;
	}
	void set_ram_load(int value) {
		ram_load = value;
	}
	void set_gpu_load(int value) {
		gpu_load = value;
	}
	void set_vram_load(int value) {
		vram_load = value;
	}
	void set_drive_load(int value) {
		drive_load = value;
	}
	string get_session() {
		return session;
	}
	string get_date() {
		return date;
	}
	string get_time() {
		return time;
	}
	string get_smart() {
		return smart;
	}
	string get_fans() {
		return fans;
	}
	string get_drives_temp() {
		return drives_temp;
	}
	int get_mboard_temp() {
		return mboard_temp;
	}
	int get_cpu_temp() {
		return cpu_temp;
	}
	int get_gpu_temp() {
		return gpu_temp;
	}
	int get_cpu_tdp_percent() {
		return cpu_tdp_percent;
	}
	int get_gpu_tdp_percent() {
		return gpu_tdp_percent;
	}
	int get_cpu_load() {
		return cpu_load;
	}
	int get_ram_load() {
		return ram_load;
	}
	int get_gpu_load() {
		return gpu_load;
	}
	int get_vram_load() {
		return vram_load;
	}
	int get_drive_load() {
		return drive_load;
	}
private:
	string session;
	string date;
	string time;
	string smart;
	string fans;

	string drives_temp;
	int mboard_temp;
	int cpu_temp;
	int gpu_temp;

	int cpu_tdp_percent;
	int gpu_tdp_percent;

	int cpu_load;
	int ram_load;
	int gpu_load;
	int vram_load;
	int drive_load;
};