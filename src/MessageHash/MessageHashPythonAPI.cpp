#include <Python.h>
#include <string>
#include <iostream>
#include <MessageHashAPI.h>


struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif





static PyObject* wrap_HashMsgSHA256(PyObject* self, PyObject *args)
{
    char * argA; 

    if (!PyArg_ParseTuple(args, "s", &argA))
        return NULL;

    std::string result = HashMsgSHA256 (argA);
    return Py_BuildValue("s",result.c_str());
}

static PyObject* wrap_HashMsg(PyObject* self, PyObject *args)
{
    char * argA; 
    char * argB; 

    if (!PyArg_ParseTuple(args, "ss", &argA, &argB))
        return NULL;

    std::string result = HashMsg(argA,argB);
    return Py_BuildValue("s",result.c_str());
}




static PyMethodDef ModuleMethods[] =
{
    {"HashMsgSHA256",wrap_HashMsgSHA256,METH_VARARGS,"Generate SHA256 hash on a given input"},
    {"HashMsg",wrap_HashMsg,METH_VARARGS,"Generate a HASH on a given input using the hashing algorithim specified"},
    {NULL, NULL, 0, NULL},
};
 
#if PY_MAJOR_VERSION >= 3

static int MessageHashAPI_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int MessageHashAPI_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "MessageHashAPI",
        NULL,
        sizeof(struct module_state),
        ModuleMethods,
        NULL,
        MessageHashAPI_traverse,
        MessageHashAPI_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_MessageHashAPI(void)

#else
#define INITERROR return

void
initmyextension(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("MessageHashAPI", myextension_methods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("MessageHashAPI.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
