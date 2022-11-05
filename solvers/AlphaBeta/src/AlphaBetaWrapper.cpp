/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file AlphaBetaWrapper.cpp
 * \brief Wrapper for the python library
 *
 * This file is used to make the C++ code of AlphaBeta available to Python.
 *
 */

/* C++ Libraries */
/* The following pragma are used to removed depraction warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/stl_iterator.hpp>
#pragma GCC diagnostic pop

/* Other */
#include "AlphaBeta.hpp"

/// @brief Type that allows for registration of conversions from
///        python iterable types.
struct iterable_converter {
  /// @note Registers converter from a python interable type to the
  ///       provided type.
  template <typename Container>
  iterable_converter&
  from_python() {
      boost::python::converter::registry::push_back(
      &iterable_converter::convertible,
      &iterable_converter::construct<Container>,
      boost::python::type_id<Container>());

    // Support chaining.
    return *this;
  }

  /// @brief Check if PyObject is iterable.
  static void* convertible(PyObject* object) {
    return PyObject_GetIter(object) ? object : NULL;
  }

  /// @brief Convert iterable PyObject to C++ container type.
  ///
  /// Container Concept requirements:
  ///
  ///   * Container::value_type is CopyConstructable.
  ///   * Container can be constructed and populated with two iterators.
  ///     I.e. Container(begin, end)
  template <typename Container>
  static void construct(
    PyObject* object,
    boost::python::converter::rvalue_from_python_stage1_data* data) {
        namespace python = boost::python;
        // Object is a borrowed reference, so create a handle indicting it is
        // borrowed for proper reference counting.
        boost::python::handle<> handle(python::borrowed(object));

        // Obtain a handle to the memory block that the converter has allocated
        // for the C++ type.
        typedef python::converter::rvalue_from_python_storage<Container>
                                                            storage_type;
        void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

        typedef python::stl_input_iterator<typename Container::value_type>
                                                            iterator;

        // Allocate the C++ type into the converter's memory block, and assign
        // its handle to the converter's convertible variable.  The C++
        // container is populated by passing the begin and end iterators of
        // the python object to the container's constructor.
        new (storage) Container(
          iterator(python::object(handle)),  // begin
          iterator());                       // end
        data->convertible = storage;
    }
};

struct gridtype_to_list {
    static PyObject* convert(GridType const& grid) {
        boost::python::list *result = new boost::python::list;
        for (std::vector<Color> x : grid) {
            boost::python::list row;
            for (Color value : x)
                row.append(static_cast<int>(value));
            result->append(row);
        }
        return boost::python::incref(result->ptr());
    }
};

struct ListOfPositionType_to_list {
    static PyObject* convert(ListOfPositionType const& move) {
        boost::python::list *result = new boost::python::list;
        for (std::vector<int> position : move) {
            boost::python::list row;
            for (int value : position)
                row.append(value);
            result->append(row);
        }
        return boost::python::incref(result->ptr());
    }
};

struct Result_to_int {
    static PyObject* convert(Result const& value) {
        boost::python::long_ *result = new boost::python::long_;
        *result = boost::python::long_(value);
        return boost::python::incref(result->ptr());
    }
};

struct fromPythonToColor {
    fromPythonToColor() {
        boost::python::converter::registry::push_back(
          &convertible,
          &construct,
          boost::python::type_id<Color>());
    }

    static void* convertible(PyObject* obj_ptr) {
        return obj_ptr;
    }

    static void construct(
    PyObject* obj_ptr,
    boost::python::converter::rvalue_from_python_stage1_data* data) {
        int value_reg = static_cast<int>(PyLong_AsLong(obj_ptr));
        Color value;
        switch (value_reg) {
            case 0:
                value = Empty;
                break;

            case 1:
                value = White;
                break;

            case 2:
                value = Black;
                break;

            default:
                break;
        }
        Color *storage = new Color(value);
        data->convertible = storage;
    }
};



BOOST_PYTHON_MODULE(AlphaBeta) {
    boost::python::to_python_converter<GridType, gridtype_to_list>();
//    boost::python::to_python_converter<Result, Result_to_int>();
    boost::python::to_python_converter<ListOfPositionType, ListOfPositionType_to_list>();
    fromPythonToColor();

    iterable_converter()
      .from_python<std::vector<int> >()
      .from_python<std::vector<PositionType> >()
      .from_python< ListOfPositionType >()
      .from_python<std::vector<std::vector<int> > >()
      .from_python<std::vector<Color> >()
      .from_python<std::vector<std::vector<Color> > >();

    boost::python::enum_<Result>("Result")
        .value("NotFinished", NotFinished)
        .value("Draw", Draw)
        .value("WhiteWon", WhiteWon)
        .value("BlackWon", BlackWon);
    
    boost::python::class_<AlphaBeta>("Solver")
//        .def("availableMoves", &AlphaBeta::availableMoves)
        .def("getMove", &AlphaBeta::getMove)
        .def("state_of_game", &AlphaBeta::state_of_game)
        .def("print_grid_", &AlphaBeta::print_grid_)
        .def("move", &AlphaBeta::move);
}


