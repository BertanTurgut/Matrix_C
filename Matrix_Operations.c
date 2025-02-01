#include "Matrix_Operations.h"

// IMPORTANT: tune these values accordingly
float ZERO_THRESHOLD = 0.00001;
int VIEWING_DECIMAL = 2;

float** createMatrix(const int rowSize, const int columnSize) {
    float** matrix = malloc(rowSize * sizeof(float*));
    for (int i = 0; i < rowSize; i++) {
        matrix[i] = malloc(columnSize * sizeof(float));
        for (int j = 0; j < columnSize; j++) {
            matrix[i][j] = (float) 0;
        }
    }
    return matrix;
}

float** createIdentityMatrix(int size) {
    float** matrix = malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        matrix[i] = malloc(size * sizeof(float));
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (i == j) ? (float) 1 : (float) 0;
        }
    }
    return matrix;
}

float** createTranspose(float** matrix, const int rowSize, const int columnSize) {
    float** transpose = createMatrix(columnSize, rowSize);
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < columnSize; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
    return transpose;
}

float findDeterminant(float** matrix, int size) {
    if (size == 1)
        return matrix[0][0];

    float determinant = 0;
    for (int i = 0; i < size; i++) {
        float** minor = getMinor(matrix, size, 0, i);
        float minorDeterminant = findDeterminant(minor, size-1);
        int coefficient = (i % 2 == 0) ? 1 : -1;
        determinant += coefficient * minorDeterminant * matrix[0][i];
        deleteMatrix(minor, size-1);
    }
    return determinant;
}

float** createAdjoint(float** matrix, const int size) {
    float** adjoint = createMatrix(size, size);
    float** minor;
    float minorDeterminant = 0, cofactor = 0;
    int coefficient = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            minor = getMinor(matrix, size, i, j);
            minorDeterminant = findDeterminant(minor, size-1);
            coefficient = (i+j) % 2 == 0 ? 1 : -1;
            cofactor = coefficient * minorDeterminant;
            adjoint[j][i] = cofactor;
            deleteMatrix(minor, size-1);
        }
    }
    return adjoint;
}

float** createInverse(float** matrix, const int size) {
    // IMPORTANT: input matrix must be non-singular
    float** adjoint = createAdjoint(matrix, size);
    float scalar = 1/findDeterminant(matrix, size);
    scalarProduct(adjoint, size, size, scalar);
    return adjoint;
}

void deleteMatrix(float** matrix, const int rowSize) {
    for (int i = 0; i < rowSize; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void copyValues(float** matrix, float** copyMatrix, int rowSize, int columnSize) {
    // IMPORTANT: matrix sizes must be identical
    for (int i = 0; i < rowSize; i++)
        for (int j = 0; j < columnSize; j++)
            copyMatrix[i][j] = matrix[i][j];
}

void assignValues(float** matrix, const int rowSize, const int columnSize, const float* values) {
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < columnSize; j++) {
            matrix[i][j] = values[i*columnSize + j];
            formatValue(matrix, i, j);
        }
    }
}

