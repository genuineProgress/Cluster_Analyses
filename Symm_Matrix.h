#ifndef CLUSTERANALYSES_SYMM_MATRIX_H
#define CLUSTERANALYSES_SYMM_MATRIX_H

#include <vector>
using namespace std;

/* My idea is to keep the matrix like that:
 * . . . .
 * . . .
 * . .
 * .
 */

class Symm_Matrix {
public:
    class Line {
    public:
        explicit Line(vector<double*> vec);
        double &operator[] (int index);
        vector<double *> line;
    };

    Symm_Matrix (int order);
    Line operator[] (int index);
private:
    vector<vector<double>> lines;
};


#endif //CLUSTERANALYSES_SYMM_MATRIX_H
