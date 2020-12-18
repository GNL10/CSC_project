#include "seal_admin_interface.h"
#include "utils.h"

int main(){
    SealWrapperAdmin admin (POLY, COEFF);
    bool flag = true;

    while (flag) {
        string input;
        cout << "\n\tThis is the Options Menu\n"<< endl;
        cout << "\t PRESS 1 to create RSA keys" << endl;
        cout << "\t PRESS 2 to create FHE keys" << endl;
        cout << "\t PRESS 3 to quit\n\n\n  Choice: ";

        getline(cin, input);

        int i = 0;
        std::istringstream(input) >> i;

        switch (i) {
            case 1:
                // create clients
                system("../../admin.sh");
                break;
            case 2:
                // Generate FHE keys
                admin.gen_and_upload_keys(SK_fname, PK_fname);
                break;
            case 3:
                flag = false;
                break;
            default:
                cout << "\n\nTenta outra vez ¯\\_(ツ)_/¯\n\n";
                system("clear");

        }
    }
    return 0;
}
