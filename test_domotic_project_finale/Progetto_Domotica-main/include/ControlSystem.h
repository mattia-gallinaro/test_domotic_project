//autore Mattia Gallinaro

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
        //accetta un double per indicare il contributo dell'impianto e un'altro double per indicare la contribuzione della rete elettrica
        ControlSystem(const double& value_impianto, const double& rete = 3.5) : impianto("Impianto fotovoltaico", value_impianto){
            rete_elettrica = rete;
            read_devices_from_file();
        };

        //prende il comando dato da riga di terminale dall'utente, lo tokenizza e poi esegue la funzione relativa al comando se esiste
        //ritorna l'output della funzione relativa al comando si voleva chiamare
        std::string execute_command(const std::string& input);
        
        //controlla se si ha raggiunto la fine della giornata 
        //1439 = 1439 min = 23:59
        bool dayFinished(){
            return house_time >= 1439;
        };

        //Salva il contenuto della stringa all'interno del file log.txt
        void save_log(const std::string& event);
    private:

        //serve per gestire timer dei dispositivi e anche per sapere quando dover spegnere o accendere un dispositivo
        struct Action{
            
            int time_action;//indica a che ora avviene una specifica azione
            
            std::string name;//serve per indicare il nome del dispositivo su cui eseguire l'operazione di accensione o spegnimento
           
            bool to_activate;//indica se il dispositivo a cui si riferisce deve essere acceso o spento ,
                            // true = accendi il dispositivo accendere , false = spegni il dispostivo
            
            bool destoyable_action;//permette di sapere il timer che viene creato , è un timer da togliere dopo l'uso o meno
                                   //esempio nel caso dell'accensione di un dispositivo a ciclo prefissato , viene creato un timer
                                   //per indicare quando spegnere quel dispositivo e , una volta che il dispositivo viene spento,
                                   //allora il timer viene eliminato 

            Action(std::string nam, int time_act, bool act, bool destroyable = false) {
                name = nam;
                time_action = time_act;
                to_activate = act;
                destoyable_action = destroyable;
            };
        };

        /*
            Permette di spegnere i dispositivi in ordine inverso all'accensione
            per raggiungere l'equilibrio della contribuzione energetica istantanea
            Ritorna l'output dei dispositivi che vengono spenti
        */
        std::string turn_off_devices(void);

        /*
            Sposta l'ultimo dispositivo acceso in testa al vettore devices 
        */
        void order_devices(const int& pos);

        /*
            Legge i dispositivi presenti in dispositivi.txt e inserisce dei puntatori all'interno di devices
        */
        void read_devices_from_file(void);

        /*
            Dato un nome in input, controlla se esiste un dispositivo con quel nome e se lo trova allora lo accende
            Ritorna una stringa in cui viene indicato se il dispositivo è stato acceso o meno
        */
        std::string set_device_on(const std::string& name);

        /*
            Dato un nome in input, controlla se esiste un dispositivo con quel nome e se lo trova allora lo spegne
            Ritorna una stringa in cui viene indicato se il dispositivo è stato spento o meno
        */
        std::string set_device_off(const std::string& name);

        /*
            Permette di andare ad una specifica ora del giorno ed esegue le azioni presenti in action_timestamp 
            comprese tra l'orario di inizio e l'orario di fine
            Ritorna tutti gli eventi di accensione e spegnimento dei dispositivi
        */
        std::string set_time_of_day(const std::string& time_to_reach);

        /*
            Cerca se esiste il dispositivo in base al nome passato e se lo trova , ritorna una stringa
            in cui viene specificato il nome del dispositivo e quanto ha consumato/prodotto(solo per impianto)
        */
        std::string show_device(const std::string& name);

        /*
            Ritorna una stringa in cui viene specificato quanto il sistema ha prodotto e consumato
            e per ogni dispositivo viene indicato il nome e quanto ha consumato
        */
        std::string show_all(void);

        /*
            Resetta il sistema alle condizioni di partenza ed elimina i timer presenti
        */
        void reset_all(void);

        /*
            Riporta l'orario a 00:00 e spegne tutti i dispositivi siccome all'inizio sono spenti,
            ma mantiene i timer dei dispositivi
        */
        void reset_time(void);

        /*
            Rimuove tutti i timer dei dispositivi e mantiene lo stato dei dispositivi attuale
        */
        void reset_timers(void);

        /*
            Rimuove il timer associato al dispositivo
            Ritorna una stringa in cui viene indicato se è stato rimosso il timer ad un dispositivo
        */
        std::string remove_timer(const std::string& name);

        /*
            Cerca il dispositivo in base al nome passato e se lo trova, allora associa un timer a quel dispositivo 
            e lo salva in action_timestamp
            Ritorna una stringa in cui viene indicato se è stato possibile associare un timer al dispositivo indicato
        */
        std::string set_timer_device(const std::string& name , const std::string& start_time, const std::string& end_time = "");
        
        /*
            Converte il nome dato in un tempo e ritorna il tempo in minuti
        */
        int convert_string_time(const std::string& name);

        /*
            ordina i timer in base all'ora in cui devono avvenire in ordine crescente
        */
        void orderActions();

        /*
            Permette di inserire un nuovo timer o di modificarne uno vecchio sostituendo i dati passati alla funzione
        */
        void add_action_to_vector(const std::string& name,const int& time,bool activate, bool destroyable = false);

        //rimuove i timer associati ad un dispositivo
        void remove_action(const std::string& name);

        //rimuove tutti i timer temporanei
        void remove_destroyable_timers(void);

        /*
            Ritorna la posizione in cui quel dispositivo si trova 
            oppure -1 per indicare che un dispositivo con quel nome non esiste
        */
        int find_device_by_name(const std::string& name);

        /*
            Unisce le stringhe dell'array tmp da start_pos a last_pos 
            e poi ritorna una stringa concatenata
        */
        std::string put_together(std::vector<std::string>& tmp,int start_pos, const int& last_pos);
        
        /*
            Permette di suddivere il comando dell'utente e ritorna un vettore che contiene la stringa suddivisa
        */
        std::vector<std::string> divide_input_string(const std::string &input);


        double get_current_contribution();

        int house_time = 0; //indica l'ora del sistema
        double rete_elettrica; //indica quanta corrente si può prendere costantemente 
        Manual impianto; //rappresemta l'impianto fotovoltaico

        std::vector<Device*> devices; //contiene i puntatori ai dispositivi a ciclo prefissato e manuali

        std::vector<std::string> commands ={"set","rm","show","reset"};// sono i possibili comandi che l'utente può usare

        std::vector<Action> action_timestamp; // contiene i timer dei dispositivi
};

#endif
