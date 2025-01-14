//AUTORE: DIEGO MARCHINI

#ifndef DEVICE_H
#define DEVICE_H

#include<iostream>
#include<string>
#include<stdexcept>
class Device{
    protected:
    static int shared_IDs; // variabile che consente di averre un ID diverso per ogni dispositivo
    double energy_contribution; // perchè può anche mettere 0.2 KW
    bool state; // true = acceso , false = spento
    double tot_energy_consumed; // energia consumata/prodotta dal dispositivo
    int starting_time; // espresso in minuti es. 71 = 71 min = 01:11; 1024 = 1024 min = 17:04;
    int end_time;      // espresso in minuti es. 71 = 71 min = 01:11; 1024 = 1024 min = 17:04;
    std::string name; // nome del dispositivo
    unsigned int ID; // ID del dispositivo

    //funzione che aggiunge al totale di energia già prodotta/consumata di un dato dispositivo, l'energia prodotta in un intervallo di funzionamento
    void add_energy_consumed(void);

    public:
    //funzione che spegne il dispositivo richiedendo come parametro il tempo corrente
    std::string setOff(unsigned int time);

    //funzione virtuale pura che accende un dispositivo richiedendo come parametro il tempo corrente
    virtual std::string setOn(unsigned int time) = 0;
    
    //funzione virtuale pura che imposta un timer di accensione e/o spegnimento(opzionale) richiedendo il tempo corrente e gli orari di accesione/spegnimento
    virtual std::string setTimer(unsigned int time, unsigned int start, unsigned int end = 2000) = 0;

    //funzione virtuale pura che rimuove i timer dal dispositivo conservando il suo stato corrrente (acceso/spento)
    virtual std::string reset_timer(unsigned int time) = 0;

    //funzione che resetta lo stato del dispositivo spegnendolo e portando a zero il valore dell'energia da esso consumata/prodotta
    void reset_state(void);

    // GETTER FUNCTIONS

    //funzione che ritorna lo stato (acceso/spento) del dispositivo
    bool get_status(void) const;

    //funzione che ritorna il tempo di accensione del dispositivo
    unsigned int get_start_time(void) const;

    //funzione che ritorna l'apporto energetico del dispositivo
    double get_contribuition(void) const;

    //funzione che ritorna l'energia totale consumata/prodotta dal dispositivo durante la giornata
    double get_energy_consumed(void) const;

    //funzione che ritorna il nome del dispositivo
    std::string get_name(void) const;

    //funzione che ritorna l'ID del dispositivo
    int get_ID(void) const;

    //distruttore
    virtual ~Device();
    
};
// HELPER FUNCTIONS

//funzione usata per convertire un valore di tempo espresso in minuti in un orario nel formato HH:MM
std::string convert_time(unsigned int time);

#endif //DEVICE_H
