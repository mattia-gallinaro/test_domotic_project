//AUTORE: DIEGO MARCHINI

#ifndef PROGRAMMEDCYCLE_H
#define PROGRAMMEDCYCLE_H

#include"Device.h"
//classe di dispositivi A CICLO PREFISSATO che estende la classe astratta DEVICE
class ProgrammedCycle : public Device{
    private:
    unsigned int work_period; //periodo di funzionamento fisso del dispositivo

    public:
    
    //costruttore -> richiesti nome, apporto energetico e periodo di funzionamento
    ProgrammedCycle(std::string name_device, double contribution, unsigned int work_period);

    //funzione che accende un dispositivo richiedendo come parametro il tempo corrente
    std::string setOn(unsigned int time);

    //funzione che imposta un timer di accensione richiedendo il tempo corrente e l'orariodi accesione
    std::string setTimer(unsigned int time, unsigned int start, unsigned int end = 2000);

    //funzione che rimuove i timer dal dispositivo conservando il suo stato corrrente (acceso/spento)
    std::string reset_timer(unsigned int time);

    //GETTER FUNCTIONS

    //ritorna il valore di work_period
    unsigned int get_work_period(void) const;
};

#endif //PROGRAMMEDCYCLE_H