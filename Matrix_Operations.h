#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Base Functions
float** createMatrix(int rowSize, int columnSize);
float** createIdentityMatrix(int size);
float** createTranspose(float** matrix, int rowSize, int columnSize);
float findDeterminant(float** matrix, int size);
float** createAdjoint(float** matrix, int size);
float** createInverse(float** matrix, int size);
void deleteMatrix(float** matrix, int rowSize);
void copyValues(float** matrix, float** copyMatrix, int rowSize, int columnSize);
void assignValues(float** matrix, int rowSize, int columnSize, const float* values);
void viewMatrix(float** matrix, int rowSize, int columnSize);

// Matrix Arithmetic
void scalarProduct(float** matrix, int rowSize, int columnSize, float scalar);
float** dotProduct(float** matrix1, int rowSize1, int columnSize1, float** matrix2, int columnSize2);

// Elementary Row Operations
void swapRows(float** matrix, int columnSize, int row1, int row2);
void multiplyRow(float** matrix, int columnSize, int row, float coefficient);
void multiplyAndAddRow(float** matrix, int columnSize, int changedRow, int usedRow, float coefficient);

// Service Functions
int getLeadingIndexOfRow(float** matrix, int columnSize, int row);
void sortRowsToLeadingIndices(float** matrix, int rowSize, int columnSize);
bool isColumnREF(float** matrix, int rowSize, int column, int startRow);
void eliminateRowLeadingValue(float** matrix, int columnSize, int eliminatedRow, int pivotRow);
void formatValue(float** matrix, int row, int column);
float** getMinor(float** matrix, int size, int row, int column);

// Higher Functions
void reduceToREF(float** matrix, int rowSize, int columnSize);
void reduceToRREF(float** matrix, int rowSize, int columnSize);
bool solveLinearSystem(float** augmentedMatrix, int rowSize, int columnSize, float* variables);

#endif //MATRIX_OPERATIONS_H
