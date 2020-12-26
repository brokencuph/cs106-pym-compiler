# PymParser
Author: HE YUJIE, 10065,  
        CHEN YUXUAN, 10011,  
        HE PEILIN, 10078  

# Introduction
This program includes a parser and its corresponding scanner (library) of Pym language.

# Compile and Run
This program is intended to compile using compilers supporting **ISO C++17** language features and libraries. We tested compilation using `cl` (Microsoft C++ Compiler for x64 version 19.28.29335, shipped with VS 2019 ) and `g++` (GNU C++ Compiler version 9.3.0), with commands like below:
```
cl /EHsc /Feparser /O2 /std:c++17 dfa.cpp parser.cpp parser_driver.cpp parsetree.cpp scanner.cpp token.cpp utils.cpp
```
```
g++ -std=c++17 -O2 -o parser dfa.cpp parser.cpp parser_driver.cpp parsetree.cpp scanner.cpp token.cpp utils.cpp
```
Any recent version of C++ compiler could also be used.

There are 2 ways to run our program:
1. Directly execute the program, and it will prompt you to enter a path to a Pym source file;
2. Provide a path to a Pym source file as the first command-line argument, e.g. `./parser a.pym`.

# Supported features
Theoretically, all documented basic features of Pym language (in file `Pym language v2.pdf`) have been supported by our parser. However, we have not tested many source code files other than those provided.

When parsing the provided source files, some errors occur as our language does not support direct type cast, e.g. Python code

```python
a = str(b) # assume b is a number
```
should be replaced by 
```
a = num2str(b)
```
So we modified the test source files to make them more correct. After that, our parser can generate a proper parse tree.

# Team cooperation
Tasks completed by each person are documented below:

|Name|Task|
|----|----|
|HE, YUJIE|Design of the whole program and the parse tree data structures, and implementation of some parser functions (mainly the expression part)|
|CHEN, YUXUAN|Implementation of parse tree formatted output method, and some parser functions (mainly the declaration part)|
|HE, PEILIN|Implementation of some parser functions (including complex ones such as the if-statement)|

