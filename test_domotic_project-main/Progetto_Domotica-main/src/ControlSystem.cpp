//author Gallinaro Mattia
#include "ControlSystem.h"

void ControlSystem::read_devices_from_file(){
    std::string text;
    std::string tmp;
    std::vector<std::string> current_device;
    std::stringstream ss;
    int i = 0;
    std::ifstream FileReader("../src/dispositivi.txt");
    while(std::getline(FileReader, text)){
        ss << text;
        while(std::getline(ss,tmp, ',')){
            current_device.push_back(tmp);
        }
        //std::cout << current_device[0] << std::endl;
        switch (current_device[1] == "Manuale")
        {
            case true:{
                devices.push_back(new Manual(current_device[0], std::stod(current_device[2])));
                break;
            }
            case false:{
                devices.push_back(new ProgrammedCycle(current_device[0], std::stod(current_device[2]), std::stoi(current_device[1])));
            break;
            }
        }
        ss.clear();
        current_device.erase(current_device.begin(), current_device.end());
        i++;
    }
}

std::vector<std::string> ControlSystem::divide_input_string(const std::string& input){
    std::vector<std::string> output;
    std::stringstream ss(input);
    std::string item;
    while(std::getline(ss, item, ' ')){
        output.push_back(item);
    }
    switch(std::distance(std::find(commands.begin(), commands.end(), output[0]) , commands.end())){
        case 4:
        {
            if(output.size() <= 2) throw std::invalid_argument("Il comando inserito non e' corretto");
            int last_pos = output.size() - 1;
            if(output[output.size() -1].find(":") != std::string::npos && output[output.size() -2].find(":") != std::string::npos) last_pos--;
            output[1] = put_together(output, 1, last_pos);
            output.erase(output.begin() + 2, output.begin() + last_pos);
        }
        break;
        default:
            if(output.size() <= 1) break;
            output[1] = put_together(output, 1, output.size());
            output.erase(output.begin() + 2, output.end());
            break;
    }
    return output;
}

std::string ControlSystem::put_together(std::vector<std::string>& tmp,int start_pos, const int& last_pos){
    std::string text = "";
    for(; start_pos < last_pos - 1; start_pos++)text += tmp[start_pos] + " ";
    text += tmp[last_pos - 1];
    return text;
}

double ControlSystem::get_current_contribution(){
    double sum_contribution = rete_elettrica;
    if(impianto.get_status()) sum_contribution += impianto.get_contribuition();
    for(auto i : devices)if(i->get_status()){
        sum_contribution += i->get_contribuition();
    }
    return sum_contribution;
}

std::string ControlSystem::turn_off_devices(){
    std::string output = "";
    int i = 0;
    double actual_contribution = get_current_contribution();
    while(actual_contribution <= 0){
       while(!devices[i]->get_status()) i++;
       if(i >= devices.size()) break;
       output += devices[i]->setOff(house_time);
       actual_contribution -= devices[i]->get_contribuition();
    }
    return output;
}


void ControlSystem::order_devices(const int& pos){
    /*for(int i = 1; i < devices.size(); i++){
        if(!devices[i]->get_status()) continue;
        Device* dev = devices[i]; 
        int j = i - 1;
        while( j >= 0 && (devices[j]->get_start_time() < devices[i]->get_start_time() || !devices[j]->get_start_time())){
            devices[j + 1] = devices[j];
            j -= 1;
        }
        devices[j+1] = dev;
    }*/
   for(int i = pos;i >= 1;i--){
      Device* d = devices[i - 1];
      devices[i-1] = devices[i];
      devices[i] = d;
   }
}

void ControlSystem::save_log(const std::string& event){
    std::ofstream filestream;
    filestream.open("log.txt", std::ios_base::app); //serve per aprire un file in modalità append
    filestream << event << std::endl ;
    filestream.close();
}

void ControlSystem::reset_all(){
    house_time = 0;
    impianto.setOff(house_time);
    for(auto i : devices){
        //i->setOff(house_time);
        i->reset_timer(house_time);
        i->reset_state();
    }
    action_timestamp.erase(action_timestamp.begin(), action_timestamp.end());//rimuovo tutti i timer presenti
}

