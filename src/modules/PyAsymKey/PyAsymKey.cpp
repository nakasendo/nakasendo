#include <Python.h>
#include <string>
#include <iostream>
#include <AsymKey/AsymKeyAPI.h>


struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject* wrap_GenerateKeyPair(PyObject* self, PyObject *args)
{
    const std::pair<std::string, std::string> keyPair = GenerateKeyPair();
    return Py_BuildValue("ss",keyPair.first.c_str(),keyPair.second.c_str());
}

static PyMethodDef ModuleMethods[] =
{
    // {"test_get_data_nulls", wrap_test_get_data_nulls, METH_NOARGS, "Get a string of fixed length with embedded nulls"},
    {"GenerateKeyPair",wrap_GenerateKeyPair,METH_VARARGS,"Generate pair of keys"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int PyAsymKey_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int PyAsymKey_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "PyAsymKey",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        PyAsymKey_traverse,
        PyAsymKey_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_PyAsymKey(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("PyAsymKeyExtension", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("PyAsymKeyExtension.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
