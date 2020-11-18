# Pym Scanner
Authors:
HE YUJIE, 1809853J-I011-0065  
CHEN YUXUAN, 1809853J-I011-0011  
HE PEILIN, 1809853U-I011-0078

## Introduction
This program implements a scanner (or lexical analyser) for Pym language (a simple language that is similar to Python), described in *Pym Language v2.pdf*. All given Pym examples are tested and can be correctly processed to generate tokens (including errors).

## How to compile
The development platform is *Visual Studio 2019*, and the language is C++ (conforms to the **ISO C++14** Standard). Recent versions of *g++* or *cl*, which supports **C++14** standard, should be able to compile the program. **Older compilers may fail to compile our program.**

The compilation command could be like the following one:
```sh
g++ -std=c++14 dfa.cpp token.cpp scanner_driver.cpp scanner.cpp utils.cpp -o pymscanner
```

## Usage
If you run the compiled program without any command-line arguments, it will interactively prompt you to enter the file name of your Pym source. Otherwise, provide your file name as the first argument, then it will directly process that file.

Example:
```sh
./pymscanner a.pym
```

## General Design
### Design of the DFA
To make the comprehension easily, you can read JPG file ***DFA implement***. ![DFA implement](DFA%20implement.jpg)

This picture may not contain all of the DFA implemented by the program. For more specific details, you can refer to progam code file ***dfa.cpp*** 

### Handling of Indentation
Instead of strictly following the algorithm provided, which process the input line by line, we integrate the indentation process into the DFA. Once a new line is to be processed, the DFA is put into a special state called *START_NEWLINE*, which involves consideration about the *INDENT* and *DEDENT* tokens. Since indentation may produce zero or multiple *DEDENT* tokens at one time, we use the string field of a token to store how many *DEDENT* should be created, and the actual insertion is handled by upper-level *scanner* module.

## Modules
### dfa
This module contains the implementation of the Pym DFA. It uses some global variables with **internal linkage** to store current DFA state information, including string position, current state, coming token and line number. Function *dfa* gets a new character every time, and call relevant function to do state transfer.
### token
This module contains enum constants for *TokenType*, and the definition of *Token* struct, which holds information about a single token.
### scanner
This module makes use of the DFA and puts the results into a doubly linked list (implementation provided by *std::list*), and produces error messages when necessary.
### utils
This module contains general utility functions, such as reading from file.
### scanner_driver
This module contains the *main* function - user interface of the scanner.

## Feature Highlights
### Report multiple errors at a time
Once our scanner detects an error, it will report it and continue with successive characters to find if there are more errors. In this case, the tokens generated should not be trusted, but many errors can still be correctly reported.

### Colored output for error message
Following the design of modern *gcc* compilers, we use ASCII escape characters to make some text in errors red, in order to make them more alerting.

### Line number included
When we report tokens or errors, line number is always included in the message, so that user can easily locate the problems in their Pym program.

## Code Styles
### Use of C++ standard library to reduce chance of bugs
We make full use of C++ standard library facilities and their relevant language features to make the code easier to understand and to reduce the number of potential bugs.

For example, *std::list* is used for doubly linked list for tokens, while *std::string* is used for storing character strings inside token.

### Careful memory management to ensure performance
We try to use C++ class facilities to manage data objects (mainly tokens). Since tokens contain character strings, which may be time-consuming and space-consuming if managed unproperly, we carefully handle cases involving its construction, destruction and migrating. For example, we use C++11 move semantics to ensure only the pointer to string is copied when it is put into the token list, instead of copying the whole string content.

### Use of tables
- Since the number of DFA states is large, we separate handlers for different states in different functions, and create a table (array of function pointers) to manage them. Then in the *dfa* function, we just look up the table for the transfer function we need, which significantly reduce the code in the main *dfa* function.
- For keywords, we use *std::unordered_map* to record the mapping from their string form to *TokenType* enumeration type. *std::unordered_map* is implemented using hash table, which supports O(1) looking up operation.

## Problems & Bugs (Potential)
### Cannot handle non-ASCII characters in the source file
Since we treat every byte in the input file as a single text unit, support for characters exceeding 8-bit range is not available. The consequence is that the source file may not be correctly processed if it contains those characters, such as Chinese characters and punctuations.