# Sample Output
(Test programs are provided in `pymtests` folder)
arrayMax.pym:
```
:) Hello, what is the name of the Pym source file?
>>> pymtests/arrayMax.pym
Scanner succeeds!

1: LIST STMT
  2: STMT DEF max TBD 0 0
    2: LIST PARAM
      2: PARAM DYN arr TBD 0 0
      2: PARAM DYN size TBD 0 0
    5: STMT CMPD
      5: LIST STMT
        5: STMT EXPR
          5: EXPR OP ASSI
            5: EXPR ID j
            5: EXPR NUM 0\1
        6: STMT EXPR
          6: EXPR OP ASSI
            6: EXPR ID result
            6: EXPR ARRAY arr[exp]
              6: EXPR ID j
        7: STMT WHILE
          7: EXPR OP LT
            7: EXPR ID j
            7: EXPR ID size
          8: STMT CMPD
            8: LIST STMT
              8: STMT IF
                8: STMT IFC
                  8: EXPR OP GT
                    8: EXPR ARRAY arr[exp]
                      8: EXPR ID j
                    8: EXPR ID result
                  9: STMT CMPD
                    9: LIST STMT
                      9: STMT EXPR
                        9: EXPR OP ASSI
                          9: EXPR ID result
                          9: EXPR ARRAY arr[exp]
                            9: EXPR ID j
              10: STMT EXPR
                10: EXPR OP ASSI
                  10: EXPR ID j
                  10: EXPR OP PLUS
                    10: EXPR ID j
                    10: EXPR NUM 1\1
        11: STMT RETURN
          11: EXPR ID result
  13: STMT DEF print_num_array TBD 0 0
    13: LIST PARAM
      13: PARAM DYN arr TBD 0 0
      13: PARAM DYN n TBD 0 0
    14: STMT CMPD
      14: LIST STMT
        14: STMT EXPR
          14: EXPR OP ASSI
            14: EXPR ID j
            14: EXPR NUM 0\1
        15: STMT WHILE
          15: EXPR OP LT
            15: EXPR ID j
            15: EXPR OP MINUS
              15: EXPR ID n
              15: EXPR NUM 1\1
          16: STMT CMPD
            16: LIST STMT
              16: STMT EXPR
                16: EXPR CALL printnum(params)
                  16: LIST ARG
                    16: EXPR ARRAY arr[exp]
                      16: EXPR ID j
              17: STMT EXPR
                17: EXPR CALL printstr(params)
                  17: LIST ARG
                    17: EXPR STR ", "
              18: STMT EXPR
                18: EXPR OP ASSI
                  18: EXPR ID j
                  18: EXPR OP PLUS
                    18: EXPR ID j
                    18: EXPR NUM 1\1
        19: STMT EXPR
          19: EXPR CALL print(params)
            19: LIST ARG
              19: EXPR ARRAY arr[exp]
                19: EXPR ID j
        20: STMT EXPR
          20: EXPR CALL printstr(params)
            20: LIST ARG
              20: EXPR STR "\n"
  22: STMT DEF driver TBD 0 0
    22: LIST PARAM
    23: STMT CMPD
      23: LIST STMT
        23: STMT DECL arr NUM 1 5
        26: STMT EXPR
          26: EXPR OP ASSI
            26: EXPR ID j
            26: EXPR NUM 0\1
        27: STMT WHILE
          27: EXPR OP LT
            27: EXPR ID j
            27: EXPR NUM 5\1
          30: STMT CMPD
            30: LIST STMT
              30: STMT EXPR
                30: EXPR CALL printstr(params)
                  30: LIST ARG
                    30: EXPR STR "please input an integer: \n"
              31: STMT EXPR
                31: EXPR OP ASSI
                  31: EXPR ARRAY arr[exp]
                    31: EXPR ID j
                  31: EXPR CALL inputnum(params)
                    31: LIST ARG
              32: STMT EXPR
                32: EXPR OP ASSI
                  32: EXPR ID j
                  32: EXPR OP PLUS
                    32: EXPR ID j
                    32: EXPR NUM 1\1
        33: STMT EXPR
          33: EXPR CALL print(params)
            33: LIST ARG
              33: EXPR STR "The array is : "
        34: STMT EXPR
          34: EXPR CALL print_num_array(params)
            34: LIST ARG
              34: EXPR ID arr
        35: STMT EXPR
          35: EXPR CALL print(params)
            35: LIST ARG
              35: EXPR OP PLUS
                35: EXPR OP PLUS
                  35: EXPR STR "The maximum in the array is "
                  35: EXPR CALL num2str(params)
                    35: LIST ARG
                      35: EXPR CALL max(params)
                        35: LIST ARG
                          35: EXPR ID arr
                          35: EXPR NUM 5\1
                35: EXPR STR "\n"
        36: STMT EXPR
          36: EXPR OP ASSI
            36: EXPR ID s
            36: EXPR OP DIV
              36: EXPR OP MUL
                36: EXPR OP MINUS
                  36: EXPR OP PLUS
                    36: EXPR ARRAY arr[exp]
                      36: EXPR NUM 0\1
                    36: EXPR ARRAY arr[exp]
                      36: EXPR NUM 1\1
                  36: EXPR ARRAY arr[exp]
                    36: EXPR NUM 2\1
                36: EXPR ARRAY arr[exp]
                  36: EXPR NUM 3\1
              36: EXPR ARRAY arr[exp]
                36: EXPR NUM 4\1
        37: STMT EXPR
          37: EXPR CALL printstr(params)
            37: LIST ARG
              37: EXPR OP PLUS
                37: EXPR OP PLUS
                  37: EXPR STR "(arr[0]+arr[1]-arr[2])*arr[3]/arr[4] = "
                  37: EXPR CALL num2str(params)
                    37: LIST ARG
                      37: EXPR ID s
                37: EXPR STR "\n"
        38: STMT EXPR
          38: EXPR CALL print(params)
            38: LIST ARG
              38: EXPR STR "Bye"
  41: STMT EXPR
    41: EXPR CALL driver(params)
      41: LIST ARG
```

complex_expression_wrong.pym:
```
:) Hello, what is the name of the Pym source file?
>>> pymtests/complex_expression_wrong.pym
Scanner succeeds!

Parser error! ] expected
When processing token: 9 NEWLINE
```