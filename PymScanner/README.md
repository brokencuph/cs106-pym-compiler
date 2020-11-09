# Pym Scanner
Authors:
HE YUJIE, 1809853J-I011-0065  
CHEN YUXUAN, 1809853J-I011-0011  
HE PEILIN, 1809853U-I011-0078

## Introduction
This program implements a scanner (or lexical analyser) for Pym language, described in *Pym Language v2.pdf*. All given Pym examples are tested and can be correctly processed to generate tokens (including errors).

## How to compile
The development platform is *Visual Studio 2019*, and the language is C++ (conforms to the **ISO C++14** Standard). Recent versions of *g++* or *cl*, which supports **C++14** standard, should be able to compile the program.

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

## Modules
### dfa

### token

### scanner

### utils

### scanner_driver

## Highlights

## Problems & Bugs (Potential)