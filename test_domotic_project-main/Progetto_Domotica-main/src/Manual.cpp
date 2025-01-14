#include "Manual.h"

 Manual::Manual(std::string name_device, double contribution){
        this->name = name_device;
        this->energy_contribution = contribution;
        this->state = false;
        this->starting_time = -1;
        this->end_time = -1;
        this->tot_energy_consumed = 0;
        this->ID = shared_IDs++;
 }
 
 std::string Manual::setOn(unsigned int time){//non serve fare controlli dell'orario perchè l'orario i sistema è già checkato nel main 
        std::string s;
        if(!state){
            starting_time = time;
            state = true;
            std::string time_log = convert_time(time);
            s = "[" + time_log+"] Il dispositivo '"+ name+ "["+ std::to_string(ID) + "]' si è acceso\n";
        }
        else s = "Dispositivo '"+ name+ "["+ std::to_string(ID) + "]' già acceso\n";
        return s;
 }
/*
 std::string Manual::setOff(unsigned int time){//non serve fare controlli dell'orario perchè l'orario i sistema è già checkato nel main 
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
 std::string Manual::setTimer(unsigned int time, unsigned int start, unsigned int end){
       std::string s;
       std::string time_log = convert_time(time);
       if(start < 0 || start > 1439) throw std::invalid_argument("L'orario di accensione deve essere compreso tra 00:00 e 23:59");
       if(state) throw std::invalid_argument("Dispositivo già in funzione");
       if(end != 2000){
              if(end < 0 || end > 1439) throw std::invalid_argument("L'orario di spegnimento deve essere compreso tra 00:00 e 23:59");
              if(end <= start) throw std::invalid_argument("L'orario di spegnimento deve essere successivo all'accensione");
              starting_time = start;
              std::string start_log = convert_time(start);
              end_time = end;
              std::string end_log = convert_time(end);
              s = "[" + time_log + "] Impostato un timer per il dispositivo " + name+ "["+ std::to_string(ID) + "] dalle "+ start_log +" alle "+ end_log +"\n";
       }
       else{
              starting_time = start;
              std::string start_log = convert_time(start); 
              s = "[" + time_log + "] Impostata l'accensione del dispositivo " + name+ "["+ std::to_string(ID) +"] per le "+ start_log + "\n";
       }
       return s;
 }

 std::string Manual::reset_timer(unsigned int time){
        if(state){
            end_time = time;
            this->add_energy_consumed();
            starting_time = time; 
        }
        end_time = -1;
        std::string time_log = convert_time(time);
        std::string s = "["+ time_log +"] Rimosso il timer dal dispositivo "+ name+ "["+ std::to_string(ID) + "]\n";
        return s;
 }
