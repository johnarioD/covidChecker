#include "basicUtils.hpp"

using namespace std;

extern int macaddress_num;
extern macaddress_t* macaddress_list;
extern vector<macaddress_t> contacts;
extern map<macaddress_t,int> close_contacts;

void init(){
    srand(time(NULL));
    macaddress_list = (macaddress_t*)malloc(macaddress_num * sizeof(macaddress_t));
    for(int i = 0; i < macaddress_num; i++){
        macaddress_list[i].macaddress[0] = randomInt(0,255);
        macaddress_list[i].macaddress[1] = randomInt(0,255);
        macaddress_list[i].macaddress[2] = randomInt(0,255);
        macaddress_list[i].macaddress[3] = randomInt(0,255);
        macaddress_list[i].macaddress[4] = randomInt(0,255);
        macaddress_list[i].macaddress[5] = randomInt(0,255);
    }
}

int randomInt(int low, int high){
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    random_device rd;
    mt19937::result_type seed = rd()^(unsigned long)current_time.tv_sec;
    mt19937 gen(seed);
    uniform_int_distribution<> distrib(low,high);
    
    return distrib(gen);
}

void updateContacts(macaddress_t macadd, int tick){
    contacts.push_back(macadd);

    if(contacts.size() > 20*ticksInMin)
    {
        contacts.erase(contacts.begin());
    }

    bool newCloseContact = false;
    if(contacts.size() > 4*ticksInMin)
    {
        for(int i = 4*ticksInMin; i < contacts.size(); i++)
        {
            if(contacts[i] == macadd)
            {
                auto close_contacts_index = close_contacts.find(macadd);

                if(close_contacts_index==close_contacts.end())
                {
                    close_contacts.insert(pair<macaddress_t,int>(macadd, tick));
                    newCloseContact = true;
                } else  {
                    close_contacts_index->second = tick;
                }
            }
        }
    }
    
    if(newCloseContact){
        map<macaddress_t,int>::iterator it = close_contacts.begin();
        while(it!=close_contacts.end()){
            if(it->second - tick > 14*ticksInDay){
                close_contacts.erase(it);
            } else {
                it++;
            }
        }
    }
}
