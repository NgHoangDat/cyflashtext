import sys
from glob import glob
from setuptools import setup, find_packages

from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_cmake_dir

__version__ = "0.0.1"

with open("README.md", "r", encoding='utf-8') as fh:
    long_description = fh.read()

ext_modules = [
    Pybind11Extension(
        "cyvi", glob("lib/**/*.cpp", recursive=True),
        define_macros = [('VERSION_INFO', __version__)]
    ),
]

setup(
    name="cyflashtext",
    packages=find_packages(),    
    version=__version__,
    author="nghoangdat",
    author_email="18.hoang.dat.12@gmail.com",
    url="https://github.com/NgHoangDat/cyflashtext",
    description="Python flashtext with c++ backend",
    long_description=long_description,
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Topic :: Text Processing :: Linguistic"
    ],
    python_requires='>=3.6',
    install_requires=[
        'pydantic'
    ]
)