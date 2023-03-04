cmake_minimum_required (VERSION 3.20)

# ignore BOOST deprecated headers
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_SHARED_LIBRARY_SUFFIX_CXX ".so")
set(CMAKE_SHARED_LIBRARY_PREFIX_CXX "")
set(CMAKE_CXX_FLAGS "-Ofast")


project ("ChineseCheckers")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()




###############################################################################
## file globbing ##############################################################
###############################################################################

set(CXXFILES ./src/ChineseCheckers.cpp ./src/ChineseCheckersWrapper.cpp)
set(CXXFILESALPHABETA ./solvers/AlphaBeta/src/AlphaBeta.cpp ./solvers/AlphaBeta/src/AlphaBetaWrapper.cpp)
set(CXXFILESUNITTESTS ./src/ChineseCheckers_unittest.cpp)
set(CXXFILESALPHABETABENCHMARKS ./solvers/AlphaBeta/src/AlphaBeta_benchmark.cpp)
set(CXXFILESTOURNAMENT ./src/tournament.cpp)
set(CXXFILESINTUITIONDATAGENERATOR ./src/intuition_data_generator.cpp)
set(CXXFILESOPENINGSGENERATOR ./src/openings_generator.cpp)

###############################################################################
## target definitions #########################################################
###############################################################################

add_library(libChineseCheckers
                SHARED
                ${CXXFILES})

add_library(AlphaBeta
                SHARED
                ${CXXFILESALPHABETA})

if(TEST_ENABLED)
    add_executable(unittests
                ${CXXFILESUNITTESTS})
endif()

if(BENCHMARK_ENABLED)
    add_executable(AlphaBeta_benchmarks
                ${CXXFILESALPHABETABENCHMARKS})
endif()

if(TOURNAMENT_ENABLED)
    add_executable(Tournament ${CXXFILESTOURNAMENT})
endif()
#add_executable(Intuition_data_generator ${CXXFILESINTUITIONDATAGENERATOR})

if(OPENINGS_GENERATOR_ENABLED)
    add_executable(Openings_generator ${CXXFILESOPENINGSGENERATOR})
endif()



include_directories("./include")
target_include_directories(AlphaBeta PRIVATE ./solvers/AlphaBeta/include/)

if(BENCHMARK_ENABLED)
    target_include_directories(AlphaBeta_benchmarks PRIVATE ./solvers/AlphaBeta/include/)
endif()

if(TOURNAMENT_ENABLED)
    target_include_directories(Tournament PRIVATE ./solvers/AlphaBeta/include/)
endif()
#target_include_directories(Intuition_data_generator PRIVATE ./solvers/AlphaBeta/include/)

if(OPENINGS_GENERATOR_ENABLED)
    target_include_directories(Openings_generator PRIVATE ./solvers/AlphaBeta/include/)
endif()

if(TEST_ENABLED)
    add_custom_command(TARGET unittests POST_BUILD
            COMMAND cp -R ../raw_data ./raw_data
            )
    add_custom_command(TARGET unittests POST_BUILD
            COMMAND unittests
            )
endif()

###############################################################################
## dependencies ###############################################################
###############################################################################
find_package(PythonLibs 3.6 REQUIRED)

string(REPLACE "." ";" VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
list(GET VERSION_LIST 0 PYTHONLIBS_VERSION_MAJOR)
list(GET VERSION_LIST 1 PYTHONLIBS_VERSION_MINOR)


find_package(Boost COMPONENTS python${PYTHONLIBS_VERSION_MAJOR}${PYTHONLIBS_VERSION_MINOR} REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(cppflow REQUIRED)

target_link_libraries(libChineseCheckers PUBLIC ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_link_libraries(AlphaBeta PUBLIC libChineseCheckers ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} cppflow::cppflow)

if(BENCHMARK_ENABLED)
    find_package(benchmark REQUIRED)
    target_link_libraries(AlphaBeta_benchmarks PUBLIC AlphaBeta libChineseCheckers benchmark::benchmark)
endif()

if(TEST_ENABLED)
    find_package(GTest REQUIRED)
    target_link_libraries(unittests PUBLIC libChineseCheckers GTest::gtest)
endif()

if(TOURNAMENT_ENABLED)
    target_link_libraries(Tournament PUBLIC AlphaBeta)
endif()
#target_link_libraries(Intuition_data_generator PUBLIC AlphaBeta cppflow::cppflow)
if(OPENINGS_GENERATOR_ENABLED)
    target_link_libraries(Openings_generator PUBLIC AlphaBeta)
endif()

target_include_directories(libChineseCheckers PUBLIC ${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})
target_include_directories(AlphaBeta PUBLIC ${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR})
#target_include_directories(Intuition_data_generator PUBLIC ${EIGEN3_INCLUDE_DIR})