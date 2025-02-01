#include "Matrix_Operations.h"

int main(void) {
    float values[6] = {2, 3, 8, 1, 4, 7};
    float** augmentedMatrix = createMatrix(2, 3);
    assignValues(augmentedMatrix, 2, 3, values);
    float variables[2];
    bool solved = solveLinearSystem(augmentedMatrix, 2, 3, variables);
    printf("%f\t%f", variables[0], variables[1]);
    return 0;
}