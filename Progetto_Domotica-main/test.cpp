#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <map>
struct Device{
    std::string name;
    double contrib;
    bool active = false;
    int start_time = 0;
};

struct Point{
    int arr_corrisp;
    int pos;
};

struct Action{
    int time_of_action;//mi serve per organizzare quando un'azione deve avvenire
    Point device_to_act_on;
    bool activate_device;
};
//void read_devices_from_file();

//void turn_off_devices();

//Device impianto_fotovoltaico;
//std::vector<Device> manualDevices;
//std::vector<Device> programDevices;

//std::vector<std::string> commands ={"set","rm","show","reset"};
//std::vector<std::string> divide_input_string(const std::string &input);

//double check_and_convert_contatore_contr_value(const std::string& value);
void function_test_commands(const std::string& input);
std::string put_together(std::vector<std::string> &tmp , int start_pos, const int& end_pos);
std::string get_device_name(Point& pos);

Point find_device_by_name(const std::string& name);

void show_all();
void show(const std::string& name,const int& arr_corrisp = -1,const int& pt = -1);
void set_device_off(const std::string& name, Point* pt = NULL);
void set_time(const std::string& time_to_go);
void set_on(const std::string& name , Point* pt = NULL);
void remove_timer(const std::string& name);
void order_manual_devices();// mi serve un modo per capire chi è stato avviato per ultimo e quindi se ordino i dispositivi 
                                   //in ordine decrescente rispetto all'orario di avvio allora il primo dispositivo di ogni vettore corrisponderà al ultimo dispositivo avviato di quella tipologia
void order_programmed_devices();

/*Sono per debug*/
//void reset_all();
//void reset_time();
//void reset_timers();


std::vector<Action> action_timestamp;

double contribuzione_alimentazione;
static int house_time = 0;

int main(int argc, char* argv[]){
    int clock = 0; // tempo della giornata di defualt parte da 0
    //per ora faccio due liste di oggetti , uno per manuali e uno per programmedCycle
    std::string test;
    if(argc >= 2){
        try{
            double val = check_and_convert_contatore_contr_value(argv[1]);// in teoria dovrebbe essere il secondo argomento usando cmake
            std::cout << "Hai messo " << val << " kwh" << std::endl;
            contribuzione_alimentazione = val;
            //rete_elettrica = 
        }catch(std::logic_error e){
            std::cout << "Il valore inserito come argomento non va bene";
            return 1;
        }
    }//se ha aggiunto qualcosa oltre al comando di cmake (sto facendo un test)
    else {
        std::cout<<"non hai inserito nulla" << std::endl;// assumo io che dalla rete elettrica vengano estratti 3kWh 
        contribuzione_alimentazione = 3.5;
    }
    read_devices_from_file();
    std::string tmp; 
    while(house_time < 1440){ // dopo da sostituire con un controllo a if day finished di control system
    try{
        std::cout << " Inserisci un comando " << std::endl;
        std::getline(std::cin, tmp);
        function_test_commands(tmp);
    }catch(std::logic_error e){
        std::cout << "Il comando inserito e' errato , riprova" << std::endl;
    }
    }
    return 0;
}


void function_test_commands(const std::string& input){
    std::vector<std::string> test = divide_input_string(input);
    try{
    switch(std::distance(std::find(commands.begin(), commands.end(), test[0]), commands.end())){
        //caso per set
        case 4:
            //tento di dividere il più possibile il numero di controlli andando a vedere se l'ultimo elemento del comando era una tempo 
            if(test.size() <= 2) throw std::invalid_argument("Wrong argument");
            if(test[test.size() -1].find(":") != std::string::npos){
                //ora devo controllare se ho eseguito un set time o set device time_start time_end basta controllare il secondo elemento
                if(test[1] != "time"){}//chiamo funzione con set device e i due time
                else{}// chiamo funzione con set time tempo
            }else{
                if(test[test.size() - 1] == "on"){set_on(test[1]);} //funzione per attivare un dispostivo
                else{set_device_off(test[1]);}//funzione per disattivare un dispositivo
            }
            break;
        //caso per remove
        case 3:
            if(test.size() <= 1) throw std::invalid_argument("Il comando inserito non è corretto");
            remove_timer(test[test.size() - 1]); // che dovrebbe corrispondere a test[1] siccome con remove contiene al massimo 2 elementi
            break;
        //caso per show
        case 2:
            //può sia contenere un nome che no
            //bool show_present = test[0].find("show") != std::string::npos;// non serve perchè son sicuro che show ci deve essere affinchè entri
            if(test.size() == 1){show_all();} //mostra tutti i dispositivi
            else{show(test[1]);} //mostra solo quel dispositivo
            break;
        //caso per reset:
        case 1:{
            std::map<std::string, std::function<void()>> map = { {"all", &reset_all} , {"timer", &reset_time}, {"timers", &reset_timers}};
            if(map.find(test[1]) != map.end()){
                map[test[1]]();
            }else throw std::invalid_argument("");
            break;
            }
        default:
            throw std::invalid_argument("");
            break;
    }
    }catch(std::logic_error e){
    }
}

