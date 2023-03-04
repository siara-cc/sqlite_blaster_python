#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "sqlite_blaster/src/util.h"
#include "sqlite_blaster/src/sqlite_index_blaster.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

#define SQIB_UNHANDLED_TYPE_PASSED 1;
#define SQIB_MALFORMED_REC 2;

static const uint8_t int_type_from_len[] = {0, 1, 2, 3, 4, 0, 5, 0, 6};
int fill_col_arr(py::list py_args, void **col_arr, size_t *col_lens, uint8_t *col_types) {
    int col_idx = 0;
    int est_len = 0;
    for (auto item : py_args) {
        if (py::isinstance<py::str>(item)) {
            std::string s = py::cast<std::string>(item);
            col_arr[col_idx] = (void *) malloc(s.length() + 1);
            memcpy(col_arr[col_idx], s.c_str(), s.length());
            col_lens[col_idx] = s.length();
            col_types[col_idx] = 13;
            //col_arr[col_idx][s.length()] = '\0';
            est_len += s.length();
        } else if (py::isinstance<py::int_>(item)) {
            int i = py::cast<int>(item);
            col_arr[col_idx] = (void *) malloc(sizeof(int));
            memcpy(col_arr[col_idx], &i, sizeof(int));
            col_lens[col_idx] = sizeof(int);
            col_types[col_idx] = int_type_from_len[sizeof(int)];
            est_len += sizeof(int);
        } else if (py::isinstance<py::float_>(item)) {
            double d = py::cast<double>(item);
            col_arr[col_idx] = (void *) malloc(sizeof(double));
            memcpy(col_arr[col_idx], &d, sizeof(double));
            col_lens[col_idx] = sizeof(double);
            col_types[col_idx] = 7;
            est_len += sizeof(double);
        }
        col_idx++;
    }
    if (col_idx != py_args.size())
        throw SQIB_UNHANDLED_TYPE_PASSED;
    return est_len;
}

py::list get_values(sqlite_index_blaster& self, uint8_t *rec, int rec_len) {
    py::list result;
    int8_t vlen;
    int col_type_or_len, col_len, col_type;
    int hdr_len = util::read_vint32(rec, &vlen);
    int hdr_pos = vlen;
    uint8_t *data_ptr = rec + hdr_len;
    col_len = vlen = 0;
    do {
        data_ptr += col_len;
        hdr_pos += vlen;
        if (data_ptr - rec > rec_len)
            break;
        if (hdr_pos >= hdr_len)
            break;
        col_type_or_len = util::read_vint32(rec + hdr_pos, &vlen);
        col_len = self.derive_data_len(col_type_or_len);
        col_type = self.derive_col_type(col_type_or_len);
        switch (col_type) {
            case SQLT_TYPE_NULL:
            case SQLT_TYPE_BLOB:
            case SQLT_TYPE_TEXT: {
                    std::string str_val((const char *) data_ptr, col_len);
                    result.append(py::str(str_val));
                }
                break;
            case SQLT_TYPE_INT0:
                result.append(py::int_(0));
                break;
            case SQLT_TYPE_INT1:
                result.append(py::int_(1));
                break;
            case SQLT_TYPE_INT8:
                result.append(py::int_(*data_ptr));
                break;
            case SQLT_TYPE_INT16: {
                    int int_val = util::read_uint16(data_ptr);
                    result.append(py::int_(int_val));
                }
                break;
            case SQLT_TYPE_INT24: {
                    int32_t int_val = util::read_uint24(data_ptr);
                    result.append(py::int_(int_val));
                }
                break;
            case SQLT_TYPE_INT32: {
                    int32_t int_val = util::read_uint32(data_ptr);
                    result.append(py::int_(int_val));
                }
                break;
            case SQLT_TYPE_INT48: {
                    int int_val = util::read_int48(data_ptr);
                    result.append(py::int_(int_val));
                }
                break;
            case SQLT_TYPE_INT64: {
                    int64_t int_val = util::read_uint64(data_ptr);
                    result.append(py::int_(int_val));
                }
                break;
            case SQLT_TYPE_REAL: {
                    double dbl_val = util::read_double(data_ptr);
                    result.append(PyFloat_FromDouble(dbl_val));
                }
                break;
        }
    } while (hdr_pos < hdr_len);
    return result;
}

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
                std::string, std::string,
                int, int,
                const char *>())
        .def("close", &sqlite_index_blaster::close)
        .def("put_string", &sqlite_index_blaster::put_string)
        .def("get_string", &sqlite_index_blaster::get_string)
        .def("put_rec", [](sqlite_index_blaster& self, py::list py_args) {
            int col_count = py_args.size();
            void *col_arr[col_count];
            size_t col_lens[col_count];
            uint8_t col_types[col_count];
            int est_len = fill_col_arr(py_args, col_arr, col_lens, col_types);
            uint8_t rec[est_len + col_count * 3 + 3];
            int rec_len = self.make_new_rec(rec, col_count, (const void **) col_arr, col_lens, col_types);
            return self.put(rec, -rec_len, NULL, 0);
        })
        .def("get_rec", [](sqlite_index_blaster& self, py::list py_args) {
            int col_count = py_args.size();
            void *col_arr[col_count];
            size_t col_lens[col_count];
            uint8_t col_types[col_count];
            int est_len = fill_col_arr(py_args, col_arr, col_lens, col_types);
            uint8_t rec[est_len + col_count * 3 + 3];
            int rec_len = self.make_new_rec(rec, col_count, (const void **) col_arr, col_lens, col_types);
            int out_len;
            py::list result;
            if (self.get(rec, -rec_len, &out_len)) {
                uint8_t *val = (uint8_t *) malloc(out_len);
                self.copy_value(val, &out_len);
                result = get_values(self, val, out_len);
                free(val);
            }
            return result;
        });

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
