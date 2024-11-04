#define PY_SSIZE_T_CLEAN

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h> 
#include "symnmf.h"

/* Macro for an error message if the object is not a Python list */
#define VALIDATE_LIST(obj)  \
    do { \
        if (!(obj) || !PyList_Check(obj)) { \
            fprintf(stderr, "%s\n", ERROR_MESSAGE); \
            exit(1); \
        } \
    } while (0)

/*
 * Converts a Python list of lists (PyObject) into a C matrix (2D array).
 * Parameters:
 *   pyMat: The Python list object to convert.
 *   cMat: The C matrix to populate.
 *   rows, columns: Dimensions of the matrix.
 */
static void PyObj_To_cMatrix(PyObject* pyMat, double **cMat, int rows, int columns)
{
    int i,j;
    PyObject *matRow, *matValue;

    for (i = 0; i < rows ; i++){
        matRow = PyList_GetItem(pyMat, i);
        if (!PyList_Check(matRow)){
            continue;
        }
        for (j = 0; j < columns; j++){
            matValue = PyList_GetItem(matRow, j);
            cMat[i][j] = PyFloat_AsDouble(matValue);
        }
    }
}

/*
 * Converts a C matrix (2D array) to a Python list of lists (PyObject).
 * Parameters:
 *   cMat: The C matrix to convert.
 *   rows, columns: Dimensions of the matrix.
 * Returns: Python list of lists representation of the matrix.
 */
static PyObject* cMatrix_to_PyObject(double** cMat, int rows, int columns){
    int i, j;
    PyObject* PyMat = PyList_New(rows);
    for (i=0; i<rows; i++){
        PyObject* single_row = PyList_New(columns);
        for (j=0; j<columns; j++){
            PyList_SET_ITEM(single_row, j, PyFloat_FromDouble(cMat[i][j]));
        }
        PyList_SET_ITEM(PyMat, i, single_row);
    }
    return PyMat;
}

/*
 * Python wrapper function for calculating the similarity matrix.
 * Parameters: Python list of lists (data points), number of rows (n), and columns (d).
 * Returns: Python list of lists representing the similarity matrix.
 */
static PyObject* py_sym(PyObject *self, PyObject *args){
    double **data_matrix ,**A;
    int n, d;
    PyObject *PyDataPoints;
    PyObject *result_mat;

    if (!PyArg_ParseTuple(args, "O", &PyDataPoints)) {
        return NULL;
    }
    VALIDATE_LIST(PyDataPoints);

    n = PyList_Size(PyDataPoints); 
    d = n > 0 ? PyList_Size(PyList_GetItem(PyDataPoints, 0)) : 0;

    data_matrix = create_matrix(n, d);
    PyObj_To_cMatrix(PyDataPoints, data_matrix, n, d);
    A = sym(data_matrix, n, d);
    result_mat = cMatrix_to_PyObject(A, n, n);

    free_matrix(A, n), free_matrix(data_matrix, n);
    return result_mat;
}

/*
 * Python wrapper function for calculating the diagonal degree matrix.
 * Parameters: Python list of lists (data points), number of rows (n), and columns (d).
 * Returns: Python list of lists representing the diagonal degree matrix.
 */
static PyObject* py_ddg(PyObject *self, PyObject *args){
    double **data_matrix ,**D, **A;
    int n, d;
    PyObject *PyDataPoints;
    PyObject *result_mat;

    if (!PyArg_ParseTuple(args, "O", &PyDataPoints)) {
        return NULL;
    }
    VALIDATE_LIST(PyDataPoints);

    /* Calculate n and d based on PyDataPoints dimensions */
    n = PyList_Size(PyDataPoints);
    d = n > 0 ? PyList_Size(PyList_GetItem(PyDataPoints, 0)) : 0;

    data_matrix = create_matrix(n, d);
    PyObj_To_cMatrix(PyDataPoints, data_matrix, n, d);


    A = sym(data_matrix, n, d);
    D = ddg(A ,n);
    result_mat = cMatrix_to_PyObject(D, n, n);
    free_matrix(A, n), free_matrix(data_matrix, n), free_matrix(D, n);
    return result_mat;
}

/*
 * Python wrapper function for calculating the normalized similarity matrix.
 * Parameters: Python list of lists (data points), number of rows (n), and columns (d).
 * Returns: Python list of lists representing the normalized similarity matrix.
 */
static PyObject* py_norm(PyObject *self, PyObject *args){
    double **data_matrix ,**D, **A, **W;
    int n, d;
    PyObject *PyDataPoints;
    PyObject *result_mat;

    if (!PyArg_ParseTuple(args, "O", &PyDataPoints)) {
        return NULL;
    }
    VALIDATE_LIST(PyDataPoints);

    n = PyList_Size(PyDataPoints); 
    d = n > 0 ? PyList_Size(PyList_GetItem(PyDataPoints, 0)) : 0;

    data_matrix = create_matrix(n, d);
    PyObj_To_cMatrix(PyDataPoints, data_matrix, n, d);

    A = sym(data_matrix, n, d);
    D = ddg(A ,n);
    W = norm(D, A, n);
    result_mat = cMatrix_to_PyObject(W, n, n);

    free_matrix(A, n), free_matrix(data_matrix, n), free_matrix(D, n), free_matrix(W, n);
    return result_mat;
}

/*
 * Python wrapper function for performing symNMF on matrix W.
 * Parameters: Python list of lists for W and H, number of rows (n), and clusters (k).
 * Returns: Python list of lists representing the resulting H matrix.
 */
static PyObject* py_symnmf(PyObject *self, PyObject *args){
    double **W, **H;
    int n, k;
    PyObject *Py_W, *Py_H;
    PyObject *result_mat;

    if (!PyArg_ParseTuple(args, "OOii", &Py_W, &Py_H, &n, &k)) { /*Delete d as an argument */
        return NULL;
    }

    VALIDATE_LIST(Py_W);
    VALIDATE_LIST(Py_H);

    W = create_matrix(n, n);
    H = create_matrix(n, k);

    PyObj_To_cMatrix(Py_W, W, n, n);
    PyObj_To_cMatrix(Py_H, H, n, k);

    H = optimize_H(H, W, n, k);
    result_mat = cMatrix_to_PyObject(H, n, k);
    
    free_matrix(W, n), free_matrix(H, n);
    return result_mat;
}



static PyMethodDef symNMF_Methods[] = {
    {"sym", py_sym, METH_VARARGS, "Calculate the similarity matrix."},
    {"ddg", py_ddg, METH_VARARGS, "Calculate the diagonal degree matrix."},
    {"norm", py_norm, METH_VARARGS, "Calculate the normalized similarity matrix."},
    {"symnmf", py_symnmf, METH_VARARGS, "Perform the full symNMF."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmfmodule",
    NULL, 
    -1,  
    symNMF_Methods
};

PyMODINIT_FUNC PyInit_symnmfmodule(void) {
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m) {
        return NULL;
    }
    return m;
}
