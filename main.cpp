#include "Interface.h"
using namespace std;
int main () {
    Controller cc;
    Interface ii (0, true, &cc);
    ifstream s ("run.txt"); //insert your file name in this brackets,
    // file must belong to cmake-build-debug directory
    /*runs the program from the given file
     * run_error - file which contains command errors
     * run - usual file
    */
    ii.run (s);
}