void set_time(const std::string& time_to_go){
    std::stringstream s(time_to_go);
    std::vector<std::string> hours;
    std::string item;
    while(std::getline(s, item, ':')){
        hours.push_back(item);
    }
    int hour = std::stoi(hours[0]);
    int minutes = std::stoi(hours[1]);
    if(hour < 0 || hour > 23 || minutes < 0 || minutes > 59) throw std::invalid_argument("Time inserted is not valid");
    int time_to_reach = hour * 60 + minutes;
    while(house_time < time_to_reach && house_time < 1440){ //1440 corrisponde alla mezzanotte del giorno successivo
        if(action_timestamp.size() > 0){
            while(house_time == action_timestamp[0].time_of_action){
                if(action_timestamp[0].activate_device){
                    set_on(get_device_name(action_timestamp[0].device_to_act_on), &(action_timestamp[0].device_to_act_on));
                }else{
                    set_device_off(get_device_name(action_timestamp[0].device_to_act_on), &(action_timestamp[0].device_to_act_on));
                }
                action_timestamp.erase(action_timestamp.begin(), action_timestamp.begin() + 1);
            }
        }
        house_time++;//se non voglio dover fare tante volte il ++ , basta aumentare house_time fino alla prossima azione disponibile
    }

}

void set_device_off(const std::string& name , Point* pt){
    Point pos;
    if(pt == NULL){
        pos = find_device_by_name(name);
        if(pos.arr_corrisp == -1) throw std::invalid_argument("Invalid name inserted");
    }
    else pos = {pt->arr_corrisp, pt->pos};
    if(pos.arr_corrisp == 1){
        if(manualDevices[pos.pos].active){
            manualDevices[pos.pos].active = false; // + chiamo la funzione set off di manual
            std::cout << "[" << house_time << "] Il Dispositivo " << name << "si e' spento" << std::endl;
        }
    }else{
        if(programDevices[pos.pos].active){
            programDevices[pos.pos].active = false; // + chiamo la funzione set off di programDevices
            std::cout << "["<< house_time <<"] Il Dispositivo " << name << "si e' spento" << std::endl;
        }
    }
    if(get_current_contribuition() <= 0) turn_off_devices();
}

void set_on(const std::string& name, Point* pt){
    Point pos;
    if(pt == NULL){
        pos = find_device_by_name(name);
        if(pos.arr_corrisp == -1) throw std::invalid_argument("Invalid name inserted");
    }
    else pos = {pt->arr_corrisp, pt->pos};
    if(pos.arr_corrisp == 1){
        if(!manualDevices[pos.pos].active && get_current_contribuition() + manualDevices[pos.pos].contrib > 0){
            manualDevices[pos.pos].active = true; // + chiamo la funzione set on di manual
            std::cout << "[" << house_time << "] Il Dispositivo " << name << "si e' acceso" << std::endl;
        }
    }else{
        if(!programDevices[pos.pos].active && get_current_contribuition() + programDevices[pos.pos].contrib > 0 ){
            programDevices[pos.pos].active = true; // + chiamo la funzione set on di programDevices
            std::cout << "["<< house_time <<"] Il Dispositivo " << name << "si e' acceso" << std::endl;
        }
    }
}

void reset_all(){
    house_time = 0;
    std::for_each(manualDevices.begin(), manualDevices.end(), &get_current_contribuition);//da sostituire con la funzione che mi permette di resettare il disp manuale
    std::for_each(programDevices.begin(), programDevices.end(), &get_current_contribuition);//per quello ciclico
}

void reset_time(){
    house_time = 0;
    //resetto le condizioni dei dispositivi
}

void remove_timer(const std::string& name){
    Point pos = find_device_by_name(name);
    if(pos.arr_corrisp == 0){
        //manualDevices[pos.pos].remove_timer()
    }
    else if(pos.arr_corrisp == 1){
        //manualDevices[pos.pos].remove_timer()
    }
    else{throw std::invalid_argument("");}
}


void reset_timers(){
    //std::for_each(manualDevices.begin(), manualDevices.end(), &reset_timer);
    //std::for_each(programDevices.begin(), programDevices.end(), &reset_timer);
}


//funzioni che workano

