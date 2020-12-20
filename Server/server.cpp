#include "seal_server_interface.h"
#include "comparator.h"
#include "api.h"
#include "file_watcher.h"
#include "server_parse_cmd.h"

int main(){
    SealWrapperServer sealServer(POLY, COEFF);
    Comparator comparator(sealServer._evaluator, sealServer._relin_key);
    ServerParseCmd parser;
    Api api;

    if(DEBUG) cout << "[DEBUG] Files open: " << api.check_all_is_open() << endl;

    FileWatcher fw{"./", chrono::milliseconds(2000)};

    // TODO: dar check para ver se já exite info para tratar!!!

    fw.start([&sealServer, &parser, &api, &comparator] (string path_to_watch, FileStatus status) -> void {
        // Process only regular files, all other file types are ignored
        if(!fs::is_regular_file(fs::path(path_to_watch)) && status != FileStatus::erased) {
            return ;
        }

        switch(status) {
            case FileStatus::created:

                if(path_to_watch.compare("./" + string(RK_fname)) == 0 ){
                    std::cout << "File created: " << path_to_watch << '\n';
                    if(DEBUG) std::cout << "::RK::" << '\n';
                    sealServer.load_RK_from_file(RK_fname);
                }

                if(path_to_watch.compare("./" + string(cmd_out_fname)) == 0 ){
                    std::cout << "File created: " << path_to_watch << '\n';
                    if(DEBUG) std::cout << "::CMD IN::" << '\n';
                    // parser.read_command(&db, sealServer, &comparator, api.cmd_in, api.fhe_in);
                }

                break;
            case FileStatus::modified:

                if(path_to_watch.compare("./" + string(RK_fname)) == 0 ){
                    std::cout << "File modified: " << path_to_watch << '\n';
                    if(DEBUG) std::cout << "::RK::" << '\n';
                    sealServer.load_RK_from_file(RK_fname);
                }

                if(path_to_watch.compare("./" + string(cmd_out_fname)) == 0 ){
                    std::cout << "File modified: " << path_to_watch << '\n';
                    if(DEBUG) std::cout << "::CMD IN::" << '\n';
                    // parser.read_command(&db, sealServer, &comparator, api.cmd_in, api.fhe_in);
                }

                break;
            case FileStatus::erased:
                std::cout << "File erased: " << path_to_watch << '\n';
                break;
            default:
                std::cout << "Error! Unknown file status.\n";
        }
    });

    return 0;
}
