/// A sample C++11 extension module for Python.
///
/// This produces a self-contained module that is usable by Python code via a
/// regular `import` statement. A single module cannot contain both Python and
/// C/C++ code, but both types of modules can coexist in the same parent
/// package. If a Python module and an extension module have the same name, the
/// extension module will hide the Python module. For example, if both 'spam.py'
/// 'spam.so' are visible, `import spam` will load 'spam.so'.
///
/// The file name is arbitrary, but it must match the build parameters defined
/// for this module (see setup.py). By convention, the source file for a module
/// named 'spam' would be either 'spammodule.cpp' or 'spam.cpp'.
///
/// The Python header must be included before any standard headers. It includes
/// <cstdio>, <cstring>, <cerrno>, and <cstdlib> for convenience.
///
#ifdef __APPLE__
    #include <Python/Python.h>
#else
    #include <Python.h>
#endif

#include "_cpplib.hpp"
#include <string>


/// Define module-level functions.
///
namespace  // internal linkage
{
    /// Python wrapper for the cpplib::hello() function.
    ///
    /// The two arguments are the parent object (the module instance in the
    /// this case) and a tuple of the positional arguments used to call the
    /// function. The function name is arbitrary but it must match the module's
    /// method table in initcpp().
    ///
    PyObject* hello(PyObject* self, PyObject* args)
    {
        // Validate the arguments and return hello() result as a Python string
        // object. An empty format string is passed to ParseTuple() to indicate
        // that this function expects no arguments.
        if (PyArg_ParseTuple(args, "") == 0) {
            // ParseTuple failed, so the function was called with the wrong
            // arguments. Put a TypeError on the exception stack, and return a
            // null pointer to indicate failure.
            PyErr_SetString(PyExc_TypeError, "hello() takes no arguments");
            return nullptr;
        }
        return Py_BuildValue("s", cpplib::hello().c_str());
    }
}


/// Python wrapper for the cpplib::Greeting class.
///
/// Each Python object maintains an internal pointer to a cpplib::Greeting
/// object that does all of the work. While a C++ class is mapped to a Python
/// class in this case, there is no inherent relationship between Python
/// classes and C++ classes. Python classes can be implemented using any
/// combination of C++ functions and/or class methods.
///
namespace  // internal linkage
{
    /// A Greeting object instance.
    ///
    /// This stores object attributes. The HEAD macro defines the standard
    /// attributes used by all Python objects. The name is arbitrary.
    ///
    struct Greeting
    {
        PyObject_HEAD  // define standard attributes; don't add semicolon

        // External attributes that will be visible within Python. They need
        // reference counting.

        // Internal attributes. These are not part of the Python interface so
        // they do not need reference counting. (Or do they?)
        cpplib::Greeting* cppobj;  // TODO: what about pickling?
    };

     /// Create a new Greeting object.
     ///
     /// Implementation of __new__(). The name is arbitrary but must match the
     /// tp_new field in GreetingType.
     ///
    PyObject* Greeting_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
    {
        // Allocate memory for the type; this must be deallocated in dealloc().
        Greeting* self((Greeting*)type->tp_alloc(type, 0));
        if (self) {
            // Initialize attributes. Can't fully initialize cppobj yet so make
            // it safe for delete for now.
            self->cppobj = nullptr;
        }
        return (PyObject*)self;
    }

    /// Initialize a new Greeting object.
    ///
    /// Implementation of __init__(). The name is arbitrary but must match the
    /// tp_init field in GreetingType. This might called multiple times or not
    /// at all (e.g. during unpickling), and it can be overridden.
    ///
    int Greeting_init(Greeting* self, PyObject* args)
    {
       const char* name;
       if (PyArg_ParseTuple(args, "s", &name) == 0) {
           // ParseTuple failed, so the function was called with the wrong
           // arguments. Put a TypeError on the exception stack, and return an
           // error code.
           PyErr_SetString(PyExc_TypeError, "invalid arguments");
           return -1;  // magic number taken from tutorial
       }
       delete self->cppobj;
       self->cppobj = new cpplib::Greeting(name);
       return 0;  // success; magic number taken from tutorial
    }

