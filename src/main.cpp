#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "sqlite_blaster/src/sqlite_index_blaster.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(sqlite_blaster_python, m) {
    m.doc() = R"pbdoc(
        Pybind11 bindings for sqlite_blaster_python
        -----------------------

        .. currentmodule:: sqlite_blaster_python

        .. autosummary::
           :toctree: _generate

           get
           put
    )pbdoc";
    py::class_<sqlite_index_blaster>(m, "sqlite_index_blaster")
        .def(py::init<int, int, 
                vector<string>, const char *,
                int, int,
                const char *>())
        .def("get_string", &sqlite_index_blaster::get_string)
        .def("put_string", &sqlite_index_blaster::put_string)
        .def("close", &sqlite_index_blaster::close);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