void viewMatrix(float** matrix, const int rowSize, const int columnSize) {
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < columnSize; j++) {
            char format[20];
            sprintf(format, "%%.%df\t", VIEWING_DECIMAL);
            printf(format, matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void scalarProduct(float** matrix, const int rowSize, const int columnSize, const float scalar) {
    for (int i = 0; i < rowSize; i++) {
        multiplyRow(matrix, columnSize, i, scalar);
    }
}

float** dotProduct(float** matrix1, const int rowSize1, const int columnSize1, float** matrix2, const int columnSize2) {
    // IMPORTANT: columnSize1 is equal to the rowSize2
    float** result = createMatrix(rowSize1, columnSize2);
    for (int i = 0; i < rowSize1; i++) {
        for (int j = 0; j < columnSize2; j++) {
            for (int k = 0; k < columnSize1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}



void swapRows(float** matrix, const int columnSize, const int row1, const int row2) {
    float temp[columnSize];
    for (int i = 0; i < columnSize; i++) {
        temp[i] = matrix[row1][i];
    }
    for (int i = 0; i < columnSize; i++) {
        matrix[row1][i] = matrix[row2][i];
        matrix[row2][i] = temp[i];
    }
}

void multiplyRow(float** matrix, const int columnSize, const int row, const float coefficient) {
    for (int i = 0; i < columnSize; i++) {
        if (matrix[row][i] == (float) 0)
            continue;
        matrix[row][i] *= coefficient;
        formatValue(matrix, row, i);
    }
}

void multiplyAndAddRow(float** matrix, const int columnSize, const int changedRow, const int usedRow, const float coefficient) {
    for (int i = 0; i < columnSize; i++) {
        if (matrix[usedRow][i] != (float) 0) {
            matrix[changedRow][i] += matrix[usedRow][i] * coefficient;
            formatValue(matrix, changedRow, i);
        }
    }
}


int getLeadingIndexOfRow(float** matrix, const int columnSize, const int row) {
    // IMPORTANT: the row is a 0-row, -1 is returned
    for (int i = 0; i < columnSize; i++) {
        formatValue(matrix, row, i);
        if (matrix[row][i] != (float) 0) {
            return i;
        }
    }
    return -1;
}

void sortRowsToLeadingIndices(float** matrix, const int rowSize, const int columnSize) {
    int counter;
    do {
        counter = 0;
        for (int i = 0; i < rowSize-1; i++) {
            int index1 = getLeadingIndexOfRow(matrix, columnSize, i);
            int index2 = getLeadingIndexOfRow(matrix, columnSize, i+1);
            if (index1 == -1 && index2 == -1) {}
            else if ((index1 == -1) || (index1 > index2 && index2 != -1)) {
                swapRows(matrix, columnSize, i, i + 1);
                counter++;
            }
        }
    } while (counter > 0);
}

bool isColumnREF(float** matrix, const int rowSize, const int column, const int startRow) {
    for (int i = startRow+1; i < rowSize; i++) {
        if (matrix[i][column] != (float) 0)
            return false;
    }
    return true;
}

void eliminateRowLeadingValue(float** matrix, const int columnSize, const int targetRow, const int pivotRow) {
    // IMPORTANT: target row and the pivot row must have the same index for their leading values
    int leadingIndex = getLeadingIndexOfRow(matrix, columnSize, pivotRow);
    float coefficient = -1 * matrix[targetRow][leadingIndex] / matrix[pivotRow][leadingIndex];
    multiplyAndAddRow(matrix, columnSize, targetRow, pivotRow, coefficient);
}

void formatValue(float** matrix, const int row, const int column) {
    if (fabsf(matrix[row][column]) < ZERO_THRESHOLD)
        matrix[row][column] = (float) 0;
}

float** getMinor(float** matrix, int size, int row, int column) {
    float** minor = createMatrix(size-1, size-1);
    int k = 0;
    for (int i = 0; i < size; i++) {
        if (i == row) continue;
        int l = 0;
        for (int j = 0; j < size; j++) {
            if (j == column) continue;
            minor[k][l] = matrix[i][j];
            l++;
        }
        k++;
    }
    return minor;
}


void reduceToREF(float** matrix, const int rowSize, const int columnSize) {
    int currentRow = 0, currentColumn = 0;
    while (currentRow < rowSize) {
        // find the currentColumn
        sortRowsToLeadingIndices(matrix, rowSize, columnSize);
        for (int i = 0; i < columnSize; i++) {
            if (matrix[currentRow][i] != (float) 0) {
                currentColumn = i;
                break;
            }
        }

        // eliminate all the rows below the pivot
        for (int i = currentRow+1; i < rowSize; i++) {
            if (getLeadingIndexOfRow(matrix, columnSize, i) > currentColumn)
                break;
            eliminateRowLeadingValue(matrix, columnSize, i, currentRow);
        }

        // change current row
        currentRow++;
    }
}

void reduceToRREF(float** matrix, int rowSize, int columnSize) {
    int currentRow = 0, currentColumn = 0;
    while (currentRow < rowSize && getLeadingIndexOfRow(matrix, columnSize, currentRow) != -1) {
        // find the currentColumn
        sortRowsToLeadingIndices(matrix, rowSize, columnSize);
        for (int i = 0; i < columnSize; i++) {
            if (matrix[currentRow][i] != (float) 0) {
                currentColumn = i;
                break;
            }
        }

        // make the pivot row's leading value 1
        float coefficient = 1/matrix[currentRow][currentColumn];
        multiplyRow(matrix, columnSize, currentRow, coefficient);

        // eliminate all the rows below and above the pivot
        for (int i = 0; i < rowSize; i++) {
            int leadingIndex = getLeadingIndexOfRow(matrix, columnSize, i);
            if (leadingIndex > currentColumn || leadingIndex == -1)
                break;
            if (i == currentRow)
                continue;
            eliminateRowLeadingValue(matrix, columnSize, i, currentRow);
        }

        // change current row
        currentRow++;
    }
}

bool solveLinearSystem(float** augmentedMatrix, const int rowSize, const int columnSize, float* variables) {
    // IMPORTANT: coefficient matrix must be a square matrix and variables list must have size rowSize
    // extract result column vector and the coefficient matrix
    float** resultVector = createMatrix(rowSize, 1);
    float** coefficientMatrix = createMatrix(rowSize, columnSize-1);
    for (int i = 0; i < rowSize; i++) {
        resultVector[i][0] = augmentedMatrix[i][columnSize-1];
        for (int j = 0; j < columnSize; j++) {
            if (j < columnSize-1) {
                coefficientMatrix[i][j] = augmentedMatrix[i][j];
            }
        }
    }

    // check the singularity of the coefficient matrix
    if (fabsf(findDeterminant(coefficientMatrix, rowSize)) < ZERO_THRESHOLD) {
        deleteMatrix(resultVector, rowSize);
        deleteMatrix(coefficientMatrix, rowSize);

        return false;
    }

    // find the inverse of the coefficient matrix and multiply it by the result matrix to the left
    float** inverseCoefficientMatrix = createInverse(coefficientMatrix, rowSize);
    float** variablesMatrix = dotProduct(inverseCoefficientMatrix, rowSize, rowSize, resultVector, 1);
    for (int i = 0; i < rowSize; i++) {
        variables[i] = variablesMatrix[i][0];
    }

    deleteMatrix(resultVector, rowSize);
    deleteMatrix(coefficientMatrix, rowSize);
    deleteMatrix(inverseCoefficientMatrix, rowSize);
    deleteMatrix(variablesMatrix, rowSize);
    return true;
}