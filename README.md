# Chinese Checkers

This is a python library to use a fast implementation of the chinese checkers in C++, a graphic interface in Python and various solvers. The version of the game is limited to the case where there are two players and two colors. We might extend it to the complete game latter on.

We used some codes of an original implementation made by a member of our group. Here is the link to the code base [ChineseCheckers](https://github.com/Cyvernes/Chinese-Checkers).


## Table of contents 
1. [Getting Started](#getting-started)
    1. [Prerequisites](#prerequisites)
    2. [Installing](#installing)
2. [Running the tests](#running-the-tests)
3. [Built With](#built-with)
4. [Contributing](#contributing)
5. [License](#license)
6. [Acknowledgments](#acknowledgments)

## Getting Started 

These instructions will give you a copy of the project up and running on
your local machine for development and testing purposes.

### Prerequisites

Requirements for the software and other tools to build, test and push 
- [Boost](https://www.boost.org/)
- [Boost.Python](https://www.boost.org/doc/libs/1_72_0/libs/python/doc/html/index.html)
- [Tkinter](https://docs.python.org/3/library/tkinter.html)


### Installing

#### Installing prerequisites

On Ubuntu

```sh
sudo apt update
sudo apt install libboost-all-dev libboost-python-dev doxygen

pip3 install --upgrade pip
pip3 install tk cpplint
```

On macOS
```sh
brew install boost boost-python3 doxygen

pip3 install --upgrade pip
pip3 install python-tk cpplint
```

On ArchLinux
```sh
sudo pacman -S boost doxygen

pip3 install --upgrade pip
pip3 install tk cpplint
```

#### Compiling the library

Run `setup.py` in order to configure `MakeFile`. Then, run `make`.
```sh
python3 setup.py
make
```

## Documentation

The full documentation is accessible at [Documentation](https://alexicanesse.github.io/ChineseCheckers/index.html).

## Running the tests

TODO


## Built With

  - [Google C++ Style Guide](https://www.contributor-covenant.org/) - Used
    for clean C++ code.
  - [Google Python Style Guide](https://google.github.io/styleguide/pyguide.html) - Used for clean Python code.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code
of conduct, and the process for submitting pull requests to us.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE)
Creative Commons License - see the [LICENSE](LICENSE) file for
details

## Acknowledgments

TODO
