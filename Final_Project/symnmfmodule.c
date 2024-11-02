#define PY_SSIZE_T_CLEAN

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h> 
#include "symnmf.h"

/* Macro to validate if a Python object is a list */
#define VALIDATE_LIST(obj)  \
    do { \
        if (!(obj) || !PyList_Check(obj)) { \
            printf("An Error Has Occurred!\n"); \
            ERROR_MSG(7); \
            exit(1); \
        } \
    } while (0)

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

static PyObject* py_sym(PyObject *self, PyObject *args){
    (void)self;
    double **data_matrix ,**A;
    int n, d;
    PyObject *PyDataPoints;
    PyObject *result_mat;

    if (!PyArg_ParseTuple(args, "Oii", &PyDataPoints, &n, &d)) {
        return NULL;
    }
    VALIDATE_LIST(PyDataPoints);
    data_matrix = create_matrix(n, d);
    PyObj_To_cMatrix(PyDataPoints, data_matrix, n, d);
    A = sym(data_matrix, n, d);
    result_mat = cMatrix_to_PyObject(A, n, n);

    free_matrix(A, n), free_matrix(data_matrix, n);
    return result_mat;
}

static PyObject* py_ddg(PyObject *self, PyObject *args){
    (void)self;
    double **data_matrix ,**D, **A;
    int n, d;
    PyObject *PyDataPoints;
    PyObject *result_mat;

    if (!PyArg_ParseTuple(args, "Oii", &PyDataPoints, &n, &d)) {
        return NULL;
    }
    VALIDATE_LIST(PyDataPoints);
    data_matrix = create_matrix(n, d);
    PyObj_To_cMatrix(PyDataPoints, data_matrix, n, d);
    A = sym(data_matrix, n, d);
    D = ddg(A ,n);
    result_mat = cMatrix_to_PyObject(D, n, n);

    free_matrix(A, n), free_matrix(data_matrix, n), free_matrix(D, n);
    return result_mat;
}

static PyObject* py_norm(PyObject *self, PyObject *args){
    (void)self;
    double **data_matrix ,**D, **A, **W;
    int n, d;
    PyObject *PyDataPoints;
    PyObject *result_mat;

    if (!PyArg_ParseTuple(args, "Oii", &PyDataPoints, &n, &d)) {
        return NULL;
    }
    VALIDATE_LIST(PyDataPoints);
    data_matrix = create_matrix(n, d);
    PyObj_To_cMatrix(PyDataPoints, data_matrix, n, d);
    A = sym(data_matrix, n, d);
    D = ddg(A ,n);
    W = norm(D, A, n);
    result_mat = cMatrix_to_PyObject(W, n, n);

    free_matrix(A, n), free_matrix(data_matrix, n), free_matrix(D, n), free_matrix(W, n);
    return result_mat;
}

static PyObject* py_symnmf(PyObject *self, PyObject *args){
    (void)self;
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
    CHECK_MATRIX(H);

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