void ControlSystem::reset_timers(){

    action_timestamp.erase(action_timestamp.begin(), action_timestamp.end());//per rimuovere tutti i timer dei dispositivi
    for(auto i : devices){
        i->reset_timer(house_time);
        if(dynamic_cast<ProgrammedCycle*>(i) != nullptr && i->get_status()) add_action_to_vector( i->get_name(),house_time + dynamic_cast<ProgrammedCycle *>(i)->work_period, false, true);
    }
}


void ControlSystem::reset_time(){
    house_time = 0;
    impianto.setOff(house_time);
    for(auto i : devices){
        i->reset_timer(house_time);
        i->reset_state();
    }
    remove_destroyable_timers();
}

int ControlSystem::find_device_by_name(const std::string& name){
    int pos = -1;
    for(int i = 0; i < devices.size(); i++){
        if(devices[i]->get_name() == name){
            return i;
        }
    }
    return pos; 
}

std::string ControlSystem::remove_timer(const std::string& name){
    std::string output = "";
    if(impianto.get_name() == name){
        output = impianto.reset_timer(house_time);
        remove_action(impianto.get_name());
        return output;
    }
    int pos = find_device_by_name(name);
    if(pos != -1){// se lo trovo allora in pos ho la posizione nell'array del dispositivo richiesto
        output = devices[pos]->reset_timer(house_time);
        remove_action(devices[pos]->get_name());
        if(dynamic_cast<ProgrammedCycle*>(devices[pos]) != nullptr) add_action_to_vector(devices[pos]->get_name(), dynamic_cast<ProgrammedCycle*>(devices[pos])->work_period + house_time, false, true);
        return output;
    }
    else{throw std::invalid_argument("Il nome inserito non è corretto");}
}

std::string ControlSystem::show_device(const std::string& name){
    double energy_consumed = 0;
    if(name == impianto.get_name()){
        energy_consumed = impianto.get_energy_consumed();
        std::string output =  "Il Dispositivo " + name + " ha attualmente generato " + std::to_string(energy_consumed) +" kWh \n";
        return output;
    }
    else{
        int pos = find_device_by_name(name);
        if(pos == -1) throw std::invalid_argument("Il dispositivo non esiste");
        else{energy_consumed = devices[pos]->get_energy_consumed();}
    }
    std::string output =  "Il Dispositivo " + name + " ha attualmente consumato " + std::to_string(energy_consumed) +" kWh \n";
    return output;
}

std::string ControlSystem::show_all(){
    double energy_consumed = 0;
    std::string output = "";
    std::string log_devices = "";
    for(auto i : devices){
        energy_consumed += i->get_energy_consumed();
        output = std::to_string(i->get_energy_consumed());
        output.erase(output.begin()); 
        log_devices += "   -  Il dispositivo "+ i->get_name() + " ha attualmente consumato " + output + " kWh \n";
    }
    std::string energy_dev = std::to_string(energy_consumed);
    energy_dev.erase(energy_dev.begin());
    output = "["+ convert_time(house_time) + "] Attualmente il sistema ha prodotto (compresa la rete elettrica) " 
            + std::to_string((impianto.get_energy_consumed() + rete_elettrica * (double)(house_time / 60.0))) 
            + "kWh e consumato " + energy_dev + " kWh. Nello specifico: \n" + log_devices;
    return output;
}

std::string ControlSystem::set_device_on(const std::string& name){
    std::string output;
    if(name == impianto.get_name()){
        output = impianto.setOn(house_time);
        return output;
    }
    int pos = find_device_by_name(name);
    if(pos == -1)throw std::invalid_argument("Il nome inserito non è presente tra i dispositivi disponibili");
    else{
        if(!devices[pos]->get_status() && dynamic_cast<ProgrammedCycle*>(devices[pos]) != NULL) add_action_to_vector(devices[pos]->get_name(),house_time + dynamic_cast<ProgrammedCycle *>(devices[pos])->work_period, false, true);
        output = devices[pos]->setOn(house_time);
        order_devices(pos);
        if(get_current_contribution() <= 0) output += turn_off_devices();
        return output;
    }
}

