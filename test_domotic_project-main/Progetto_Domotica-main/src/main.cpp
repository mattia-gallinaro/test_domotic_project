//verranno eliminati gli include tra poco (devo fare le include guards solo)
#include<string>
#include<list>
#include<vector>
#include<sstream>
#include<iostream>
#include<fstream>
#include"ProgrammedCycle.h"
#include"Manual.h"
#include"ControlSystem.h"
//max kw è di 3.5

double convert_rete_elettrica_value(const std::string& value);

int main(int argc, char* argv[]){

    double val = 3.5;
    std::string test;
    std::string output;
    std::string support;
    if(argc >= 2){
        try{
            double val = convert_rete_elettrica_value(argv[1]);// in teoria dovrebbe essere il secondo argomento usando cmake
        }catch(std::logic_error e){
            std::cout << "Il valore inserito come argomento non va bene" << std::endl;
            return 1;
        }
    }
    bool done = false;
    while(!done){
        output = "";
        support += "Inserisci la contribuzione energetica del impianto fotovoltaico\n";
        std::cout << support ;
        try{
            std::getline(std::cin, test);
            support += test;
            if(std::stod(test) > 0){
                done = true;
            }else throw std::invalid_argument("");
        }catch(std::logic_error e){
            output = "Valore non accettabile \n";
            std::cout << output;
        }
        support += output;
    }

    ControlSystem cs(std::stod(test), val);

    cs.save_log(support);

    while(!cs.dayFinished()){
        support = "Inserisci un comando \n";
        std::cout << support;
        try{
            std::getline(std::cin, test);
            support += (test + "\n");
            output = cs.execute_command(test);
            std::cout << output << std::endl;
        }catch(std::logic_error e){
            output = e.what();
            std::cout << output << std::endl;
        }
        cs.save_log(support + output);
    }
    return 0;
}

double convert_rete_elettrica_value(const std::string& value){
    try{
        double val = std::stod(value);
        if(val <= 0.1 || val  > 3.5) throw std::invalid_argument("valore non accettabile");
        return val;
    }catch(std::logic_error e){
    	throw std::invalid_argument("Valore non accettabile");
    }
}
