#ifndef UTILS_H
#define UTILS_H

#include "admin.h"
#include <experimental/filesystem>

class Utils{
    
    public:
    
        int* parseInt_toBinary(int value){
            static int a[NUM_MAX_BITS];
            for (int i = 0; value>0; i++){
                a[i] = value%2;
                value = value/2;
            }
            return a;
        }

        int move_file(const char *from, const char *to){
            const auto copyOptions = std::experimental::filesystem::copy_options::update_existing
                           | std::experimental::filesystem::copy_options::recursive;
            std::experimental::filesystem::copy(from, to, copyOptions);
        }

        void send_files_to_client(vector<string> files){
            for(int c_id=0; c_id<CLI_NUM+1; c_id++){
                string client_path = "../client/client";
                client_path.append(to_string(c_id));
                client_path.append("/");
                for (vector<string>::const_iterator i = files.begin(); i != files.end(); ++i){
                    string file_name;
                    file_name = client_path;
                    file_name.append(*i);
                    const char *client_file = file_name.c_str();
                    move_file(i->c_str(), client_file);
                }
            }
        }

        void send_files_to_server(vector<string> files){
            string server_path = "../server/";
            for (vector<string>::const_iterator i = files.begin(); i != files.end(); ++i){
                string file_name;
                file_name = server_path;
                file_name.append(*i);
                const char *server_file = file_name.c_str();
                move_file(i->c_str(), server_file);
            }
        }

};

#endif