# Contributing guidelines

## Table of content 
 - [Pull Request Checklist](#pull-request-checklist)
 - [How to become a contributor and submit your own code](#how-to-become-a-contributor-and-submit-your-own-code)
   - [Workflow of a pull request](#workflow-of-a-pull-request) 
   - [General guidelines](#general-guidelines)
   - [Coding style](#coding-style)
     - [C++ coding style](#c++-coding-style) 
     - [Python coding style](#python-coding-style)


## Pull Request Checklist 
- Read the [contributing guidelines](CONTRIBUTING.md)
- Read the [Code of conduct](code_of_conduct.md)
- Make sure your changes are consistent with the [conventions](#general-guidelines).
- Make sure the project still passes the tests.

## How to become a contributor and submit your own code  

### Workflow of a pull request
1. **New Pull Request**. We review every pull requests. 
If the pull request meet the basic requirements, we sent it to the next step.
If it is not ready to go to the next step yet, we leave a message on your PR to 
indicate how to improve it.
2. **Assigment of a reviewer**. A reviewer is assigned to your PR. The 
reviewer may ask you to make changes until everything looks good and 
your PR passes all the tests.
3. **Approved**. When the PR is approved by the reviewer, your code is 
integrated to the main branch and scheduled to be included in the next 
release.

### General guidelines 

Before sending your pull request, make sure your enhancements are consistent 
with the guidelines. If they do not meet all the requirements, we wont
be able to accept your PR.  

Whenever you add new features, include tests. they will help to 
validate your PR faster, and they will prevent future breaking.

Any modification in the main library must be done with compatibility in 
mind. We are not against progress. But any modification that change the
use of the library requires all the projects using it to be rewritten 
after the next update of the library. Such a burden could prevent some 
projects from being updated to the last version of the project.

All the tests must follow the [testing best practice](TESTING.md).

All new files must include the licence. C++ exemple:
```cpp
/*
 * This file is part of ChineseCheckers which is released under GNU General Public License v3.0.
 * See file LICENSE or go to https://github.com/alexicanesse/ChineseCheckers/blob/main/LICENSE for full license details.
 * Copyright 2022 - ENS de Lyon
 */
```

### Coding style

#### C++ coding style 

All C++ code must conform to [Google's C++ Style guide](https://google.github.io/styleguide/cppguide.html).

You can check a C++ file using `cpplint`.

#### Python Coding style

All python code must conform to [Google's Python Style Guide](https://github.com/google/styleguide/blob/gh-pages/pyguide.md).

You can check a Python file using `pylint`.





