//AUTORE: DIEGO MARCHINI

#include "Manual.h"

//costruttore -> richiesti nome e apporto energetico
 Manual::Manual(std::string name_device, double contribution){
        this->name = name_device;
        this->energy_contribution = contribution;
        this->state = false; //dispositivo inizialmente spento
        this->starting_time = -1; //valori che indicano che non è previsto per ora un accensione/spegnimento 
        this->end_time = -1;
        this->tot_energy_consumed = 0; //alla costruzione il dispositivo non ha consumato/prodotto energia
        this->ID = shared_IDs++;
 }
 
// funzione che accende il dispositivo richiedendo come parametro il tempo corrente
// restituisce una stringa utile per il log 
 std::string Manual::setOn(unsigned int time){
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

// funzione che imposta un timer di accensione e/o spegnimento(opzionale) richiedendo il tempo corrente e gli orari di accesione/spegnimento
// restituisce una stringa utile per il log
 std::string Manual::setTimer(unsigned int time, unsigned int start, unsigned int end){
       std::string s;
       std::string time_log = convert_time(time);
       if(start < 0 || start > 1439) throw std::invalid_argument("L'orario di accensione deve essere compreso tra 00:00 e 23:59");
       if(state) throw std::invalid_argument("Dispositivo già in funzione");
       if(end != 2000){//se è stato fornito anche un orario di spegnimento
              if(end < 0 || end > 1439) throw std::invalid_argument("L'orario di spegnimento deve essere compreso tra 00:00 e 23:59");
              if(end <= start) throw std::invalid_argument("L'orario di spegnimento deve essere successivo all'accensione");
              starting_time = start;
              std::string start_log = convert_time(start);
              end_time = end;
              std::string end_log = convert_time(end);
              s = "[" + time_log + "] Impostato un timer per il dispositivo '" + name+ "["+ std::to_string(ID) + "]' dalle "+ start_log +" alle "+ end_log +"\n";
       }
       else{//altrimenti imposto solo un'orario di accensione programmata
              starting_time = start;
              std::string start_log = convert_time(start); 
              s = "[" + time_log + "] Impostata l'accensione del dispositivo '" + name+ "["+ std::to_string(ID) +"]' per le "+ start_log + "\n";
       }
       return s;
 }

// funzione che rimuove i timer dal dispositivo conservando il suo stato corrrente (acceso/spento)
// restituisce una stringa utile per il log
 std::string Manual::reset_timer(unsigned int time){
        if(state){
            end_time = time;
            this->add_energy_consumed(); //sommo il valore dell'energia consumata/prodotta prima di resettare
            starting_time = time; 
        }
        end_time = -1;
        std::string time_log = convert_time(time);
        std::string s = "["+ time_log +"] Rimosso il timer dal dispositivo '"+ name+ "["+ std::to_string(ID) + "]'\n";
        return s;
 }
