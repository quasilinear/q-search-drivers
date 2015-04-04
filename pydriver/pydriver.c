
#include <qio.h>
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>

static qio_callbacks_t* callbacks;

static PyObject* py_get_pc(PyObject* self) {
  uint64_t pc = callbacks->get_pc();
  return Py_BuildValue("L", pc);
}

static PyObject* py_get_xpr(PyObject* self, PyObject* args) {
  uint8_t which;
  PyArg_ParseTuple(args, "b", &which);
  uint64_t val = callbacks->get_xpr(which);  
  return Py_BuildValue("L", val);
}

static PyObject* py_get_fpr(PyObject* self, PyObject* args) {
  uint8_t which;
  PyArg_ParseTuple(args, "b", &which);
  uint64_t val = callbacks->get_fpr(which);  
  return Py_BuildValue("L", val);
}

static PyObject* py_get_mem(PyObject* self, PyObject* args) {
  uint64_t addr;
  PyArg_ParseTuple(args, "L", &addr);
  uint64_t val = callbacks->get_mem(addr);  
  return Py_BuildValue("L", val);
}

static PyObject* py_get_insn(PyObject* self, PyObject* args) {
  uint64_t addr;
  PyArg_ParseTuple(args, "L", &addr);
  uint32_t insn = callbacks->get_insn(addr);  
  return Py_BuildValue("I", insn);
}

static PyMethodDef qio_methods[] = {
  {"qio_get_pc", (PyCFunction)py_get_pc, METH_NOARGS, NULL},
  {"qio_get_xpr", (PyCFunction)py_get_xpr, 1, NULL},
  {"qio_get_fpr", (PyCFunction)py_get_fpr, 1, NULL},
  {"qio_get_mem", (PyCFunction)py_get_mem, 1, NULL},
  {"qio_get_insn", (PyCFunction)py_get_insn, 1, NULL},
  {NULL, NULL, 0, NULL}
};

static PyObject* pymodule;

void qio_init(qio_callbacks_t* _callbacks) {

  callbacks = _callbacks;

  char* pymodule_name = getenv("Q_PYMODULE");
  if (pymodule_name == NULL) {
    fprintf(stderr, "Q_PYMODULE was not defined\n");
    exit(-1);
  }

  Py_Initialize();

  Py_InitModule("qio_callbacks", qio_methods);
  PyRun_SimpleString("from qio_callbacks import *\n");

  pymodule = PyImport_ImportModule(pymodule_name);
  if (pymodule == NULL) {
    fprintf(stderr, "failed to import '%s' (is PYTHONPATH set appropriately?)\n", pymodule_name);
    exit(-1);
  }

  PyObject* pInit = PyObject_GetAttrString(pymodule, "qio_init");
  Py_XDECREF(pInit);  
  if (pInit && PyCallable_Check(pInit)) {
    PyObject* pRes = PyObject_CallObject(pInit, NULL);
    Py_XDECREF(pRes);
  }
  Py_XDECREF(pInit);  

}

uint64_t qio_exec(uint8_t funct, uint64_t xs1, uint64_t xs2) {

  // TODO: need to make sure we exit safely when python executes sys.exit(-1)

  uint64_t res = 0;
  PyObject* pExec = PyObject_GetAttrString(pymodule, "qio_exec");
  if (pExec && PyCallable_Check(pExec)) {
    PyObject* pRes = PyObject_CallFunction(pExec, "bLL", funct, xs1, xs2);
    res = PyInt_AsUnsignedLongLongMask(pRes);
    Py_DECREF(pRes);
  }
  Py_XDECREF(pExec);

  return res;
}
