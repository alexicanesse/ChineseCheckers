[![OpenSSF Best Practices](https://bestpractices.coreinfrastructure.org/projects/6677/badge)](https://bestpractices.coreinfrastructure.org/projects/6677)
[![Documentation](https://img.shields.io/badge/Documentation-githubpages-blue)](https://alexicanesse.github.io/ChineseCheckers/html/index.html)
[![License](https://img.shields.io/badge/License-GPL--3.0%20license-ff69b4)](https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE)
# Chinese Checkers Solver using Alpha Beta pruning in C++

## Overview

This project is a solver for two players' Chinese Checkers game using the Alpha Beta pruning algorithm. The program is written in C++ and uses a number of heuristics to improve the performance of the algorithm.

We used some codes of an original implementation made by a member of our group. Here is the link to the code base [ChineseCheckers](https://github.com/Cyvernes/Chinese-Checkers).

Overall, the Chinese Checkers solver program has been built with performance in mind, and is designed to deliver fast 
and efficient game play. Whether you are playing against a bot or using the program as a library in your own project, 
you can be confident that the program will deliver high-performance results.

## Table of contents 
- [Overview](#overview)
- [Rules of the game](#rules-of-the-game)
- [Building status](#building-status)
- [Getting Started](#getting-started)
   - [Prerequisites](#prerequisites)
   - [Installing](#installing)
     - [Installing prerequisites](#installing-prerequisites)
     - [Building](#building)
- [Usage](#usage)
- [Usage for developers](#usage-for-developers)
- [Optimisations](#optimisations)
- [Documentation](#documentation)
- [Built With](#built-with)
- [Contributing](#contributing)
- [Code of conduct](#code-of-conduct)
- [License](#license)

## Building status
| | **Architecture** |     **Build**     |
|---|:---:|:-----------------:|
| **macOS**        | x86_64 | ![Build Status](https://img.shields.io/badge/build-passing-success) |
| **Ubuntu 18.04** | x86_64 | ![Build Status](https://img.shields.io/badge/build-failing-critical) |
| **Ubuntu 22.04** | x86_64 | ![Build Status](https://img.shields.io/badge/build-passing-success) |


## Rules of the game

Chinese checkers is a classic turn-based game played by two players. The objective of the game is to fill 
the opponent's starting zone with at least one of your ten pawns before they do the same to your starting zone.

To move a pawn, a player has two options: move it to an adjacent tile or jump over another pawn. 
To execute a jump, a pawn must be exactly n tiles away from another pawn, with every tile in between 
being free, and the n+1 tiles on the other side must also be free. If a pawn jumps over another pawn,
it can continue to move by jumping over other pawns, as long as it is a legal move.

To prevent unsporting behavior, the game has rules that disallow certain positions. A position is considered illegal 
if any of the other player's pawns cannot reach one or more free tiles in a player's starting zone.

Chinese checkers is a game of strategy and planning, and the ability to think ahead is essential to succeed. 
The game has a simple set of rules, but mastering it requires practice and patience. So, gather a friend and enjoy 
an exciting game of Chinese checkers today!

## Getting Started 

These instructions will give you a copy of the project up and running on
your local machine for development and testing purposes.

### Prerequisites

Requirements for the software and other tools to build, test and push 
- [pip3](https://pypi.org/project/pip/)
- [Numpy](https://numpy.org)
- [Boost](https://www.boost.org/)
- [Boost.Python](https://www.boost.org/doc/libs/1_72_0/libs/python/doc/html/index.html)
- [Tkinter](https://docs.python.org/3/library/tkinter.html)
- [GoogleTest](https://github.com/google/googletest)
- [Google Benchmark](https://github.com/google/benchmark)
- [Eigen3](https://eigen.tuxfamily.org/index.php?title=Main_Page)
- [Tensorflow C API](https://www.tensorflow.org/install/lang_c)
- [Cppflow](https://github.com/serizba/cppflow)


### Installing

#### Installing prerequisites

On Ubuntu and Debian based systems

```sh
# pip3
sudo apt install python3-pip
# Numpy
pip3 install numpy
# Boost
sudo apt-get install libboost-all-dev
# Boost.Python
sudo apt-get install libboost-python-dev
# Tkinter
sudo apt-get install python3-tk
# Google Test
sudo apt-get install libgtest-dev
# Google Benchmark
sudo apt-get install libbenchmark-dev
# Eigen3
sudo apt-get install libeigen3-dev
# Tensorflow C API
FILENAME=libtensorflow-cpu-linux-x86_64-2.11.0.tar.gz
wget -q --no-check-certificate https://storage.googleapis.com/tensorflow/libtensorflow/${FILENAME}
sudo tar -C /usr/local -xzf ${FILENAME}
# cppflow
git clone https://github.com/serizba/cppflow
cd cppflow
mkdir build
cd build
cmake ..
make -j
sudo make install
```

On macOS
```sh
# Numpy
pip3 install numpy
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
# Tensorflow C API
FILENAME=libtensorflow-cpu-darwin-x86_64-2.11.0.tar.gz
wget -q --no-check-certificate https://storage.googleapis.com/tensorflow/libtensorflow/${FILENAME}
sudo tar -C /usr/local -xzf ${FILENAME}
# cppflow
git clone https://github.com/serizba/cppflow
cd cppflow
mkdir build
cd build
cmake ..
make -j
sudo make install
```

#### Building

To build the projet, follow these steps:
1. Clone the repository to your local machine using Git.
2. Create a build directory by running the following command: `mkdir bin`
3. Run CMake to generate the build files in the bin directory: `cmake -B ./bin -S .`
4. Build the project by running the following command: `cmake --build ./bin`

```sh
git clone https://github.com/alexicanesse/ChineseCheckers
mkdir bin
cmake -B ./bin -S .
cmake --build ./bin
```

## Usage

Assuming that the project has been successfully built using the instructions provided, you can now use `ChineseCheckers.py` 
to play the game. This file contains the necessary code to run the Chinese Checkers solver program 
and simulate a game between two players.

To start playing, simply run the `ChineseCheckers.py` file using your preferred Python interpreter. Please note that the
has only been tested with `python3.11` and ``python3.10`.

The program will also display the game board after each move, so you can keep track of the progress.

It's worth noting that the Chinese Checkers solver program developed using Alpha Beta pruning can be played in different 
modes, depending on the type of players you want to simulate. The program allows you to specify the number of human and 
bot players in the game, as well as their respective difficulty levels. This means that you can play against another 
human player, against a bot, or even watch two bots play against each other.

If you choose to play against a bot, the program will use the Alpha Beta pruning algorithm to compute the best move for 
the bot player based on the current state of the game. You can choose the difficulty level of the bot player,
which will determine how deep the algorithm will search for the best move.

## Usage for developers

If you are a developer, you can make use of several components of the Chinese Checkers solver program in your own 
projects. 

The shared libraries `libChineseCheckers` and `AlphaBeta` can be used to interface with the main game logic and solver,
respectively, in other projects (in C++ and Python). These libraries have been carefully designed to provide efficient and reliable 
functionality that can be integrated into a wide range of projects.

Here are the other components:
 - `AlphaBeta_benchmarks`: This executable can be used to run benchmarks on the Alpha Beta pruning solver. 
You can use this tool to measure the performance of the solver in various scenarios, and to identify 
areas for optimization.
 - `Tournament`: This executable can be used to improve solvers using a genetic algorithm. 
 - `Intuition_data_generator`: This executable can be used to generate intuition data for the Alpha Beta pruning solver. 
You can use this tool to generate training data that can be used to improve the performance of the solver.

Use the [documentation](#documentation) for details about the use of those libraries.

 Overall, the Chinese Checkers solver program has been built with performance in mind, and is designed to deliver fast 
 and efficient game play. Whether you are using the shared libraries in your own project or running the executables 
 to measure and optimize performance, you can be confident that the program will deliver high-performance results.

## Optimisations

TODO

## Documentation

The full documentation is accessible at [Documentation](https://alexicanesse.github.io/ChineseCheckers/html/index.html).


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

