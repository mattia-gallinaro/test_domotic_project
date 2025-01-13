#ifndef MANUAL_H
#define MANUAL_H

#include"Device.h"

class Manual : public Device{
    public:
        Manual(std::string name_device, double contribution);
        //std::string setOff(unsigned int time);//ererditato
        std::string setOn(unsigned int time);//ereditato
        //void setStart(unsigned int time);//ereditato
        //void setEnd(unsigned int time);//originale
        std::string setTimer(unsigned int time, unsigned int start, unsigned int end = 2000);//ereditato
        std::string reset_timer(unsigned int time); //ereditato
};

#endif //MANUAL_H