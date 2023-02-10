/*
MonitoringIS.h
*/
#pragma once

#include <thread>
#include <chrono>
#include <mutex>
#include "report.h"
#include "pc.h"
#include "hwExpert.h"

using namespace std;

mutex g_lock;

void monitoring_pc(pc& comp, report& rep, hwExpert& expert, const string connection_params, const unsigned int delay_min) {
	while (1) {
		g_lock.lock();
		comp.show_data();
		comp.db_add_sensors_data(connection_params);
		expert.db_update_hw_status(connection_params, comp.get_indicators());
		expert.show();
		rep.update();
		comp.update(rep.get_data());
		g_lock.unlock();
		this_thread::sleep_for(chrono::minutes(delay_min));
	}
}