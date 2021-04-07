#include "Interface.h"
using namespace std;
int main () {
    Controller cc;
    Interface ii (0, true, &cc);
    ifstream s ("run_error.txt");
    /*runs the program from the given file
     * run_error - file which contains command errors
     * run - usual file
    */
    ii.run (s);
}