/*

double check_and_convert_contatore_contr_value(const std::string& value){
    try{
        double val = std::stod(value);
        if(val <= 0.1 || val  > 3.5) throw std::invalid_argument("valore non accettabile");
        return val;
    }catch(std::invalid_argument e){
    	throw std::invalid_argument("Invalid argument");
    }catch(std::out_of_range e){
    	throw std::invalid_argument("Invalid argument");
    }
}


Point find_device_by_name(const std::string& name){
    Point pt = {-1,-1};
    for(int i = 0; i < manualDevices.size(); i++){
        if(manualDevices[i].name == name) {
            pt = {0,i}; 
            return pt;
        }
    }
    for(int i = 0; i  < programDevices.size(); i++){
        if(programDevices[i].name == name){
            pt = {1,i};
            return pt;
        }
    }
    return pt;     
}

void show_all(){
    int pos = 0;
    for(int i = 0;i < manualDevices.size() ; i++ ){
        show(manualDevices[i].name, pos, i);
    }
    pos = 1;
    for(int i = 0;i < programDevices.size() ; i++ ){
        show(programDevices[i].name, pos, i);
    }
}

std::string get_device_name(Point& pos){
    if(pos.arr_corrisp == 0){
        return manualDevices[pos.pos].name;
    }else{
        return programDevices[pos.pos].name;
    }
}

void show(const std::string& name,const int& arr_corrisp,const int& pt){
    Point pos = {arr_corrisp, pt};
    double contrib;
    if(arr_corrisp == -1){
        pos = find_device_by_name(name);
        if(pos.arr_corrisp != 0 && pos.arr_corrisp != 1){throw std::invalid_argument("");}
    }
    if(pos.arr_corrisp == 0){contrib = manualDevices[pos.pos].contrib;}
    else{contrib = programDevices[pos.pos].contrib;}
    std::cout << "Il Dispositivo " << name << " ha consumato " << contrib <<" kWh" << std::endl;

}


*/








/*FUNZIONI IMPORTATE

//da rifinire sostituendo il .active con le funzioni di manual e programmedDevice
void turn_off_devices(){
    int i = 0, j = 0;
    double actual_contribution = get_current_contribuition();
    while(actual_contribution <= 0){
        while(!manualDevices[i].active) i++;
        while(!programDevices[j].active ) j++;
        if(manualDevices[i].start_time < programDevices[i].start_time){
            manualDevices[i].active = false;
            actual_contribution -= manualDevices[i].contrib;
        }else {
            programDevices[i].active = false;
            actual_contribution -= programDevices[i].contrib;
        }
    }
    order_manual_devices();
    order_program_devices();
}
void read_devices_from_file(){
    std::string text;
    std::string tmp;
    std::vector<std::string> current_device;
    std::stringstream ss;
    int i = 0;
    std::ifstream FileReader("./dispositivi.txt");
    while(std::getline(FileReader, text)){
        ss << text;
        while(std::getline(ss,tmp, ',')){
            current_device.push_back(tmp);;
        }
        Device device =  {current_device[0] , std::stod(current_device[2])};
        switch (current_device[1] == "Manuale")
        {
            case true:
                manualDevices.push_back(device);
                break;
            case false:
                programDevices.push_back(device);
            break;
        }
        ss.clear();
        current_device.erase(current_device.begin(), current_device.end());
        i++;
    }
}

std::vector<std::string> divide_input_string(const std::string &input){
    std::vector<std::string> output;
    std::stringstream ss(input);
    std::string item;
    while(std::getline(ss, item, ' ')){
        output.push_back(item);
    }
    switch(std::distance(std::find(commands.begin(), commands.end(), output[0]) , commands.end())){
        case 4:
        {
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

std::string put_together(std::vector<std::string> &tmp ,int start_pos,const int& end_pos){
    std::string text = "";
    for(; start_pos < end_pos - 1; start_pos++ ){
        text += tmp[start_pos] + " ";
    } 
    text += tmp[end_pos - 1];
    return text;
}

void order_manual_devices(){
    for(int i = 1; i < programDevices.size(); i++){
        if(!programDevices[i].active) continue;
        Device dev = programDevices[i]; 
        int j = i - 1;
        while( j >= 0 && (programDevices[j].start_time < programDevices[i].start_time || !programDevices[j].active)){
            programDevices[j + 1] = programDevices[j];
            j -= 1;
        }
        programDevices[j+1] = dev;
    }
}
void order_program_devices(){
    for(int i = 1; i < programDevices.size(); i++){
        if(!programDevices[i].active) continue;
        Device dev = programDevices[i]; 
        int j = i - 1;
        while( j >= 0 && (programDevices[j].start_time < programDevices[i].start_time || !programDevices[j].active)){
            programDevices[j + 1] = programDevices[j];
            j -= 1;
        }
        programDevices[j+1] = dev;
    }
}

void save_log(const std::string& event){
    std::ofstream filestream;
    filestream.open("log.txt", std::ios_base::app); //serve per aprire un file in modalità append
    filestream << event ;
    filestream.close();
}

double get_current_contribuition(){
    double sum_contribuition = contribuzione_alimentazione;
    for(auto i : manualDevices){
        if(i.active)sum_contribuition += i.contrib;
    }

    for(auto i : programDevices){
        if(i.active)sum_contribuition += i.contrib;
    }

    return sum_contribuition ; 
}
*/