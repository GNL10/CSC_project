#include "seal_client_interface.h"
#include "api.h"
#include "file_watcher.h"
#include "client_parse_cmd.h"

#define NUM_THREADS 1
struct notifications {
    string status;
    string file;
};

void *WatcherRoutine(void *notifications){

    struct watcher_thread_data *args;
    args = (struct watcher_thread_data *) notifications;
    
    FileWatcher fw{"./", chrono::milliseconds(2000)};

    fw.start([] (string path_to_watch, FileStatus status) -> void {
        // Process only regular files, all other file types are ignored
        if(!fs::is_regular_file(fs::path(path_to_watch)) && status != FileStatus::erased) {
            return ;
        }
        
        switch(status) {
            case FileStatus::created:
                std::cout << "File created: " << path_to_watch << '\n';
                
                break;
            case FileStatus::modified:
                std::cout << "File modified: " << path_to_watch << '\n';

                break;
            case FileStatus::erased:
                std::cout << "File erased: " << path_to_watch << '\n';

                break;
            default:
                std::cout << "Error! Unknown file status.\n";
        }
    });
}

int main () {
    SealWrapperClient sealClient(POLY, COEFF);
    ClientParseCmd parser;
    Api api;
    
    if(DEBUG) cout << "[DEBUG] Files open: " << api.check_all_is_open() << endl;

    pthread_t threads[NUM_THREADS];
    int t_id;

    if(DEBUG) cout << "[DEBUG] Creating threads" << endl;

    try{
        struct notifications wt;
        t_id = pthread_create(&threads[0], NULL, WatcherRoutine, (void *)&wt);
        if (t_id) {
            cout << "Error:unable to create thread," << t_id << endl;
        }
    }
    catch(std::exception const& e){
        cout << "[ERROR] There was an error creating threads :: " << e.what() << endl;
    }
    
    bool flag = true;
    //cout << "current file system " << std::filesystem::current_path() << endl;
    while (flag) {
        string input;
        cout << "\nThis is the Options Menu:\n" << endl;
        cout << "- PRESS 1 to send a command;" << endl;
        cout << "- PRESS 2 to read responses;" << endl;
        cout << "- PRESS 3 to quit." << endl;

        getline(cin, input);

        int i = 0;
        std::istringstream(input) >> i;

        switch (i) {
            case 1:
                parser.send_command(&sealClient, api.cmd_out, api.fhe_out);
                break;
            case 2:
                parser.read_command(&sealClient, api.cmd_in, api.fhe_in);
                break;
            case 3:
                flag = false;
                break;
            default:
                cout << "ERROR: Input value not valid!";
        }
    }

    pthread_exit(NULL);
}

