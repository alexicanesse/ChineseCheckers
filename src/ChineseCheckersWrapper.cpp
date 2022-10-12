/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */

#include <boost/python.hpp>

#include "../include/ChineseCheckers.hpp"


BOOST_PYTHON_MODULE(ChineseCheckers) {
    boost::python::class_<ChineseCheckers>("Game")
        .def("move", &ChineseCheckers::move)
        .def("is_finished", &ChineseCheckers::is_finished)
        .def("new_game", &ChineseCheckers::new_game)
        .def("get_grid", &ChineseCheckers::get_grid);
}


