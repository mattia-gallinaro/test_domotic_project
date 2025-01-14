#ifndef CONTROL_H
#define CONTROL_H

#include "Manual.h"
#include "ProgrammedCycle.h"
#include "Device.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <map>

class ControlSystem{
    public:
        ControlSystem(const double& value_impianto, const double& rete = 3.5) : impianto("Impianto fotovoltaico", value_impianto){
            rete_elettrica = rete;
            read_devices_from_file();
        };
        ControlSystem(const std::string& event_log) : impianto("Impianto fotovoltaico", 1){
            save_log(event_log);
        };
        std::string execute_command(const std::string& input);
        bool dayFinished(){
            return house_time >= 1439;
        };
        void save_log(const std::string& event);
    private:

        struct Action{
            int time_action;
            std::string name;
            bool to_activate;
            bool destoyable_action ;
            Action(std::string nam, int time_act, bool act, bool destroyable = false) {
                name = nam;
                time_action = time_act;
                to_activate = act;
                destoyable_action = destroyable;
            };
        };

        std::string turn_off_devices(void);
        void order_devices(const int& pos);
        void read_devices_from_file(void);
        std::string set_device_on(const std::string& name);
        std::string set_device_off(const std::string& name);
        std::string set_time_of_day(const std::string& time_to_reach);
        std::string show_device(const std::string& name);
        std::string show_all(void);
        void reset_all(void);
        void reset_time(void);
        void reset_timers(void);
        std::string remove_timer(const std::string& name);
        std::string set_timer_device(const std::string& name , const std::string& start_time, const std::string& end_time = "");
        int convert_string_time(const std::string& name);
        void orderActions();
        void add_action_to_vector(const std::string& name,const int& time,bool activate, bool destroyable = false);
        void remove_action(const std::string& name);
        void remove_destroyable_timers(void);

        int find_device_by_name(const std::string& name);
        std::string put_together(std::vector<std::string>& tmp,int start_pos, const int& last_pos);
        std::vector<std::string> divide_input_string(const std::string &input);
        double get_current_contribution();

        int house_time = 0;
        double rete_elettrica; //indica quanta corrente si può prendere costantemente 
        Manual impianto;

        //std::vector<Manual> manualDevices;
        //std::vector<ProgrammedCycle> programDevices;
        std::vector<Device*> devices;
        std::vector<std::string> commands ={"set","rm","show","reset"};
        std::vector<Action> action_timestamp;
};

#endif
