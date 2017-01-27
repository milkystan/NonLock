//
//  main.c
//  test
//
//  Created by Landleany on 2017/1/26.
//  Copyright © 2017年 Landleany. All rights reserved.
//

#include <stdio.h>
#include <Python/Python.h>

static PyObject *ReleaseError;


static void del_value(void* p) {
    free(p);
}

static PyObject * new_value(PyObject *self, PyObject *args) {
    int* p = malloc(sizeof(*p));
    *p = 0;
    return PyCObject_FromVoidPtr(p, del_value);
}

static PyObject * acquire(PyObject *self, PyObject *args) {
    PyObject* arg = 0;
    PyArg_ParseTuple(args, "O", &arg);
    int* v = (int*)PyCObject_AsVoidPtr(arg);
    if(__sync_bool_compare_and_swap(v, 0, 1)) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
    
}

static PyObject * release(PyObject *self, PyObject *args) {
    PyObject* arg = 0;
    PyArg_ParseTuple(args, "O", &arg);
    int* v = (int*)PyCObject_AsVoidPtr(arg);
    if(!*v) {
        PyErr_SetString(ReleaseError, "release unlocked lock");
        return NULL;
    }
    *v = 0;
    Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
    {"new_value", new_value, METH_VARARGS},
    {"acquire", acquire, METH_VARARGS},
    {"release", release, METH_VARARGS},
    {NULL, NULL}
};

void initnon_lock(void) {
    PyObject* m = Py_InitModule("non_lock", methods);
    if(m == NULL) return;
    ReleaseError = PyErr_NewException("thread.error", NULL, NULL);
    Py_INCREF(ReleaseError);
    PyModule_AddObject(m, "release_error", ReleaseError);
    
}


int main(void) {
    int* p = malloc(sizeof(*p));
    *p = 1;
    while(!__sync_bool_compare_and_swap(p, 0, 1)) {}
    printf("dfsf");
}

