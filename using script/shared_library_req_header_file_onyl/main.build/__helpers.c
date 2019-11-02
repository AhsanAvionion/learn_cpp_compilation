// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 1 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 1; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 1 * sizeof(PyObject *) );
            memcpy( python_pars + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 1 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 1 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 1; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 1 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 1 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (1 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (1 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 1 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 1, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 1 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 1 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 2 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 2; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 2 * sizeof(PyObject *) );
            memcpy( python_pars + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 2 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 2 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 2; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 2 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 2 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 2 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 2, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 2 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 2 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 3; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 3 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 3; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 3 * sizeof(PyObject *) );
            memcpy( python_pars + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 3 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 3 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 3; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 3 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 3 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 3 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 3, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 3 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 3 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 4; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 4 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 4; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 4 * sizeof(PyObject *) );
            memcpy( python_pars + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 4 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 4 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 4; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 4 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 4 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (4 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (4 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 4 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 4, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 4 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 4 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS5( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 5; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 5 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 5; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 5 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 5 * sizeof(PyObject *) );
            memcpy( python_pars + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 5 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 5 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 5; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 5 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 5 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 5 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 5 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (5 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 5 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (5 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 5 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 5, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 5 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            5
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 5 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
MOD_INIT_DECL(__main__);
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] =
{
    { "__main__", MOD_INIT_NAME( __main__ ), 0, 0,  },
    { "_bsddb", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_cn", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_hk", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_iso2022", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_jp", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_kr", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_tw", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_csv", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_curses", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_curses_panel", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_hotshot", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_json", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_lsprof", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multibytecodec", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_tkinter", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "dbm", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "mmap", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "os", NULL, 437, 25575, NUITKA_BYTECODE_FLAG },
    { "parser", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "readline", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "resource", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "termios", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "types", NULL, 26012, 2703, NUITKA_BYTECODE_FLAG },
    { "BaseHTTPServer", NULL, 28715, 21674, NUITKA_BYTECODE_FLAG },
    { "Bastion", NULL, 50389, 6622, NUITKA_BYTECODE_FLAG },
    { "CGIHTTPServer", NULL, 57011, 10984, NUITKA_BYTECODE_FLAG },
    { "Canvas", NULL, 67995, 15396, NUITKA_BYTECODE_FLAG },
    { "ConfigParser", NULL, 83391, 25087, NUITKA_BYTECODE_FLAG },
    { "Cookie", NULL, 108478, 22574, NUITKA_BYTECODE_FLAG },
    { "Dialog", NULL, 131052, 1902, NUITKA_BYTECODE_FLAG },
    { "DocXMLRPCServer", NULL, 132954, 9791, NUITKA_BYTECODE_FLAG },
    { "FileDialog", NULL, 142745, 9666, NUITKA_BYTECODE_FLAG },
    { "FixTk", NULL, 152411, 2078, NUITKA_BYTECODE_FLAG },
    { "HTMLParser", NULL, 154489, 13655, NUITKA_BYTECODE_FLAG },
    { "MimeWriter", NULL, 168144, 7338, NUITKA_BYTECODE_FLAG },
    { "Queue", NULL, 175482, 9360, NUITKA_BYTECODE_FLAG },
    { "ScrolledText", NULL, 184842, 2646, NUITKA_BYTECODE_FLAG },
    { "SimpleDialog", NULL, 187488, 4309, NUITKA_BYTECODE_FLAG },
    { "SimpleHTTPServer", NULL, 191797, 7980, NUITKA_BYTECODE_FLAG },
    { "SimpleXMLRPCServer", NULL, 199777, 22783, NUITKA_BYTECODE_FLAG },
    { "SocketServer", NULL, 222560, 23948, NUITKA_BYTECODE_FLAG },
    { "StringIO", NULL, 246508, 11434, NUITKA_BYTECODE_FLAG },
    { "Tix", NULL, 257942, 95416, NUITKA_BYTECODE_FLAG },
    { "Tkconstants", NULL, 353358, 2236, NUITKA_BYTECODE_FLAG },
    { "Tkdnd", NULL, 355594, 12765, NUITKA_BYTECODE_FLAG },
    { "Tkinter", NULL, 368359, 199207, NUITKA_BYTECODE_FLAG },
    { "UserDict", NULL, 567566, 9613, NUITKA_BYTECODE_FLAG },
    { "UserList", NULL, 577179, 6501, NUITKA_BYTECODE_FLAG },
    { "UserString", NULL, 583680, 14720, NUITKA_BYTECODE_FLAG },
    { "_LWPCookieJar", NULL, 598400, 5414, NUITKA_BYTECODE_FLAG },
    { "_MozillaCookieJar", NULL, 603814, 4445, NUITKA_BYTECODE_FLAG },
    { "__future__", NULL, 608259, 4203, NUITKA_BYTECODE_FLAG },
    { "_abcoll", NULL, 612462, 25466, NUITKA_BYTECODE_FLAG },
    { "_osx_support", NULL, 637928, 11712, NUITKA_BYTECODE_FLAG },
    { "_pyio", NULL, 649640, 64343, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 713983, 15124, NUITKA_BYTECODE_FLAG },
    { "_sysconfigdata", NULL, 729107, 271, NUITKA_BYTECODE_FLAG },
    { "_sysconfigdata_nd", NULL, 729378, 20828, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 750206, 6347, NUITKA_BYTECODE_FLAG },
    { "_weakrefset", NULL, 756553, 9574, NUITKA_BYTECODE_FLAG },
    { "abc", NULL, 766127, 6113, NUITKA_BYTECODE_FLAG },
    { "aifc", NULL, 772240, 30305, NUITKA_BYTECODE_FLAG },
    { "anydbm", NULL, 802545, 2786, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 805331, 64049, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 869380, 12882, NUITKA_BYTECODE_FLAG },
    { "asynchat", NULL, 882262, 8742, NUITKA_BYTECODE_FLAG },
    { "asyncore", NULL, 891004, 18767, NUITKA_BYTECODE_FLAG },
    { "atexit", NULL, 909771, 2183, NUITKA_BYTECODE_FLAG },
    { "audiodev", NULL, 911954, 8407, NUITKA_BYTECODE_FLAG },
    { "bdb", NULL, 920361, 18971, NUITKA_BYTECODE_FLAG },
    { "binhex", NULL, 939332, 15350, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 954682, 3053, NUITKA_BYTECODE_FLAG },
    { "bsddb", NULL, 957735, 12368, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "bsddb.db", NULL, 970103, 582, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbobj", NULL, 970685, 18601, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbrecio", NULL, 989286, 5269, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbshelve", NULL, 994555, 12910, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbtables", NULL, 1007465, 24402, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbutils", NULL, 1031867, 1615, NUITKA_BYTECODE_FLAG },
    { "cProfile", NULL, 1033482, 6263, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 1039745, 27747, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 1067492, 32417, NUITKA_BYTECODE_FLAG },
    { "cgitb", NULL, 1099909, 12094, NUITKA_BYTECODE_FLAG },
    { "chunk", NULL, 1112003, 5572, NUITKA_BYTECODE_FLAG },
    { "cmd", NULL, 1117575, 13989, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 1131564, 10294, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 1141858, 6569, NUITKA_BYTECODE_FLAG },
    { "collections", NULL, 1148427, 26051, NUITKA_BYTECODE_FLAG },
    { "colorsys", NULL, 1174478, 3967, NUITKA_BYTECODE_FLAG },
    { "commands", NULL, 1178445, 2449, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 1180894, 6997, NUITKA_BYTECODE_FLAG },
    { "compiler", NULL, 1187891, 1287, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "compiler.ast", NULL, 1189178, 71339, NUITKA_BYTECODE_FLAG },
    { "compiler.consts", NULL, 1260517, 727, NUITKA_BYTECODE_FLAG },
    { "compiler.future", NULL, 1261244, 2928, NUITKA_BYTECODE_FLAG },
    { "compiler.misc", NULL, 1264172, 3687, NUITKA_BYTECODE_FLAG },
    { "compiler.pyassem", NULL, 1267859, 25783, NUITKA_BYTECODE_FLAG },
    { "compiler.pycodegen", NULL, 1293642, 56161, NUITKA_BYTECODE_FLAG },
    { "compiler.symbols", NULL, 1349803, 17557, NUITKA_BYTECODE_FLAG },
    { "compiler.syntax", NULL, 1367360, 1862, NUITKA_BYTECODE_FLAG },
    { "compiler.transformer", NULL, 1369222, 47387, NUITKA_BYTECODE_FLAG },
    { "compiler.visitor", NULL, 1416609, 4159, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 1420768, 4422, NUITKA_BYTECODE_FLAG },
    { "cookielib", NULL, 1425190, 54366, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 1479556, 12110, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 1491666, 13441, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 1505107, 20224, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._endian", NULL, 1525331, 2287, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 1527618, 8518, NUITKA_BYTECODE_FLAG },
    { "curses", NULL, 1536136, 1539, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "curses.ascii", NULL, 1537675, 5085, NUITKA_BYTECODE_FLAG },
    { "curses.has_key", NULL, 1542760, 5928, NUITKA_BYTECODE_FLAG },
    { "curses.panel", NULL, 1548688, 267, NUITKA_BYTECODE_FLAG },
    { "curses.textpad", NULL, 1548955, 7121, NUITKA_BYTECODE_FLAG },
    { "curses.wrapper", NULL, 1556076, 1206, NUITKA_BYTECODE_FLAG },
    { "dbhash", NULL, 1557282, 706, NUITKA_BYTECODE_FLAG },
    { "decimal", NULL, 1557988, 171637, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 1729625, 61782, NUITKA_BYTECODE_FLAG },
    { "dircache", NULL, 1791407, 1560, NUITKA_BYTECODE_FLAG },
    { "dis", NULL, 1792967, 6204, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 1799171, 405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.archive_util", NULL, 1799576, 7383, NUITKA_BYTECODE_FLAG },
    { "distutils.bcppcompiler", NULL, 1806959, 7856, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 1814815, 36746, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 1851561, 16722, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 1868283, 655, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 1868938, 5149, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 1874087, 4963, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_msi", NULL, 1879050, 23917, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 1902967, 17580, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 1920547, 10878, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 1931425, 5115, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 1936540, 6319, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 1942859, 19252, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 1962111, 11443, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 1973554, 4455, NUITKA_BYTECODE_FLAG },
    { "distutils.command.check", NULL, 1978009, 6192, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 1984201, 3051, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 1987252, 12637, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 1999889, 17884, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 2017773, 3096, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_egg_info", NULL, 2020869, 4345, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 2025214, 2233, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 2027447, 6649, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 2034096, 2906, NUITKA_BYTECODE_FLAG },
    { "distutils.command.register", NULL, 2037002, 10174, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 2047176, 16652, NUITKA_BYTECODE_FLAG },
    { "distutils.command.upload", NULL, 2063828, 6288, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 2070116, 3543, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 2073659, 7520, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 2081179, 9793, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 2090972, 244, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 2091216, 3164, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 2094380, 6764, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 2101144, 39034, NUITKA_BYTECODE_FLAG },
    { "distutils.emxccompiler", NULL, 2140178, 7441, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 2147619, 6237, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 2153856, 7396, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 2161252, 11908, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 2173160, 6732, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 2179892, 10714, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 2190606, 2754, NUITKA_BYTECODE_FLAG },
    { "distutils.msvc9compiler", NULL, 2193360, 21427, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 2214787, 17465, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 2232252, 6405, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 2238657, 15079, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 2253736, 9229, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 2262965, 8087, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 2271052, 14304, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 2285356, 7170, NUITKA_BYTECODE_FLAG },
    { "distutils.versionpredicate", NULL, 2292526, 5520, NUITKA_BYTECODE_FLAG },
    { "doctest", NULL, 2298046, 83440, NUITKA_BYTECODE_FLAG },
    { "dumbdbm", NULL, 2381486, 6700, NUITKA_BYTECODE_FLAG },
    { "dummy_thread", NULL, 2388186, 5356, NUITKA_BYTECODE_FLAG },
    { "dummy_threading", NULL, 2393542, 1275, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 2394817, 2852, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._parseaddr", NULL, 2397669, 13763, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 2411432, 5305, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 2416737, 13499, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 2430236, 2210, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 2432446, 3491, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 2435937, 11516, NUITKA_BYTECODE_FLAG },
    { "email.generator", NULL, 2447453, 10334, NUITKA_BYTECODE_FLAG },
    { "email.header", NULL, 2457787, 13622, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 2471409, 2348, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 2473757, 28576, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 2502333, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.mime.application", NULL, 2502453, 1570, NUITKA_BYTECODE_FLAG },
    { "email.mime.audio", NULL, 2504023, 2893, NUITKA_BYTECODE_FLAG },
    { "email.mime.base", NULL, 2506916, 1102, NUITKA_BYTECODE_FLAG },
    { "email.mime.image", NULL, 2508018, 2035, NUITKA_BYTECODE_FLAG },
    { "email.mime.message", NULL, 2510053, 1434, NUITKA_BYTECODE_FLAG },
    { "email.mime.multipart", NULL, 2511487, 1655, NUITKA_BYTECODE_FLAG },
    { "email.mime.nonmultipart", NULL, 2513142, 874, NUITKA_BYTECODE_FLAG },
    { "email.mime.text", NULL, 2514016, 1294, NUITKA_BYTECODE_FLAG },
    { "email.parser", NULL, 2515310, 3804, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 2519114, 8816, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 2527930, 9284, NUITKA_BYTECODE_FLAG },
    { "encodings.mbcs", NULL, 2537214, 2019, NUITKA_BYTECODE_FLAG },
    { "filecmp", NULL, 2539233, 9574, NUITKA_BYTECODE_FLAG },
    { "fileinput", NULL, 2548807, 14432, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 2563239, 3594, NUITKA_BYTECODE_FLAG },
    { "formatter", NULL, 2566833, 19016, NUITKA_BYTECODE_FLAG },
    { "fpformat", NULL, 2585849, 4679, NUITKA_BYTECODE_FLAG },
    { "fractions", NULL, 2590528, 19625, NUITKA_BYTECODE_FLAG },
    { "ftplib", NULL, 2610153, 34609, NUITKA_BYTECODE_FLAG },
    { "genericpath", NULL, 2644762, 3487, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 2648249, 6626, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 2654875, 4722, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 2659597, 17993, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 2677590, 2919, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 2680509, 15168, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 2695677, 6883, NUITKA_BYTECODE_FLAG },
    { "heapq", NULL, 2702560, 14520, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 2717080, 4514, NUITKA_BYTECODE_FLAG },
    { "hotshot", NULL, 2721594, 3454, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "hotshot.log", NULL, 2725048, 5507, NUITKA_BYTECODE_FLAG },
    { "hotshot.stats", NULL, 2730555, 3374, NUITKA_BYTECODE_FLAG },
    { "hotshot.stones", NULL, 2733929, 1144, NUITKA_BYTECODE_FLAG },
    { "htmlentitydefs", NULL, 2735073, 6357, NUITKA_BYTECODE_FLAG },
    { "htmllib", NULL, 2741430, 20119, NUITKA_BYTECODE_FLAG },
    { "httplib", NULL, 2761549, 36779, NUITKA_BYTECODE_FLAG },
    { "ihooks", NULL, 2798328, 21226, NUITKA_BYTECODE_FLAG },
    { "imaplib", NULL, 2819554, 45161, NUITKA_BYTECODE_FLAG },
    { "imghdr", NULL, 2864715, 4798, NUITKA_BYTECODE_FLAG },
    { "importlib", NULL, 2869513, 1488, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "imputil", NULL, 2871001, 15539, NUITKA_BYTECODE_FLAG },
    { "inspect", NULL, 2886540, 40083, NUITKA_BYTECODE_FLAG },
    { "io", NULL, 2926623, 3571, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 2930194, 13908, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 2944102, 11928, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 2956030, 13679, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 2969709, 2215, NUITKA_BYTECODE_FLAG },
    { "json.tool", NULL, 2971924, 1282, NUITKA_BYTECODE_FLAG },
    { "keyword", NULL, 2973206, 2093, NUITKA_BYTECODE_FLAG },
    { "lib2to3", NULL, 2975299, 117, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.btm_matcher", NULL, 2975416, 5800, NUITKA_BYTECODE_FLAG },
    { "lib2to3.btm_utils", NULL, 2981216, 7529, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_base", NULL, 2988745, 7146, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_util", NULL, 2995891, 14607, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes", NULL, 3010498, 123, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.fixes.fix_apply", NULL, 3010621, 2070, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_asserts", NULL, 3012691, 1547, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_basestring", NULL, 3014238, 793, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_buffer", NULL, 3015031, 950, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_dict", NULL, 3015981, 3753, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_except", NULL, 3019734, 2993, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exec", NULL, 3022727, 1418, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_execfile", NULL, 3024145, 2059, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exitfunc", NULL, 3026204, 2739, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_filter", NULL, 3028943, 2256, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_funcattrs", NULL, 3031199, 1114, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_future", NULL, 3032313, 919, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_getcwdu", NULL, 3033232, 926, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_has_key", NULL, 3034158, 3184, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_idioms", NULL, 3037342, 4515, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_import", NULL, 3041857, 3233, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports", NULL, 3045090, 5352, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports2", NULL, 3050442, 622, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_input", NULL, 3051064, 1134, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_intern", NULL, 3052198, 1783, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_isinstance", NULL, 3053981, 1838, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools", NULL, 3055819, 1791, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools_imports", NULL, 3057610, 2016, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_long", NULL, 3059626, 841, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_map", NULL, 3060467, 3040, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_metaclass", NULL, 3063507, 6579, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_methodattrs", NULL, 3070086, 1138, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ne", NULL, 3071224, 985, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_next", NULL, 3072209, 3531, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_nonzero", NULL, 3075740, 1086, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_numliterals", NULL, 3076826, 1249, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_operator", NULL, 3078075, 5112, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_paren", NULL, 3083187, 1543, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_print", NULL, 3084730, 2727, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raise", NULL, 3087457, 2498, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raw_input", NULL, 3089955, 936, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reduce", NULL, 3090891, 1262, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_renames", NULL, 3092153, 2449, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_repr", NULL, 3094602, 1016, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_set_literal", NULL, 3095618, 1988, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_standarderror", NULL, 3097606, 853, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_sys_exc", NULL, 3098459, 1705, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_throw", NULL, 3100164, 1996, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_tuple_params", NULL, 3102160, 5430, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_types", NULL, 3107590, 2196, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_unicode", NULL, 3109786, 1716, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_urllib", NULL, 3111502, 7134, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ws_comma", NULL, 3118636, 1382, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xrange", NULL, 3120018, 3063, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xreadlines", NULL, 3123081, 1152, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_zip", NULL, 3124233, 1346, NUITKA_BYTECODE_FLAG },
    { "lib2to3.main", NULL, 3125579, 9803, NUITKA_BYTECODE_FLAG },
    { "lib2to3.patcomp", NULL, 3135382, 6569, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2", NULL, 3141951, 164, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.pgen2.conv", NULL, 3142115, 8169, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.driver", NULL, 3150284, 6434, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.grammar", NULL, 3156718, 7654, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.literals", NULL, 3164372, 2002, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.parse", NULL, 3166374, 7204, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.pgen", NULL, 3173578, 12166, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.token", NULL, 3185744, 2287, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.tokenize", NULL, 3188031, 16923, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pygram", NULL, 3204954, 1427, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pytree", NULL, 3206381, 30143, NUITKA_BYTECODE_FLAG },
    { "lib2to3.refactor", NULL, 3236524, 23866, NUITKA_BYTECODE_FLAG },
    { "linecache", NULL, 3260390, 3252, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 3263642, 57332, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 3320974, 25834, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 3346808, 39488, NUITKA_BYTECODE_FLAG },
    { "macpath", NULL, 3386296, 7635, NUITKA_BYTECODE_FLAG },
    { "macurl2path", NULL, 3393931, 2228, NUITKA_BYTECODE_FLAG },
    { "mailbox", NULL, 3396159, 76271, NUITKA_BYTECODE_FLAG },
    { "mailcap", NULL, 3472430, 7045, NUITKA_BYTECODE_FLAG },
    { "markupbase", NULL, 3479475, 9225, NUITKA_BYTECODE_FLAG },
    { "md5", NULL, 3488700, 368, NUITKA_BYTECODE_FLAG },
    { "mhlib", NULL, 3489068, 33613, NUITKA_BYTECODE_FLAG },
    { "mimetools", NULL, 3522681, 8153, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 3530834, 18425, NUITKA_BYTECODE_FLAG },
    { "mimify", NULL, 3549259, 11963, NUITKA_BYTECODE_FLAG },
    { "modulefinder", NULL, 3561222, 19049, NUITKA_BYTECODE_FLAG },
    { "multifile", NULL, 3580271, 5382, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 3585653, 8405, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.connection", NULL, 3594058, 14310, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.dummy", NULL, 3608368, 5412, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.dummy.connection", NULL, 3613780, 2732, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.forking", NULL, 3616512, 14400, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.heap", NULL, 3630912, 6861, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.managers", NULL, 3637773, 38147, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.pool", NULL, 3675920, 22468, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.process", NULL, 3698388, 9544, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.queues", NULL, 3707932, 11421, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.reduction", NULL, 3719353, 5972, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.sharedctypes", NULL, 3725325, 8572, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.synchronize", NULL, 3733897, 10939, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 3744836, 10081, NUITKA_BYTECODE_FLAG },
    { "mutex", NULL, 3754917, 2494, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 3757411, 4688, NUITKA_BYTECODE_FLAG },
    { "new", NULL, 3762099, 852, NUITKA_BYTECODE_FLAG },
    { "nntplib", NULL, 3762951, 20948, NUITKA_BYTECODE_FLAG },
    { "ntpath", NULL, 3783899, 13077, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 3796976, 1801, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 3798777, 13880, NUITKA_BYTECODE_FLAG },
    { "opcode", NULL, 3812657, 6127, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 3818784, 53606, NUITKA_BYTECODE_FLAG },
    { "os", NULL, 437, 25575, NUITKA_BYTECODE_FLAG },
    { "os2emxpath", NULL, 3872390, 4499, NUITKA_BYTECODE_FLAG },
    { "pdb", NULL, 3876889, 43433, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 3920322, 38344, NUITKA_BYTECODE_FLAG },
    { "pickletools", NULL, 3958666, 56960, NUITKA_BYTECODE_FLAG },
    { "pipes", NULL, 4015626, 9268, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 4024894, 18885, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 4043779, 38489, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 4082268, 19115, NUITKA_BYTECODE_FLAG },
    { "popen2", NULL, 4101383, 8985, NUITKA_BYTECODE_FLAG },
    { "poplib", NULL, 4110368, 13271, NUITKA_BYTECODE_FLAG },
    { "posixfile", NULL, 4123639, 7620, NUITKA_BYTECODE_FLAG },
    { "posixpath", NULL, 4131259, 11377, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 4142636, 10148, NUITKA_BYTECODE_FLAG },
    { "profile", NULL, 4152784, 16370, NUITKA_BYTECODE_FLAG },
    { "pstats", NULL, 4169154, 24885, NUITKA_BYTECODE_FLAG },
    { "pty", NULL, 4194039, 4938, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 4198977, 6603, NUITKA_BYTECODE_FLAG },
    { "pyclbr", NULL, 4205580, 9617, NUITKA_BYTECODE_FLAG },
    { "pydoc", NULL, 4215197, 92384, NUITKA_BYTECODE_FLAG },
    { "pydoc_data", NULL, 4307581, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "pydoc_data.topics", NULL, 4307701, 420808, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 4728509, 25610, NUITKA_BYTECODE_FLAG },
    { "repr", NULL, 4754119, 5343, NUITKA_BYTECODE_FLAG },
    { "rexec", NULL, 4759462, 23681, NUITKA_BYTECODE_FLAG },
    { "rfc822", NULL, 4783143, 31685, NUITKA_BYTECODE_FLAG },
    { "rlcompleter", NULL, 4814828, 6054, NUITKA_BYTECODE_FLAG },
    { "robotparser", NULL, 4820882, 7947, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 4828829, 8755, NUITKA_BYTECODE_FLAG },
    { "sched", NULL, 4837584, 4968, NUITKA_BYTECODE_FLAG },
    { "sets", NULL, 4842552, 16775, NUITKA_BYTECODE_FLAG },
    { "sgmllib", NULL, 4859327, 15334, NUITKA_BYTECODE_FLAG },
    { "sha", NULL, 4874661, 411, NUITKA_BYTECODE_FLAG },
    { "shelve", NULL, 4875072, 10194, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 4885266, 7524, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 4892790, 18835, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 4911625, 16582, NUITKA_BYTECODE_FLAG },
    { "sitecustomize", NULL, 4928207, 224, NUITKA_BYTECODE_FLAG },
    { "smtpd", NULL, 4928431, 15813, NUITKA_BYTECODE_FLAG },
    { "smtplib", NULL, 4944244, 30186, NUITKA_BYTECODE_FLAG },
    { "sndhdr", NULL, 4974430, 7319, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 4981749, 16084, NUITKA_BYTECODE_FLAG },
    { "sqlite3", NULL, 4997833, 154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "sqlite3.dbapi2", NULL, 4997987, 2682, NUITKA_BYTECODE_FLAG },
    { "sqlite3.dump", NULL, 5000669, 2057, NUITKA_BYTECODE_FLAG },
    { "sre", NULL, 5002726, 509, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 5003235, 32402, NUITKA_BYTECODE_FLAG },
    { "stat", NULL, 5035637, 2723, NUITKA_BYTECODE_FLAG },
    { "statvfs", NULL, 5038360, 610, NUITKA_BYTECODE_FLAG },
    { "stringold", NULL, 5038970, 12487, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 5051457, 32282, NUITKA_BYTECODE_FLAG },
    { "sunau", NULL, 5083739, 18290, NUITKA_BYTECODE_FLAG },
    { "sunaudio", NULL, 5102029, 1969, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 5103998, 3014, NUITKA_BYTECODE_FLAG },
    { "symtable", NULL, 5107012, 11678, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 5118690, 18805, NUITKA_BYTECODE_FLAG },
    { "tabnanny", NULL, 5137495, 8199, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 5145694, 75786, NUITKA_BYTECODE_FLAG },
    { "telnetlib", NULL, 5221480, 23080, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 5244560, 20228, NUITKA_BYTECODE_FLAG },
    { "test", NULL, 5264788, 114, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "test.pystone", NULL, 5264902, 7981, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 5272883, 11996, NUITKA_BYTECODE_FLAG },
    { "this", NULL, 5284879, 1210, NUITKA_BYTECODE_FLAG },
    { "threading", NULL, 5286089, 42422, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 5328511, 12149, NUITKA_BYTECODE_FLAG },
    { "tkColorChooser", NULL, 5340660, 1402, NUITKA_BYTECODE_FLAG },
    { "tkCommonDialog", NULL, 5342062, 1496, NUITKA_BYTECODE_FLAG },
    { "tkFileDialog", NULL, 5343558, 5121, NUITKA_BYTECODE_FLAG },
    { "tkFont", NULL, 5348679, 7124, NUITKA_BYTECODE_FLAG },
    { "tkMessageBox", NULL, 5355803, 3864, NUITKA_BYTECODE_FLAG },
    { "tkSimpleDialog", NULL, 5359667, 9053, NUITKA_BYTECODE_FLAG },
    { "toaiff", NULL, 5368720, 3090, NUITKA_BYTECODE_FLAG },
    { "token", NULL, 5371810, 3798, NUITKA_BYTECODE_FLAG },
    { "tokenize", NULL, 5375608, 14588, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 5390196, 22716, NUITKA_BYTECODE_FLAG },
    { "traceback", NULL, 5412912, 11631, NUITKA_BYTECODE_FLAG },
    { "ttk", NULL, 5424543, 62344, NUITKA_BYTECODE_FLAG },
    { "tty", NULL, 5486887, 1303, NUITKA_BYTECODE_FLAG },
    { "turtle", NULL, 5488190, 139105, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 5627295, 2954, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 5630249, 40207, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 5670456, 11295, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 5681751, 7970, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 5689721, 7869, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 5697590, 7581, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 5705171, 2753, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 5707924, 10460, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 5718384, 4494, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 5722878, 50958, NUITKA_BYTECODE_FLAG },
    { "urllib2", NULL, 5773836, 47019, NUITKA_BYTECODE_FLAG },
    { "urlparse", NULL, 5820855, 15038, NUITKA_BYTECODE_FLAG },
    { "user", NULL, 5835893, 1714, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 5837607, 4294, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 5841901, 23095, NUITKA_BYTECODE_FLAG },
    { "warnings", NULL, 5864996, 13456, NUITKA_BYTECODE_FLAG },
    { "wave", NULL, 5878452, 19903, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 5898355, 16327, NUITKA_BYTECODE_FLAG },
    { "webbrowser", NULL, 5914682, 19652, NUITKA_BYTECODE_FLAG },
    { "whichdb", NULL, 5934334, 2229, NUITKA_BYTECODE_FLAG },
    { "wsgiref", NULL, 5936563, 719, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wsgiref.handlers", NULL, 5937282, 16172, NUITKA_BYTECODE_FLAG },
    { "wsgiref.headers", NULL, 5953454, 7429, NUITKA_BYTECODE_FLAG },
    { "wsgiref.simple_server", NULL, 5960883, 6199, NUITKA_BYTECODE_FLAG },
    { "wsgiref.util", NULL, 5967082, 5961, NUITKA_BYTECODE_FLAG },
    { "wsgiref.validate", NULL, 5973043, 16767, NUITKA_BYTECODE_FLAG },
    { "xdrlib", NULL, 5989810, 9810, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 5999620, 1068, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom", NULL, 6000688, 6427, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom.NodeFilter", NULL, 6007115, 1112, NUITKA_BYTECODE_FLAG },
    { "xml.dom.domreg", NULL, 6008227, 3293, NUITKA_BYTECODE_FLAG },
    { "xml.dom.expatbuilder", NULL, 6011520, 32810, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minicompat", NULL, 6044330, 3394, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minidom", NULL, 6047724, 65141, NUITKA_BYTECODE_FLAG },
    { "xml.dom.pulldom", NULL, 6112865, 12986, NUITKA_BYTECODE_FLAG },
    { "xml.dom.xmlbuilder", NULL, 6125851, 15222, NUITKA_BYTECODE_FLAG },
    { "xml.etree", NULL, 6141073, 119, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementInclude", NULL, 6141192, 1951, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementPath", NULL, 6143143, 7560, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 6150703, 34966, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 6185669, 166, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 6185835, 304, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 6186139, 277, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 6186416, 3679, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 6190095, 6127, NUITKA_BYTECODE_FLAG },
    { "xml.sax.expatreader", NULL, 6196222, 15081, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 6211303, 12970, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 6224273, 14685, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 6238958, 19078, NUITKA_BYTECODE_FLAG },
    { "xmllib", NULL, 6258036, 26732, NUITKA_BYTECODE_FLAG },
    { "xmlrpclib", NULL, 6284768, 43824, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 6328592, 42007, NUITKA_BYTECODE_FLAG },
    { NULL, NULL, 0, 0, 0 }
};

void setupMetaPathBasedLoader( void )
{
    static bool init_done = false;

    if ( init_done == false )
    {
        registerMetaPathBasedUnfreezer( meta_path_loader_entries );
        init_done = true;
    }
}