     /// Deallocate a Greeting object.
     ///
     /// The name is arbitrary but must match the tp_dealloc field in
     /// GreetingType.
     ///
    void Greeting_dealloc(Greeting* self)
    {
        // Deallocate any attributes and then self.
        delete self->cppobj;
        self->ob_type->tp_free((PyObject*)self);
        return;
    }

    /// Return a greeting.
    ///
    /// This is the implementation of the hello() method. This is not part of
    /// the standard type definition; instead, it must be specified in the
    /// type's method table (see below).
    ///
    PyObject* Greeting_hello(Greeting* self)
    {
        // Delegate to cppobj.
        return Py_BuildValue("s", self->cppobj->hello().c_str());
    }

    static PyMethodDef Greeting_methods[] = {
        // Describe each type method with its Python name, local function,
        // calling convention, and docstring. This must be static.
        {"hello", (PyCFunction)Greeting_hello, METH_NOARGS, "Return a greeting."},
        {nullptr, nullptr, 0, nullptr}  // sentinel
    };

    /// Type definition for the Greeting class.
    ///
    /// All type attributes default to 0 so only the relevant attributes need
    /// to be defined here (including placeholder fields as necessary to
    /// maintain ordering).
    ///
    PyTypeObject GreetingType = {
        PyObject_HEAD_INIT(nullptr)  // don't add semicolon
        0,  // ob_size; legacy field, always set to 0
        "_cpp.Greeting",  // tp_name; used in diagnostic messages
        sizeof(Greeting),  // tp_basicsize
        0,  // itemsize; used by sequences
        (destructor)Greeting_dealloc,  // tp_dealloc
        0,  // tp_print
        0,  // tp_getattr
        0,  // tp_setattr
        0,  // tp_compare
        0,  // tp_repr
        0,  // tp_as_number
        0,  // tp_as_sequence
        0,  // tp_as_mapping
        0,  // tp_hash
        0,  // tp_call
        0,  // tp_str
        0,  // tp_getattro
        0,  // tp_setattro
        0,  // tp_as_buffer
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  // tp_flags
        "A personalized greeting.",  // tp_doc; docstring
        0,  // tp_traverse
        0,  // tp_clear
        0,  // tp_richcompare
        0,  // tp_weaklistoffset
        0,  // tp_iter
        0,  // tp_iternext
        Greeting_methods,  // tp_methods
        0,  // tp_members
        0,  // tp_getset
        0,  // tp_base
        0,  // tp_dict
        0,  // tp_descr_get
        0,  // tp_descr_set
        0,  // tp_dictoffset
        (initproc)Greeting_init, // tp_init
        0,  // tp_alloc
        Greeting_new,  // tp_new
    };
}

/// Initialize the module.
///
/// This must be callable from the Python interpreter so it needs external
/// linkage. The name is not arbitrary and is expected to be `init_cpp` since
/// this is a Python module named '_cpp', irrespective of any parent packages
/// (see setup.py).
///
PyMODINIT_FUNC init_cpp()
{
    static PyMethodDef methods[] = {
        // Describe each module function with its Python name, local function,
        // calling convention, and docstring.
        {"hello", hello, METH_VARARGS, "Return a greeting."},
        {nullptr, nullptr, 0, nullptr}  // sentinel
    };
    if (PyType_Ready(&GreetingType) < 0) {
        return;
    }
    PyObject* module(Py_InitModule3("_cpp", methods, "C++ extension demo."));
    if (!module) {
        return;
    }
    Py_INCREF(&GreetingType);
    PyModule_AddObject(module, "Greeting", (PyObject*)&GreetingType);
    return;
}
