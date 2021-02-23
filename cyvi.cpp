#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "flashtext/flashtext.h"

namespace py = pybind11;
using Match = flashtext::Match;

PYBIND11_MODULE(cyvi, mod) {
    py::class_<Match>(mod, "Match")
        .def(py::init<int &, int &>())
        .def_readwrite("start", &Match::start)
        .def_readwrite("end", &Match::end)
        .def_readwrite("text", &Match::text);

    mod.def("put", &flashtext::put, "Put keyword");
    mod.def("pop", &flashtext::pop, "Pop keyword");
    mod.def("extract", &flashtext::extract, "Extract keywords");
}
