#define PY_SSIZE_T_CLEAN
#include "symnmf.c"
#include <Python.h>
#include "symnmf.h" //header file, with C function declarations

static void convert_PyObj_To_cMatrix(PyObject* pyMat, double **cMat, int rows, int columns)
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

// Wrapper for 'sym' function
static PyObject* py_sym(PyObject *self, PyObject *args){
    PyObject* PyDataPoints;
    double** datapoints;
    int n, d;
    int i,j;

    if (!PyArg_ParseTuple(args, "Oii", &PyDataPoints, &n, &d)){
        return NULL;
    }

    if (!PyList_Check(PyDataPoints)) {
        PyErr_SetString(PyExc_TypeError, "Both arguments must be matrices.");
        return NULL;
    }

    datapoints = create_matrix(n,d);
    convert_PyObj_To_cMatrix(PyDataPoints, datapoints, n, d);

    double** sym_matrix_c = sym(datapoints,n,d);

    PyObject* sym_matrix_py = PyList_New(n);
    for (i=0; i<n; i++){
        PyObject* single_row = PyList_New(n);
        for (j=0; j<n; j++){
            PyList_SET_ITEM(single_row, j, PyFloat_FromDouble(sym_matrix_c[i][j]));
        }
        PyList_SET_ITEM(sym_matrix_py,i,single_row);
    }

    free_matrix(datapoints, n);
    free_matrix(sym_matrix_c, n);
    return sym_matrix_py; 
}


// Do the same for ddg, norm

static PyMethodDef symNMF_Methods[] = {
    {"sym", py_sym, METH_VARARGS, "Calculate the similarity matrix."},
    {"ddg", py_ddg, METH_VARARGS, "Calculate the diagonal degree matrix."},
    {"norm", py_norm, METH_VARARGS, "Calculate the normalized similarity matrix."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    symNMF_Methods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_symnmf(void) {
    return PyModule_Create(&symnmfmodule);
}