int ControlSystem::convert_string_time(const std::string& name){
    std::stringstream s(name);
    std::vector<std::string> texts;
    std::string item;
    while(std::getline(s, item, ':')){
        texts.push_back(item);
    }
    if(texts.size() != 2) throw std::invalid_argument("Il tempo deve essere scritto come HH:MM");
    int hour = std::stoi(texts[0]);
    int minutes = std::stoi(texts[1]);
    if(hour < 0 || hour > 23 || minutes < 0 || minutes > 59) throw std::invalid_argument("Time inserted is not valid");
    return hour * 60 + minutes;
}

std::string ControlSystem::set_time_of_day(const std::string& time_to_reach){
    std::string output = "";
    int end_time = convert_string_time(time_to_reach);
    int i = 0;
    while(house_time <= end_time && house_time < 1440){ //1440 corrisponde alla mezzanotte del giorno successivo
        if(action_timestamp.size() > 0){
                while(action_timestamp[i].time_action < house_time)i++;
                while(i < action_timestamp.size() && action_timestamp[i].time_action == house_time){
                    if(action_timestamp[i].to_activate){
                        output += set_device_on(action_timestamp[i].name);
                    }else{
                        if(impianto.get_name() == action_timestamp[i].name || devices[find_device_by_name(action_timestamp[i].name)]->get_status())output += set_device_off(action_timestamp[i].name);
                    }
                    if(get_current_contribution() < 0) output += turn_off_devices();
                    if(action_timestamp[i].destoyable_action){
                        action_timestamp.erase(action_timestamp.begin() + i);
                        i--;
                    }
                    i++;
                }
        }
        house_time++;
    } 
    house_time--;
    output += "[" + time_to_reach+ "] L'orario attuale e' " + time_to_reach;
    return output;
}

std::string ControlSystem::set_timer_device(const std::string& name , const std::string& start, const std::string& end){
    int start_time = convert_string_time(start);
    int end_time = 2000;
    if(end != "")end_time = convert_string_time(end);
    std::string output = "";
    if(name == impianto.get_name()) {
        output = impianto.setTimer(house_time, start_time, end_time);
        add_action_to_vector(impianto.get_name(), start_time, true, false);
        if(end_time != -1)add_action_to_vector(impianto.get_name(), end_time, false , false);
        std::cout << action_timestamp.size() <<std::endl;
        return output;
    }
    int pos = find_device_by_name(name);
    if(pos == -1) throw std::invalid_argument("Il dispositivo richiesto non esite");
    if(end_time == 2000 && dynamic_cast<ProgrammedCycle*>(devices[pos]) != nullptr)end_time = start_time + dynamic_cast<ProgrammedCycle *>(devices[pos])->work_period;
    if(end_time == 2000 ){
        output = devices[pos]->setTimer(house_time, start_time);
        add_action_to_vector(devices[pos]->get_name(), start_time, true);
    }
    else{
        if(dynamic_cast<ProgrammedCycle*>(devices[pos]) != nullptr){
            if(end != "")throw std::invalid_argument("Non puoi inserire un tempo per i dispositivi programmati");
            output = devices[pos]->setTimer(house_time, start_time);
            add_action_to_vector(devices[pos]->get_name(), start_time, true);
            add_action_to_vector(devices[pos]->get_name(), end_time, false, true);
        }else{
            output = devices[pos]->setTimer(house_time, start_time, end_time);
            add_action_to_vector(devices[pos]->get_name(), start_time, true);
            add_action_to_vector(devices[pos]->get_name(), end_time, false);
        }
    }
    return output;
}

