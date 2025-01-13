#include "Device.h"

Device::~Device() = default;
int Device::shared_IDs = 0;

// member functions
std::string Device::setOff(unsigned int time){
    std::string s;
         if(state){
             end_time = time;
             this->add_energy_consumed();
             state = false;   
             std::string time_log = convert_time(time);
             s = "[" + time_log+"] Il dispositivo '"+ name+ "["+ std::to_string(ID) + "] si è spento\n";   
         }
         else s = "Dispositivo "+ name+ "["+ std::to_string(ID) + "] già spento\n";
         return s;
}


void Device::add_energy_consumed(void){
    double hours = (double)((end_time - starting_time) / 60);
    tot_energy_consumed += energy_contribution * hours;
};

void Device::reset_state(void){
    state = false;
    tot_energy_consumed = 0;
}
// getter functions
bool Device::get_status(void) const {
    return state;
};

unsigned int Device::get_start_time(void) const {
    return starting_time;
};

double Device::get_contribuition(void) const {
    return energy_contribution;
};

double Device::get_energy_consumed(void) const {
    return tot_energy_consumed;
};

std::string Device::get_name(void) const {
    return name;
};

 int Device::get_ID(void) const {
    return ID;
};

// helper functions

std::string convert_time(unsigned int time){
    unsigned int hours = time/60;
    unsigned int minutes = time%60;
    std::string time_converted = hours + ":" + minutes;
    return time_converted;
};
