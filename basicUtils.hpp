#pragma once
#include <time.h>
#include <string>
#include <iostream>
#include <functional>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <random>
#include <vector>
#include <map>

#if !defined(COVID_CHECKER_CONSTANTS)
#define COVID_CHECKER_CONSTANTS 1

const int tickDuration = 100000;
const int ticksInMin = 6;
const int ticksInHour = 360;
const int ticksInDay = 8640;

#endif

typedef struct macaddress_t
{
    unsigned char macaddress[6];
    bool operator==(const macaddress_t &o) const {
        return (macaddress[0]==o.macaddress[0]) &&
               (macaddress[1]==o.macaddress[1]) &&
               (macaddress[2]==o.macaddress[2]) &&
               (macaddress[3]==o.macaddress[3]) &&
               (macaddress[4]==o.macaddress[4]) &&
               (macaddress[5]==o.macaddress[5]);
    }
    bool operator<(const macaddress_t &o) const{
        if(macaddress[0] < o.macaddress[0]){
            return true;
        } else if((macaddress[0]==o.macaddress[0]) && (macaddress[1] < o.macaddress[1])){
            return true;
        } else if((macaddress[1]==o.macaddress[1]) && (macaddress[2] < o.macaddress[2])){
            return true;
        } else if((macaddress[2]==o.macaddress[2]) && (macaddress[3] < o.macaddress[3])){
            return true;
        } else if((macaddress[3]==o.macaddress[3]) && (macaddress[4] < o.macaddress[4])){
            return true;
        } else if((macaddress[4]==o.macaddress[4]) && (macaddress[5] < o.macaddress[5])){
            return true;
        }
        return false;
    }
} macaddress_t;

void init();
int randomInt(int low, int high);
void updateContacts(macaddress_t macadd, int tick);
