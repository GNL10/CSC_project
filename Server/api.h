#pragma once

#ifndef API_H
#define API_H

#include "server.h"

class Api{
    public:
        ifstream fhe_to_server[CLI_NUM], cmd_to_server[CLI_NUM];
        ofstream fhe_to_client[CLI_NUM], cmd_to_client[CLI_NUM];

        Api(){

            for(int i=0; i < CLI_NUM; i++){
                fhe_to_server[i].open ("client" + to_string(i) + "_" + fhe_out_fname, ios::binary | ofstream::app);
                cmd_to_server[i].open ("client" + to_string(i) + "_" + cmd_out_fname, ios::binary | ofstream::app);
                fhe_to_client[i].open ("../client/client" + to_string(i) + "/" + fhe_in_fname, ios::binary | ofstream::app);
                cmd_to_client[i].open ("../client/client" + to_string(i) + "/" + cmd_in_fname, ios::binary | ofstream::app);
            }
        }

        ~Api(){
            for(int i=0; i < CLI_NUM; i++){
                fhe_to_server[i].close();
                cmd_to_server[i].close();
                fhe_to_client[i].close();
                cmd_to_client[i].close();
            }
        }

        bool check_all_is_open(){
            bool all_open = true;
            for(int i=0; i < CLI_NUM; i++){
                all_open = all_open && fhe_to_server[i].is_open();
                all_open = all_open && cmd_to_server[i].is_open();
                all_open = all_open && fhe_to_client[i].is_open();
                all_open = all_open && cmd_to_client[i].is_open();
            }
            return all_open;
        }

};

#endif
