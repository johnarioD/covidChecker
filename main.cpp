#include <iostream>
#include <time.h>
#include "covidTrace.hpp"

using namespace std;

int macaddress_num;
macaddress_t* macaddress_list;
vector<macaddress_t> contacts;
map<macaddress_t,int> close_contacts;

double cpu_util = 0;

void ctrlCHandler(int signum);

int main(int argsc, char** argsv)
{
    struct timespec ts_loop_start, ts_loop_end, ts_start;

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = ctrlCHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    if(argsc == 2){
        macaddress_num = stoi(argsv[1]);

        cout << "Generating MAC address collection with " << dec << macaddress_num << " addresses.\nDepending on the amount requested please wait." << endl;
        init();
        cout << "Covid Checker initialized successfully, press Ctrl+C to stop." << endl;

        unsigned long long tick = 0;
        long dur_usec = 0;
        
        clock_gettime(CLOCK_MONOTONIC, &ts_start);
        /*
        * Alternate code that lessens the delay of each loop. Described in report
        * This is needed for initialization
        * 
        * unsigned long long currentLoopEnd = ts_start.tv_sec*1000000 + ts_start.tv_nsec/1000;
        */
        while(true){
            clock_gettime(CLOCK_MONOTONIC, &ts_loop_start);
            long delay = ts_start.tv_sec*1000000 + ts_start.tv_nsec/1000 + tick*tickDuration - ts_loop_start.tv_nsec/1000 - ts_loop_start.tv_sec*1000000;
            macaddress_t macadd = BTnearMe();
            updateContacts(macadd, tick);

            if(tick%(4*ticksInHour)==0){
                if(testCOVID()){
                    uploadContacts(tick);
                }
            }
            if(tick%ticksInDay==0){
                cout << "\nDay " << tick/ticksInDay+1 << endl << endl;
            }
            tick++;

            clock_gettime(CLOCK_MONOTONIC, &ts_loop_end);
            /*
            * Alternate code that lessens the delay of each loop. Described in report
            * This replaces the next two commands
            * 
            * currentLoopEnd += tickDuration;
            * long sleep_dur = currentLoopEnd - ts_loop_end.tv_sec*1000000 - ts_loop_end.tv_nsec/1000;
            */
            long dur_usec = (ts_loop_end.tv_sec - ts_loop_start.tv_sec)*1000000
                          + (ts_loop_end.tv_nsec - ts_loop_start.tv_nsec)/1000;
            long sleep_dur = tickDuration - dur_usec;
            usleep(sleep_dur);
            cpu_util = (cpu_util*(tick-1) + double(dur_usec)/tickDuration)/tick;
        }
    } else {
        cout << "Usage: <executable> [number of mac addresses to generate]" << endl;
    }
    return 0;
}

void ctrlCHandler(int signum){
    cout << "\rExiting gracefully." << endl;
    cout << "CPU usage: " << cpu_util << "%%" << endl;

    usleep(500);
    exit(0);
}
