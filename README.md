[![OpenSSF Best Practices](https://bestpractices.coreinfrastructure.org/projects/6677/badge)](https://bestpractices.coreinfrastructure.org/projects/6677)
[![Documentation](https://img.shields.io/badge/Documentation-githubpages-blue)](https://alexicanesse.github.io/ChineseCheckers/index.html)
[![License](https://img.shields.io/badge/License-GPL--3.0%20license-ff69b4)](https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE)
# Chinese Checkers

This is a python library to use a fast implementation of the chinese checkers in C++, a graphic interface in Python and various solvers. The version of the game is limited to the case where there are two players and two colors. We might extend it to the complete game latter on.

We used some codes of an original implementation made by a member of our group. Here is the link to the code base [ChineseCheckers](https://github.com/Cyvernes/Chinese-Checkers).


## Table of contents 
- [Building status](#building-status)
- [Getting Started](#getting-started)
   - [Prerequisites](#prerequisites)
   - [Installing](#installing)
- [Running the tests](#running-the-tests)
- [Built With](#built-with)
- [Contributing](#contributing)
- [Code of conduct](#code-of-conduct)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Building status
| | **Architecture** |     **Build**     |
|---|:---:|:-----------------:|
| **macOS**        | x86_64 | ![Build Status](https://img.shields.io/badge/build-passing-success) |
| **Ubuntu 18.04** | x86_64 | ![Build Status](https://img.shields.io/badge/build-failing-critical) |
| **Ubuntu 22.04** | x86_64 | ![Build Status](https://img.shields.io/badge/build-passing-success) |




## Getting Started 

These instructions will give you a copy of the project up and running on
your local machine for development and testing purposes.

### Prerequisites

Requirements for the software and other tools to build, test and push 
- [Boost](https://www.boost.org/)
- [Boost.Python](https://www.boost.org/doc/libs/1_72_0/libs/python/doc/html/index.html)
- [Tkinter](https://docs.python.org/3/library/tkinter.html)
- [GoogleTest](https://github.com/google/googletest)
- [Google Benchmark](https://github.com/google/benchmark)
- [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page)
- [Abseil](https://github.com/abseil/abseil-cpp)
- [Tensorflow](https://github.com/tensorflow/tensorflow)
- [Protobuf v3.9.2](https://github.com/protocolbuffers/protobuf/releases/v3.9.2)


### Installing

#### Installing prerequisites

On Ubuntu and Debian based systems

```sh
# Boost
sudo apt-get install libboost-all-dev

# Boost.Python
sudo apt-get install libboost-python-dev

# Tkinter
sudo apt-get install python3-tk

#Google Test
sudo apt-get install libgtest-dev

#Google Benchmark
sudo apt-get install libbenchmark-dev

# Eigen3
sudo apt-get install libeigen3-dev

# Abseil
git clone https://github.com/abseil/abseil-cpp.git
cd abseil-cpp
mkdir build
cd build
cmake ..
make
sudo make install

# Tensorflow
pip3 install tensorflow

# Protobuf v3.9.2
wget https://github.com/protocolbuffers/protobuf/releases/download/v3.9.2/protobuf-cpp-3.9.2.tar.gz
tar xzf protobuf-cpp-3.9.2.tar.gz
cd protobuf-3.9.2
./configure
make
make check
sudo make install
```

On macOS
```sh
# Boost
brew install boost

# Boost.Python
brew install boost-python3

# Tkinter
brew install python-tk

# GoogleTest
brew install googletest

# Google Benchmark
brew install google-benchmark

# Eigen3
brew install eigen

# Abseil
git clone https://github.com/abseil/abseil-cpp.git
cd abseil-cpp
mkdir build
cd build
cmake ..
make
sudo make install

# Tensorflow
pip3 install tensorflow

# Protobuf v3.9.2
tar xzf protobuf-cpp-3.9.2.tar.gz
cd protobuf-3.9.2
./configure
make
make check
sudo make install
```

On ArchLinux
```sh
# Boost
sudo pacman -S boost

# Boost.Python
sudo pacman -S boost-python

# Tkinter
sudo pacman -S python-tk

# GoogleTest
sudo pacman -S googletest

# Google Benchmark
sudo pacman -S google-benchmark

# Eigen3
sudo pacman -S eigen

# Abseil
git clone https://github.com/abseil/abseil-cpp.git
cd abseil-cpp
mkdir build
cd build
cmake ..
make
sudo make install

# Tensorflow
pip3 install tensorflow

# Protobuf v3.9.2
tar xzf protobuf-cpp-3.9.2.tar.gz
cd protobuf-3.9.2
./configure
make
make check
sudo make install
```

#### Building

To build the projet, follow these steps:
- Clone the repository
- Create a build directory `mkdir bin`
- Run CMake `cmake -B ./bin -S .`
- Build the project `cmake --build ./bin`

```sh
git clone https://github.com/alexicanesse/ChineseCheckers
mkdir bin
cmake -B ./bin -S .
cmake --build ./bin
```

## Usage

- The shared library `libChineseCheckers` can be used to interface with the main game logic in other projects.
- The shared library `AlphaBeta` can be used to interface with the AlphaBeta solver in other projects.
- The executable `benchmarks` can be used to run benchmarks on the main game logic.
- The executable `AlphaBeta_benchmarks` can be used to run benchmarks on the AlphaBeta solver.
- The executable `Tournament` can be used to run tournament between different solvers.
- The executable `Intuition_data_generator` can be used to generate intuition data for the AlphaBeta solver

## Documentation

The full documentation is accessible at [Documentation](https://alexicanesse.github.io/ChineseCheckers/index.html).


## Built With

  - [Google C++ Style Guide](https://www.contributor-covenant.org/) - Used
    for clean C++ code.
  - [Google Python Style Guide](https://google.github.io/styleguide/pyguide.html) - Used for clean Python code.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code
of conduct, and the process for submitting pull requests to us.

Note that while we already have certain standards in place, nothing is set 
in stone. If you have an issue with the way code is structured, with any 
libraries we are using, or with any processes involved with contributing, 
please bring it up. We welcome all feedback so we can make contributing
to this project as painless as possible.

## Code of conduct 

We have adopted the code of conduct defined by the Contributor Covenant. This document is 
used across many open source communities, and we think it articulates our values well.
For more, see the [Code of Conduct](code_of_conduct.md).

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE)
Creative Commons License - see the [LICENSE](LICENSE) file for
details

[tl;dr](https://tldrlegal.com/license/gnu-lesser-general-public-license-v3-(lgpl-3)#summary) This license is mainly applied to libraries. You may copy, 
distribute and modify the software provided that modifications are 
described and licensed for free under LGPL. Derivatives works 
(including modifications or anything statically linked to the library) 
can only be redistributed under LGPL, but applications that use the library
don't have to be.

