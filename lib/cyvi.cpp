#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "flashtext/flashtext.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
using Match = flashtext::Match;
using Node = flashtext::Node;

PYBIND11_MODULE(cyvi, mod) {
    py::class_<Match>(mod, "Match")
        .def(py::init<int &, int &>())
        .def_readwrite("start", &Match::start)
        .def_readwrite("end", &Match::end)
        .def_readwrite("text", &Match::text);

    py::class_<Node>(mod, "Node")
        .def(py::init<>())
        .def("put", &Node::put_key)
        .def("pop", &Node::pop_key)
        .def("extract", &Node::extract);

    #ifdef VERSION_INFO
        mod.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
    #else
        mod.attr("__version__") = "dev";
    #endif
}
