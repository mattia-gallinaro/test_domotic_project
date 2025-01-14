//AUTORE: DIEGO MARCHINI

#ifndef MANUAL_H
#define MANUAL_H

#include"Device.h"
//classe di dispositivi MANUALI che estende la classe astratta DEVICE
class Manual : public Device{
    public:
        //costruttore -> richiesti nome e apporto energetico
        Manual(std::string name_device, double contribution);

        //funzione che accende un dispositivo richiedendo come parametro il tempo corrente
        std::string setOn(unsigned int time);

        //funzione che imposta un timer di accensione e/o spegnimento(opzionale) richiedendo il tempo corrente e gli orari di accesione/spegnimento
        std::string setTimer(unsigned int time, unsigned int start, unsigned int end = 2000);

        //funzione che rimuove i timer dal dispositivo conservando il suo stato corrrente (acceso/spento)
        std::string reset_timer(unsigned int time);
};

#endif //MANUAL_H