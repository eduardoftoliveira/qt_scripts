#include <Python.h>

static PyObject* c_grep(PyObject* self, PyObject *args)
{
    FILE *f;
    const char *filename;
    const char *pattern;
    long position;
    char buffer[100];
    char output[100];
    PyObject *lst = PyList_New(0);
    PyObject *num;
    PyObject *line;
    

    
    if (!PyArg_ParseTuple(args, "ss", &filename, &pattern))
        return NULL;
 
    if ((f = fopen(filename, "r")) == NULL) {
        PyErr_SetString(PyExc_IOError,"File not Found");
        return NULL;
    }   
    while(fgets(buffer, sizeof(buffer), f)) {
        if (strstr(buffer, pattern)) {
            position = ftell(f);
            num = PyLong_FromLong(position);
            line = PyUnicode_FromString(buffer);
            PyObject *byte_line_tuple = PyTuple_New(2);
            PyTuple_SET_ITEM(byte_line_tuple,0,num);
            PyTuple_SET_ITEM(byte_line_tuple,1,line);
            PyList_Append(lst,byte_line_tuple);
        }
    }
    return Py_BuildValue("O", lst);
}

static char c_grep_docs[] =
    "c_grep(filename, pattern): return the position of the pattern in the file(in bytes)\n";

static PyMethodDef c_grep_funcs[] = {
    {"c_grep", c_grep, 
     METH_VARARGS, c_grep_docs},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef c_grep_module = {
    PyModuleDef_HEAD_INIT,
    "c_grep",
    c_grep_docs,
    -1,
    c_grep_funcs
};

PyMODINIT_FUNC

PyInit_c_grep(void)
{
    return PyModule_Create(&c_grep_module);
}