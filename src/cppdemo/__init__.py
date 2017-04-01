""" Package for the cppdemo library.

"""
from __future__ import absolute_import

from .__version__ import __version__
from ._cpp import hello as cpphello
from ._py import hello as pyhello
