======
cppext
======

This is a demonstration project for creating a C++ extension for Python 2.7.


Install the development version of the library in a virtualenv environment:

..  code-block::

    $ virtualenv venv
    $ venv/bin/pip install -r requirements.txt -r test/requirements.txt -e .


Build the C++ extension:

..  code-block::
   
    $ python setup.py build_ext --inplace


Run the test suite:

..  code-block::

    $ venv/bin/pytest test/


Build documentation:

..  code-block::

    $ cd doc
    $ make html
