[![OpenSSF Best Practices](https://bestpractices.coreinfrastructure.org/projects/6677/badge)](https://bestpractices.coreinfrastructure.org/projects/6677)

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
| | **Architecture** | **Build** |
|---|:---:|:---:|
| **macOS**        | x86_64 |[![Build Status](https://ci.swift.org/job/oss-swift-package-macos/lastCompletedBuild/badge/icon)](https://ci.swift.org/job/oss-swift-package-macos)|
| **Ubuntu 18.04** | x86_64 |[![Build Status](https://ci.swift.org/job/oss-swift-package-ubuntu-18_04/lastCompletedBuild/badge/icon)](https://ci.swift.org/job/oss-swift-package-ubuntu-18_04)|
| **Ubuntu 22.04** | x86_64 |[![Build Status](https://ci.swift.org/job/oss-swift-package-ubuntu-22_04/lastCompletedBuild/badge/icon)](https://ci.swift.org/job/oss-swift-package-ubuntu-22_04)|




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

Run `make`.
```sh
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

## Acknowledgments

TODO
