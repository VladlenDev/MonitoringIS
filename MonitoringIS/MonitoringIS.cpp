/*
MonitoringIS.cpp
*/
#include "MonitoringIS.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    char inSym = ' ';
    const string connection_params("host=localhost port=5432 dbname=monitoringdb user=postgres password=1234");
    report rep;
    pc comp(rep.get_data());
    hwExpert expert(comp.get_name(), comp.get_indicators());

    cout << "MonitoringIS.exe: to exit nicely from the application enter \'q\'" << endl << endl;

    comp.show_config();
    comp.db_updade_or_add_config(connection_params);
    thread moniThr(monitoring_pc, ref(comp), ref(rep), ref(expert), connection_params, 1);
    moniThr.detach();
    while (inSym != 'q') {
        cin >> inSym;
    }
    return 0;
}