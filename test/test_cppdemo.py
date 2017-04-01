""" Test suite for the cppdemo library.

The script can be executed on its own or incorporated into a larger test suite.
However the tests are run, be aware of which version of the package is actually
being tested. If the package is installed in site-packages, that version takes
precedence over the version in this project directory. Use a virtualenv test
environment or setuptools develop mode to test against the development version.

"""
import pytest

from cppdemo import *


def test_pyello():
    """ Test the pyhello() function.

    """
    assert pyhello() == "Python says, 'Hello.'"
    return


# Make the module executable.

if __name__ == "__main__":
    raise SystemExit(pytest.main([__file__]))
