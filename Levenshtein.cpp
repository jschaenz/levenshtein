#include <iostream>
#include <string>
#include <assert.h>
using namespace std;


class Levenshtein {

public:

    int minCosts;
    int sameChar;


    /* Calculates the number of operations, to convert a string A to a string B
    returns: number of operations required*/
    int calculate(string originalName, string referenceName) {
        assert(originalName.length() > 0);
        assert(referenceName.length() > 0);

        int matrix[originalName.length() + 1][referenceName.length() + 1];

        for (int i = 0; i <= originalName.length(); i++) {
            matrix[i][0] = i;
        }
        for (int i = 0; i <= referenceName.length(); i++) {
            matrix[0][i] = i;
        }

        for (int row = 1; row <= originalName.length(); row++) {
            for (int column = 1; column <= referenceName.length(); column++) {
                sameChar = 0;

                if (originalName.at(row - 1) != referenceName.at(column - 1)) {
                    sameChar = 1;
                }
                minCosts = matrix[row - 1][column] + 1;

                if (matrix[row][column - 1] + 1 < minCosts) {
                    minCosts = matrix[row][column - 1] + 1;
                }

                if (matrix[row - 1][column - 1] + sameChar < minCosts) {
                    minCosts = matrix[row - 1][column - 1] + sameChar;
                }
                matrix[row][column] = minCosts;
            }
        }
        assert(matrix[originalName.length()][referenceName.length()] != NULL);

        return matrix[originalName.length()][referenceName.length()];
    }

};