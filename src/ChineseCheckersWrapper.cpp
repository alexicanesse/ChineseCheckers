#include <boost/python.hpp>

#include "../include/ChineseCheckers.hpp"

using namespace boost::python;


BOOST_PYTHON_MODULE(ChineseCheckers)
{
    class_<ChineseCheckers>("Game")
        .def("move", &ChineseCheckers::move)
        .def("is_finished", &ChineseCheckers::is_finished)
        .def("new_game", &ChineseCheckers::new_game)
    ;
}


