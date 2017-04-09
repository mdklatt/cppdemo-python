""" Test suite for the cppext library.

The script can be executed on its own or incorporated into a larger test suite.
However the tests are run, be aware of which version of the package is actually
being tested. If the package is installed in site-packages, that version takes
precedence over the version in this project directory. Use a virtualenv test
environment or setuptools develop mode to test against the development version.

"""
import pytest

from cppext import *


def test_pyhello():
    """ Test the pyhello() function.

    """
    assert pyhello() == "Greetings from Python!"
    return


def test_cpphello():
    """ Test the cpphello() function.

    """
    assert cpphello() == "Greetings from C++!"
    return


class CppGreetingTest(object):
    """ Test suite for the CppGreeting class.
    
    """
    def test_hello(self):
        """ Test the hello() method.
        
        """
        name = "CppGreetingTest"
        greeting = CppGreeting(name)
        assert greeting.hello() == "Greetings from C++, {:s}!".format(name)
        return


# Make the module executable.

if __name__ == "__main__":
    raise SystemExit(pytest.main([__file__]))
