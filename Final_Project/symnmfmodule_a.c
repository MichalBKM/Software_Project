#define PY_SSIZE_T_CLEAN
#include "symnmf.c" //check if symnmf.h should be put here instead
#include <Python.h>
#include <string.h>

#define VALIDATE_LIST(obj) \
    if (!PyList_Check(obj)) { \
        PyErr_SetString(PyExc_TypeError, "Argument must be a valid matrix."); \
        return NULL; \
    }


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

static PyObject* fit(PyObject *self, PyObject *args){
    PyObject* PyDataPoints;
    double** datapoints;
    double** output_mat_c;
    int n, d, k;
    char *goal;

    if (!PyArg_ParseTuple(args, "Osiii", &PyDataPoints, &goal, &n, &k, &d)){
        return NULL;
    }

    VALIDATE_LIST(PyDataPoints);

    datapoints = create_matrix(n,d);
    convert_PyObj_To_cMatrix(PyDataPoints, datapoints, n, d);

    output_mat_c = calc_mat(datapoints, goal, n, d, k);

    if (output_mat_c == NULL){
        PyErr_SetString(PyExc_RuntimeError, "An Error Has Occurred");
        return NULL;
    }

    PyObject* output_mat_py = cMatrix_to_PyObject(output_mat_c, n, n);

    free_matrix(datapoints, n);
    free_matrix(output_mat_c, n);
    return output_mat_py; 
}

double **calc_mat(double **data_matrix, char *goal, int n, int d, int k){
    if (strcmp(goal, "sym") == 0){
        return sym(data_matrix, n,d);
    }

    else if (strcmp(goal, "ddg") == 0){
        return ddg(data_matrix, n);
    }

    else if(strcmp(goal, "norm") == 0){
        return norm(data_matrix, n);
    }
    else {
        return NULL;
    }
}

static PyMethodDef symNMF_Methods[] = {
    {"fit", fit, METH_VARARGS, "Calculate the similarity matrix."},
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
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m) {
        return NULL;
    }
    return m;
}