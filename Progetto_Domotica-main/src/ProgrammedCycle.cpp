#include "ProgrammedCycle.h"

 ProgrammedCycle::ProgrammedCycle(std::string name_device, double contribution, unsigned int work_period){
        this->work_period = work_period;
        this->name = name_device;
        this->energy_contribution = contribution;
        this->state = false;
        this->starting_time = -1;
        this->end_time = -1;
        this->tot_energy_consumed = 0;
        shared_IDs++;
 }
 
std::string ProgrammedCycle::setOn(unsigned int time){
        std::string s;
        if(!state){
            starting_time = time;
            end_time = time + work_period;
            state = true;
            std::string time_log = convert_time(time);
            s = "[" + time_log+"] Il dispositivo '"+ name+ " ["+ std::to_string(ID) + "]' si è acceso\n";
        }
        else s = "Dispositivo '"+ name+ "["+ std::to_string(ID) + "]' già spento\n";
        return s;
}
/*
std::string ProgrammedCycle::setOff(unsigned int time){//non serve fare controlli dell'orario perchè l'orario i sistema è già checkato nel main 
         std::string s;
         if(state){
             end_time = time;
             this->add_energy_consumed();
             state = false;   
             std::string time_log = convert_time(time);
             s = "[" + time_log+"] Il dispositivo '"+ this + "' si è spento\n";   
         }
         else s = "Dispositivo "+ this + " già spento\n";
         return s;
  } 
*/
std::string ProgrammedCycle::setTimer(unsigned int time, unsigned int start, unsigned int end){
     std::string s;
     std::string time_log = convert_time(time);
     if(start < 0 || start > 1439) throw std::invalid_argument("L'orario di accensione deve essere compreso tra 00:00 e 23:59");
     if(state) throw std::invalid_argument("Dispositivo già in funzione");
     if(end != 2000) throw std::invalid_argument("Impossibile impostare un orario di spegnimento per un dispositivo PC");
     starting_time = start;
     std::string start_log = convert_time(start);
     end_time = starting_time + work_period;
     std::string end_log;
     if(end_time > 1439) end_log = convert_time(1439);
     else end_log = convert_time(end_time);
     s = "[" + time_log + "] Impostato un timer per il dispositivo " + name+ "["+ std::to_string(ID) + "] dalle "+ start_log +" alle "+ end_log +"\n";
     return s;
}
std::string ProgrammedCycle::reset_timer(unsigned int time){
        if(state){
            end_time = time;
            this->add_energy_consumed();
            starting_time = time;
        }
        else{
            starting_time = -1;
            end_time = -1;
        }  
        std::string time_log = convert_time(time);
        std::string s = "["+ time_log +"] Rimosso il timer dal dispositivo "+ name+ "["+ std::to_string(ID) + "]\n";
        return s;
}
