//AUTORE: DIEGO MARCHINI

#include "Device.h"

Device::~Device() = default;
int Device::shared_IDs = 0;

// MEMBER FUNCTIONS

// funzione che spegne il dispositivo richiedendo come parametro il tempo corrente
// restituisce una stringa utile per il log 
std::string Device::setOff(unsigned int time){
    std::string s;
         if(state){
             end_time = time;
             this->add_energy_consumed();
             state = false;   
             std::string time_log = convert_time(time);
             s = "[" + time_log+"] Il dispositivo '"+ name+ "["+ std::to_string(ID) + "]' si è spento\n";   
         }
         else s = "Dispositivo '"+ name+ "["+ std::to_string(ID) + "]' già spento\n";
         return s;
}

//funzione che aggiunge al totale di energia già prodotta/consumata di un dato dispositivo, l'energia prodotta in un intervallo di funzionamento
void Device::add_energy_consumed(void){
    double hours = ((double)(end_time - starting_time) / 60.0);
    tot_energy_consumed += energy_contribution * hours;
};

//funzione che resetta lo stato del dispositivo spegnendolo e portando a zero il valore dell'energia da esso consumata/prodotta
void Device::reset_state(void){
    state = false;
    tot_energy_consumed = 0;
}
// getter functions (descritte in Device.h)

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

// HELPER FUNCTIONS

//funzione usata per convertire un valore di tempo espresso in minuti in un orario nel formato HH:MM
std::string convert_time(unsigned int time){
    std::string hours = std::to_string(time/60);
    if(std::stoi(hours) < 10) hours = "0" + hours;
    std::string minutes = std::to_string(time%60);
    if(std::stoi(minutes) < 10) minutes = "0" + minutes;
    std::string time_converted = hours + ":" + minutes;
    return time_converted;
};
