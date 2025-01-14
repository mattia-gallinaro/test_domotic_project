#ifndef DEVICE_H
#define DEVICE_H

#include<iostream>
#include<string>
#include<stdexcept>
class Device{
    protected:
    static int shared_IDs;
    double energy_contribution; // perchè può anche mettere 0.2 KW
    bool state; // true = acceso , false = spento
    double tot_energy_consumed;
    int starting_time; //espresso in minuti es. 71 = 71 min = 01:11; 1024 = 1024 min = 17:04;
    int end_time;      //espresso in minuti es. 71 = 71 min = 01:11; 1024 = 1024 min = 17:04;
    std::string name;
    unsigned int ID;
    void add_energy_consumed(void);
    /*
    void add_energy_consumed(void){
        double hours = (double)((end_time - starting_time) / 60);
        tot_energy_consumed += energy_contribution * hours;
    };
    */
   
    public:
    std::string setOff(unsigned int time);
    //virtual std::string setOff(unsigned int time);
    virtual std::string setOn(unsigned int time) = 0;
    //virtual void setStart(unsigned int time);
    virtual std::string setTimer(unsigned int time, unsigned int start, unsigned int end = 2000) = 0;
    virtual std::string reset_timer(unsigned int time) = 0;//gli passo il momemento in cui è stato dato il reset time così posso salvare prima l'energia consumata e poi indico
    void reset_state(void);
    // getter functions
    bool get_status(void) const;
    unsigned int get_start_time(void) const;
    double get_contribuition(void) const;
    double get_energy_consumed(void) const;
    std::string get_name(void) const;
    int get_ID(void) const;
    virtual ~Device();
    /*
    void reset_state(void){
        state = false;
        tot_energy_consumed = 0;
    }
    bool get_status(void) const {return state;};
    unsigned int get_start_time(void) const {return starting_time;};
    double get_contribuition(void) const {return energy_contribution;};
    double get_energy_consumed(void) const {return tot_energy_consumed;};
    std::string get_name(void) const { return name;};
    int get_ID(void) const {return ID;};
*/
    
};

std::string convert_time(unsigned int time);
/*

std::string convert_time(unsigned int time){
        unsigned int hours = time/60;
        unsigned int minutes = time%60;
        std::string time_converted = hours + ":" + minutes;
        return time_converted;
    };
*/

#endif //DEVICE_H
