#include "covidTrace.hpp"

using namespace std;

extern int macaddress_num;
extern macaddress_t* macaddress_list;
extern map<macaddress_t,int> close_contacts;

macaddress_t BTnearMe()
{
    int index = randomInt(0, macaddress_num-1);
    return macaddress_list[index];
}

bool testCOVID(){
    bool isPositive =  randomInt(1,20) > 19;
    return isPositive;
}

void uploadContacts(int tick)
{
    ofstream f;
    stringstream filename;
    filename << "./notificationLog.txt";
    f.open(filename.str(), ofstream::app);

    map<macaddress_t,int>::iterator it;
    for(it = close_contacts.begin(); it!= close_contacts.end(); it++)
    {
        macaddress_t macadd = it->first;

        stringstream ss;
        ss << hex << macadd.macaddress[0] << "." << macadd.macaddress[1] << "." << macadd.macaddress[2] << "." << macadd.macaddress[3] << "." << macadd.macaddress[4] << "." << macadd.macaddress[5] << " " << "notified" << endl;

        f << ss.str();
    }
    cout << "All relevant contacts notified" << endl;
    f.close();
}
