#ifndef API_H
#define API_H

#include "server.h"

class Api{
    public:
        ofstream fhe_out, cmd_out;
        ifstream fhe_in, cmd_in;

        Api(){
            fhe_out.open ("../server" + string(fhe_out_fname), ios::binary | ofstream::app);
            cmd_out.open("../server" + string(cmd_out_fname), ios::binary | ios_base::app);

            fhe_in.open (fhe_in_fname, ios::binary | ofstream::app);
            cmd_in.open(cmd_in_fname, ios::binary | ios_base::app);
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