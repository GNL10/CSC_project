#ifndef API_H
#define API_H

#include "client.h"

class Api{
    public:
        ofstream fhe_out, cmd_out;
        ifstream fhe_in, cmd_in;
        
        Api(){
            fhe_out.open (FHE_OUT_FNAME, ios::binary | ofstream::app);
            cmd_out.open(CMD_OUT_FNAME, ios::binary | ios_base::app);

            fhe_in.open (FHE_IN_FNAME, ios::binary | ofstream::app);
            cmd_in.open(CMD_IN_FNAME, ios::binary | ios_base::app);
        }

        ~Api(){
            fhe_out.close();
            cmd_out.close();

            fhe_in.close();
            cmd_in.close();
        }

        bool check_all_is_open(){
            return (fhe_out.is_open() && cmd_out.is_open() && fhe_in.is_open() && cmd_in.is_open());
        }

};

#endif