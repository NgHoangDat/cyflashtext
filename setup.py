import sys
from glob import glob

from setuptools import setup

from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir

__version__ = "0.0.1"


ext_modules = [
    Pybind11Extension(
        "python_example", glob("src/**/*.cpp"),
        define_macros = [('VERSION_INFO', __version__)]
    ),
]

setup(
    name="cyflashtext",
    version=__version__,
    author="nghoangdat",
    author_email="18.hoang.dat.12@gmail.com"
)