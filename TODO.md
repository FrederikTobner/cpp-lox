# TODO

## Language Features

* Add +=, -=, *=, /= operators
* Add Precrement and Postcrement operators (e.g. ++i, i++)
* Make the language statically typed, instead of dynamically typed
* Add arrays
* Add a standard library that is written in cpplox
* Decide a syntax for built in functions e.g. @print("Hello World") / @sizeof(variable)

## General

* Add a logo
* Using more modern C++ features
* Improve testing using mocks and maybe parametrized tests
* Parsing command line arguments
* Weird mixture of c inheritance and c++ inheritance in the objects (e.g. ObjectString and ObjectFunction)
* Add void to functions that have no parameters

## Potential Bugs

* After decaclaring a global variable, the value is not popped from the stack
* The lexer does not handle some input properly. Trailing whitespace characters are breaking the lexical analysis
* Having an implicit return of null adds two return null bytecode equivalent instructions to the end of every function
