cmake_minimum_required (VERSION 3.25)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SHARED_LIBRARY_SUFFIX_CXX ".so")
set(CMAKE_SHARED_LIBRARY_PREFIX_CXX "")
set(CMAKE_CXX_FLAGS "-Ofast")


project ("ChineseCheckers")


###############################################################################
## file globbing ##############################################################
###############################################################################

set(CXXFILES ./src/ChineseCheckers.cpp ./src/ChineseCheckersWrapper.cpp)
set(CXXFILESALPHABETA ./solvers/AlphaBeta/src/AlphaBeta.cpp ./solvers/AlphaBeta/src/AlphaBetaWrapper.cpp)
set(CXXFILESUNITTESTS ./src/ChineseCheckers_unittest.cpp)
set(CXXFILESBENCHMARKS ./src/ChineseCheckers_benchmark.cpp)
set(CXXFILESALPHABETABENCHMARKS ./solvers/AlphaBeta/src/AlphaBeta_benchmark.cpp)


###############################################################################
## target definitions #########################################################
###############################################################################

add_library(libChineseCheckers
                SHARED
                ${CXXFILES})

add_library(AlphaBeta
                SHARED
                ${CXXFILESALPHABETA})

add_executable(benchmarks
                ${CXXFILESBENCHMARKS})

add_executable(AlphaBeta_benchmarks
                ${CXXFILESALPHABETABENCHMARKS})

include_directories("./include")
target_include_directories(AlphaBeta PRIVATE ./solvers/AlphaBeta/include/)
target_include_directories(AlphaBeta_benchmarks PRIVATE ./solvers/AlphaBeta/include/)

###############################################################################
## dependencies ###############################################################
###############################################################################

find_package(Boost COMPONENTS python311 REQUIRED)
find_package(PythonLibs 3.11 REQUIRED)
find_package(benchmark REQUIRED)

target_link_libraries(libChineseCheckers PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_link_libraries(AlphaBeta PUBLIC libChineseCheckers ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_link_libraries(benchmarks PUBLIC libChineseCheckers benchmark::benchmark)
target_link_libraries(AlphaBeta_benchmarks PUBLIC AlphaBeta libChineseCheckers benchmark::benchmark)

target_include_directories(libChineseCheckers PUBLIC ${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
target_include_directories(AlphaBeta PUBLIC ${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
