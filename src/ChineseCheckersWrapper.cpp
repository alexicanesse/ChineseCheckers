/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

/**
 * \file ChineseCheckersWrapper.hpp
 * \brief Wrapper for the python library
 *
 * This file is used to make the C++ code of ChineseCheckers available to Python.
 *
 */

/* C++ Libraries */
/* The following pragma are used to removed deprecation warning from boost
 * header files. Using them avoid to remove this warning from the entire project.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/stl_iterator.hpp>
#pragma GCC diagnostic pop

/* Other */
#include "../include/ChineseCheckers.hpp"

/// @brief Type that allows for registration of conversions from
///        python iterable types.
struct iterable_converter {
  /// @note Registers converter from a python iterable type to the
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
        auto *result = new boost::python::list;
        for (const std::vector<Color> &x : grid) {
            boost::python::list row;
            for (Color value : x)
                row.append(static_cast<int>(value));
            result->append(row);
        }
        return boost::python::incref(result->ptr());
    }
};

struct vector_of_vector_of_positiontype_to_list {
    static PyObject* convert(std::vector<std::vector<PositionType>>
                             const& position_colors_players_) {
        auto *result = new boost::python::list;
        for (const std::vector<PositionType> &x : position_colors_players_) {
            boost::python::list row;
            for (PositionType value : x) {
                boost::python::list position;
                position.append(value.at(0));
                position.append(value.at(1));
                row.append(position);
            }
            result->append(row);
        }
        return boost::python::incref(result->ptr());
    }
};


BOOST_PYTHON_MODULE(libChineseCheckers) {
    boost::python::to_python_converter<GridType, gridtype_to_list>();
    boost::python::to_python_converter<std::vector<std::vector<PositionType>>,
                                    vector_of_vector_of_positiontype_to_list>();

    iterable_converter()
      .from_python<std::vector<int> >()
      .from_python<std::vector<PositionType> >()
      .from_python< ListOfPositionType >()
      .from_python<std::vector<std::vector<int> > >()
      .from_python<std::vector<std::vector<Color> > >();

    boost::python::class_<ChineseCheckers>("Game")
        .def("move", &ChineseCheckers::move)
        .def("state_of_game", &ChineseCheckers::state_of_game)
        .def("new_game", &ChineseCheckers::new_game)
        .def("print_grid_", &ChineseCheckers::print_grid_)
        .def("get_grid_", &ChineseCheckers::get_grid_)
        .def("print_position_colors_players_",
             &ChineseCheckers::print_position_colors_players_)
        .def("get_position_colors_players_",
             &ChineseCheckers::get_position_colors_players_)
        .def("print_who_is_to_play_", &ChineseCheckers::print_who_is_to_play_)
        .def("get_who_is_to_play_", &ChineseCheckers::get_who_is_to_play_);
}