std::string ControlSystem::execute_command(const std::string& input){
    std::string output = "[" + convert_time(house_time) +"] L'orario attuale e' " + convert_time(house_time) + "\n";
    std::vector<std::string> command_inserted = divide_input_string(input);
    switch(std::distance(std::find(commands.begin(), commands.end(), command_inserted[0]), commands.end())){
        //caso per set
        case 4:
            //tento di dividere il più possibile il numero di controlli andando a vedere se l'ultimo elemento del comando era una tempo 
            if(command_inserted.size() <= 2) throw std::invalid_argument("Il comando inserito non e' corretto");
            if(command_inserted[command_inserted.size() -1].find(":") != std::string::npos){
                //ora devo controllare se ho eseguito un set time o set device time_start time_end basta controllare il secondo elemento
                if(command_inserted[1] != "time"){//chiamo funzione con set device e i due time
                    std::string start_time = command_inserted[command_inserted.size() - 1];
                    std::string end_time = "";
                    if(command_inserted[command_inserted.size() - 2].find(':')!= std::string::npos){
                        end_time = start_time;
                        start_time = command_inserted[command_inserted.size() - 2];
                    }
                    output += set_timer_device(command_inserted[1], start_time, end_time);
                }
                else{// chiamo funzione con set time tempo
                    output += set_time_of_day(command_inserted[command_inserted.size() - 1]);
                }
            }else{
                if(command_inserted[command_inserted.size() - 1] == "on"){
                    output += set_device_on(command_inserted[1]);//funzione per attivare un dispostivo
                }
                else if(command_inserted[command_inserted.size() - 1] == "off"){
                    output += set_device_off(command_inserted[1]);
                }//funzione per disattivare un dispositivo
                else throw std::invalid_argument("Il comando non esiste");
                if(get_current_contribution() < 0) output += turn_off_devices();
            }
            break;
        //caso per remove
        case 3:
            if(command_inserted.size() <= 1) throw std::invalid_argument("Il comando inserito non è corretto");
            output += remove_timer(command_inserted[command_inserted.size() - 1]); // che dovrebbe corrispondere a command_inserted[1] siccome con remove contiene al massimo 2 elementi
            break;
        //caso per show
        case 2:
            //può sia contenere un nome che no
            if(command_inserted.size() == 1){
                output = show_all();//per mostrare 
            }
            else{
                output = show_device(command_inserted[1]);//mostra solo quel dispositivo
            }
            break;
        //caso per reset:
        case 1:{
            if(command_inserted.size() != 2) throw std::invalid_argument("Il comando inserito non è corretto");
            if(command_inserted[1] == "time") {
                reset_time();
                output = "[00:00] L'orario attuale e' 00:00 \n";
            }
            else if(command_inserted[1] == "timers") reset_timers();
            else if(command_inserted[1] == "all"){
                reset_all();
                output = "[00:00] L'orario attuale e' 00:00 \n";
            }
            else throw std::invalid_argument("Il comando inserito non e' corretto");
           break;
            }
        default:
            throw std::invalid_argument("Il comando inserito non esiste ");
            break;
    }
    save_log(output);
    return output;
}

void ControlSystem::orderActions(){
    for(int i = 1; i < action_timestamp.size(); i++){
        Action act = action_timestamp[i]; 
        int j = i - 1;
        while( j >= 0 && action_timestamp[j].time_action > action_timestamp[i].time_action){
            action_timestamp[j + 1] = action_timestamp[j];
            j -= 1;
        }
        action_timestamp[j+1] = act;
    }
}

std::string ControlSystem::set_device_off(const std::string& name){
    if(name == impianto.get_name()){
        return impianto.setOff(house_time);
    }
    int pos = find_device_by_name(name);
    if(pos == -1)throw std::invalid_argument("Il nome inserito non è presente tra i dispositivi disponibili");
    else{
        return devices[pos]->setOff(house_time);
    }
}

void ControlSystem::add_action_to_vector(const std::string& name ,const int& time_act, bool action, bool destroyable){
    bool found = false;
    for(int i = 0; i < action_timestamp.size(); i++){
        if(action_timestamp[i].name == name && action == action_timestamp[i].to_activate){
            action_timestamp[i].time_action = time_act;
            found = true;
            action_timestamp[i].destoyable_action = destroyable;
            break;
        }
    }
    if(!found){
        Action test(name, time_act, action, destroyable);
        action_timestamp.push_back(test);
    }
    orderActions();
}

void ControlSystem::remove_action(const std::string&name){
    int i = 0;
    while(i < action_timestamp.size()){
        if(action_timestamp[i].name == name){
            action_timestamp.erase(action_timestamp.begin() + i);
            i--;
        }
        i++;
    }
}

void ControlSystem::remove_destroyable_timers(){
    for(int i = 0; i < action_timestamp.size();i++){
        if(action_timestamp[i].destoyable_action){
            action_timestamp.erase(action_timestamp.begin() + i);
            i--;
        }
    }
}
