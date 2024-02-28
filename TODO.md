# TODO

## Language Features

* Add +=, -=, *=, /= operators
* Make the language statically typed, instead of dynamically typed
* Add arrays
* Add a standard library that is written in cpplox
* Decide a syntax for built in functions e.g. @print("Hello World") / @sizeof(variable)

## General

* Add a logo
* Create a folder structure in src
* Add install (cmake)
* Using more modern C++ features
* Improve testing using mocks and maybe parametrized tests
* Parsing command line arguments

## Potential Bugs

* After decaclaring a global variable, the value is not popped from the stack
* The lexer does not handle some input properly. Trailing whitespace characters are breaking the lexical analysis
