#ifndef PROGRAMMEDCYCLE_H
#define PROGRAMMEDCYCLE_H

#include"Device.h"

class ProgrammedCycle : public Device{
    public:
    unsigned int work_period; //rimosso constexpr //da mettere private(?)
    ProgrammedCycle(std::string name_device, double contribution, unsigned int work_period);
    //std::string setOff(unsigned int time);//ererditato
    std::string setOn(unsigned int time);//ereditato
    //void setStart(unsigned int time);//ereditato
    std::string setTimer(unsigned int time, unsigned int start, unsigned int end = 2000);//ereditato
    std::string reset_timer(unsigned int time); //ereditato
};

#endif //PROGRAMMEDCYCLE_H