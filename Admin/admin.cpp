#include "seal_admin_interface.h"
#include "admin_security.h"
#include "utils.h"

int main(){
    SealWrapperAdmin admin((size_t)32768, 881, SK_fname, PK_fname);
    AdminSecurity adminSec();

    ofstream fhe_file_out, cmd_file_out;
    fhe_file_out.open(fhe_out_fname, ios::binary | std::ios_base::app); // append instead of overwrite
    cmd_file_out.open(cmd_out_fname, ios::binary | std::ios_base::app); // append instead of overwrite

    bool flag = true;

    while (flag) {
        string input;
        cout << "\n\n\tAdmin: Olá António Grilo ( ͡ᵔ ͜ʖ ͡ᵔ )\n\n" << endl;
        cout << "\t¯\\_(ツ)_/¯ ¯\\_(ツ)_/¯ ¯\\_(ツ)_/¯ ¯\\_(ツ)_/¯\n" << endl;
        cout << "\n\tThis is the Options Menu\n"<< endl;
        // cout << "- PRESS 1 to create keys" << endl;
        cout << "\t (⚆ _ ⚆ ) PRESS 1 to create clients" << endl;
        cout << "\t (⚆ _ ⚆ ) PRESS 2 to quit\n\n\n  Choice: ";
        
        getline(cin, input);

        int i = 0;
        std::istringstream(input) >> i;

        switch (i) {
            case 1:
                // create clients
                system("../../admin.sh");
                break;
            case 2:
                flag = false;
                break;
            default:
                cout << "\n\nTenta outra vez ¯\\_(ツ)_/¯\n\n";
                system("clear");

        }
    }

    fhe_file_out.close();
    cmd_file_out.close();

}

