# PymSemanticAnalyser

Author: HE YUJIE, 1809853J-I011-0065  
      CHEN YUXUAN, 1809853J-I011-0011  
      HE PEILIN, 1809853U-I011-0078

## Introduction

This program includes a semantic analyser and its supporters (scanner and parser). The computation is to embellish the tree with more particles (more data like info of types and symbols) based on a
parse tree that is provided by a parser.

A symbol table is a data structure used by a language translator such as a compiler or interpreter, where each identifier in a program's source code is associated with information relating to its declaration or appearance in the source. In other words, the entries of a symbol table store the information related to the entry's corresponding symbol.

## Compile and Run

This program is intended to compile using compilers supporting **ISO C++17** language features and libraries. We tested compilation using `cl` (Microsoft C++ Compiler for x64 version 19.28.29335, shipped with VS 2019 ) and `g++` (GNU C++ Compiler version 9.3.0), with commands like below:

```code
cl /EHsc /Fesanalyser /O2 /std:c++17 dfa.cpp parser.cpp parsetree.cpp scanner.cpp token.cpp utils.cpp s_analyser.cpp symbol_table.cpp sa_driver.cpp
```

```code
g++ -std=c++17 -O2 -o sanalyser dfa.cpp parser.cpp parsetree.cpp scanner.cpp token.cpp utils.cpp s_analyser.cpp symbol_table.cpp sa_driver.cpp
```

Any recent version of C++ compiler could also be used.

## General Design

### Error List

The task is to provide a list of errors, and the corresponding error messages that will be detected by analyzer of Pym, like the error messages listed in `./error-list.txt` for the language to describe the errors of Pym to be found by an analyzer.

## Modules

### Semantic Analyser

The specific codes of this module can be seen in `./s_analyser.cpp` and `s_analyser.h`, and it is the most important part of semantic analyser. To implement our semantic analyser, we produce symbol table by judging each tree node in order to reporting the specific errors.

### Symbol Table

The specific codes of this module can be seen in `./symbol_table.cpp` and `symbol_table.h`, which realizes printing, checking as well as locating symbol table and implementing hash table with *std::unordered_map*.  

## Supported features

Totally, this part can realize numerous function which includes scanning and parsing in pretreatment before processing symbol table.

In the sample of symbol table, the main information will be listed including the *symbol table ID*, *element name*, *element kind*, *line number of declaration*, *line number of reference*. Besides, if declaring a parameter or variable name, the table will show its type like `<NUM>`. And `Dcl line` printing 0 and `Kind` printing "*FUNC*" means this declaration is a built-in function of Pym.(To find more built-in functions you can refer to file `Pym language v2.pdf`).

As Pym supports use of a variable with direct assignment, such as *x = 5*, we cannot fix types of some expression, since the type of *x* is determined only at run-time, and may be changed by later assignments. For expression involving this kind of *dynamic variable* we all assign type *TBD* and ignore some rules for type matching.

Furthermore, we add some codes in `./sa_driver.cpp`(main function) to add **NEWLINE** before **FEOF**
and remove continuous duplicate **NEWLINE** as a processor between pure scanner and parser.

## Sample Output

Our test also print the parser tree to do favor to debug and find the fixed position of possible error.
(Test programs are provided in `pymtests` folder)
`arrayMax.pym`:

```text
:) Hello, what is the name of the Pym source file?
>>> arrayMax.pym
Scanner succeeds!

Parser succeeds!

Symbol table:
Table Name                Kind                Dcl  Ref
ID                                            line lines
----  ----                ----                ---- ----
0     printnum            FUNC                0
                                                   16
                                                   19
0     simple              FUNC                0
0     printstr            FUNC                0
                                                   17
                                                   20
                                                   30
                                                   33
                                                   35
                                                   37
                                                   38
0     max                 FUNC                2
                                                   35
0     inputnum            FUNC                0
                                                   31
0     num2str             FUNC                0
                                                   35
                                                   37
0     inputstr            FUNC                0
0     str2num             FUNC                0
0     sign                FUNC                0
0     top                 FUNC                0
0     int                 FUNC                0
0     bot                 FUNC                0
0     frac                FUNC                0
0     print_num_array     FUNC                13
                                                   34
0     driver              FUNC                22
                                                   41
1     arr                 PARAM_VAR <TBD>     2
                                                   6
                                                   8
                                                   9
1     result              DYN_VAR             6
                                                   6
                                                   8
                                                   9
                                                   11
1     size                PARAM_VAR <TBD>     2
                                                   7
1     j                   DYN_VAR             5
                                                   5
                                                   6
                                                   7
                                                   8
                                                   9
                                                   10
                                                   10
2     arr                 PARAM_VAR <TBD>     13
                                                   16
                                                   19
2     n                   PARAM_VAR <TBD>     13
                                                   15
2     j                   DYN_VAR             14
                                                   14
                                                   15
                                                   16
                                                   18
                                                   18
                                                   19
3     arr                 DCL_ARRAY <NUM>     23
                                                   31
                                                   34
                                                   35
                                                   36
                                                   36
                                                   36
                                                   36
                                                   36
3     j                   DYN_VAR             26
                                                   26
                                                   27
                                                   31
                                                   32
                                                   32
3     s                   DYN_VAR             36
                                                   36
                                                   37

Christmas Tree:
1: LIST STMT
  2: STMT DEF max TBD 0 0
    2: LIST PARAM
      2: PARAM DYN arr TBD 0 0
      2: PARAM DYN size TBD 0 0
    5: STMT CMPD
      5: LIST STMT
        5: STMT EXPR
          5: EXPR OP ASSI <TBD>
            5: EXPR ID j <TBD>
            5: EXPR NUM 0\1 <INT>
        6: STMT EXPR
          6: EXPR OP ASSI <TBD>
            6: EXPR ID result <TBD>
            6: EXPR ARRAY arr[exp] <TBD>
              6: EXPR ID j <TBD>
        7: STMT WHILE
          7: EXPR OP LT <TBD>
            7: EXPR ID j <TBD>
            7: EXPR ID size <TBD>
          8: STMT CMPD
            8: LIST STMT
              8: STMT IF
                8: STMT IFC
                  8: EXPR OP GT <TBD>
                    8: EXPR ARRAY arr[exp] <TBD>
                      8: EXPR ID j <TBD>
                    8: EXPR ID result <TBD>
                  9: STMT CMPD
                    9: LIST STMT
                      9: STMT EXPR
                        9: EXPR OP ASSI <TBD>
                          9: EXPR ID result <TBD>
                          9: EXPR ARRAY arr[exp] <TBD>
                            9: EXPR ID j <TBD>
              10: STMT EXPR
                10: EXPR OP ASSI <TBD>
                  10: EXPR ID j <TBD>
                  10: EXPR OP PLUS <TBD>
                    10: EXPR ID j <TBD>
                    10: EXPR NUM 1\1 <INT>
        11: STMT RETURN
          11: EXPR ID result <TBD>
  13: STMT DEF print_num_array TBD 0 0
    13: LIST PARAM
      13: PARAM DYN arr TBD 0 0
      13: PARAM DYN n TBD 0 0
    14: STMT CMPD
      14: LIST STMT
        14: STMT EXPR
          14: EXPR OP ASSI <TBD>
            14: EXPR ID j <TBD>
            14: EXPR NUM 0\1 <INT>
        15: STMT WHILE
          15: EXPR OP LT <TBD>
            15: EXPR ID j <TBD>
            15: EXPR OP MINUS <TBD>
              15: EXPR ID n <TBD>
              15: EXPR NUM 1\1 <INT>
          16: STMT CMPD
            16: LIST STMT
              16: STMT EXPR
                16: EXPR CALL printnum(params) <TBD>
                  16: LIST ARG
                    16: EXPR ARRAY arr[exp] <TBD>
                      16: EXPR ID j <TBD>
              17: STMT EXPR
                17: EXPR CALL printstr(params) <TBD>
                  17: LIST ARG
                    17: EXPR STR ", " <STRING>
              18: STMT EXPR
                18: EXPR OP ASSI <TBD>
                  18: EXPR ID j <TBD>
                  18: EXPR OP PLUS <TBD>
                    18: EXPR ID j <TBD>
                    18: EXPR NUM 1\1 <INT>
        19: STMT EXPR
          19: EXPR CALL printnum(params) <TBD>
            19: LIST ARG
              19: EXPR ARRAY arr[exp] <TBD>
                19: EXPR ID j <TBD>
        20: STMT EXPR
          20: EXPR CALL printstr(params) <TBD>
            20: LIST ARG
              20: EXPR STR "\n" <STRING>
  22: STMT DEF driver TBD 0 0
    22: LIST PARAM
    23: STMT CMPD
      23: LIST STMT
        23: STMT DECL arr NUM 1 5
        26: STMT EXPR
          26: EXPR OP ASSI <TBD>
            26: EXPR ID j <TBD>
            26: EXPR NUM 0\1 <INT>
        27: STMT WHILE
          27: EXPR OP LT <TBD>
            27: EXPR ID j <TBD>
            27: EXPR NUM 5\1 <INT>
          30: STMT CMPD
            30: LIST STMT
              30: STMT EXPR
                30: EXPR CALL printstr(params) <TBD>
                  30: LIST ARG
                    30: EXPR STR "please input an integer: \n" <STRING>
              31: STMT EXPR
                31: EXPR OP ASSI <NUM>
                  31: EXPR ARRAY arr[exp] <NUM>
                    31: EXPR ID j <TBD>
                  31: EXPR CALL inputnum(params) <NUM>
                    31: LIST ARG
              32: STMT EXPR
                32: EXPR OP ASSI <TBD>
                  32: EXPR ID j <TBD>
                  32: EXPR OP PLUS <TBD>
                    32: EXPR ID j <TBD>
                    32: EXPR NUM 1\1 <INT>
        33: STMT EXPR
          33: EXPR CALL printstr(params) <TBD>
            33: LIST ARG
              33: EXPR STR "The array is : " <STRING>
        34: STMT EXPR
          34: EXPR CALL print_num_array(params) <TBD>
            34: LIST ARG
              34: EXPR ID arr <ARRAY>
              34: EXPR NUM 5\1 <INT>
        35: STMT EXPR
          35: EXPR CALL printstr(params) <TBD>
            35: LIST ARG
              35: EXPR OP PLUS <STRING>
                35: EXPR OP PLUS <STRING>
                  35: EXPR STR "The maximum in the array is " <STRING>
                  35: EXPR CALL num2str(params) <STRING>
                    35: LIST ARG
                      35: EXPR CALL max(params) <TBD>
                        35: LIST ARG
                          35: EXPR ID arr <ARRAY>
                          35: EXPR NUM 5\1 <INT>
                35: EXPR STR "\n" <STRING>
        36: STMT EXPR
          36: EXPR OP ASSI <TBD>
            36: EXPR ID s <TBD>
            36: EXPR OP DIV <NUM>
              36: EXPR OP MUL <NUM>
                36: EXPR OP MINUS <NUM>
                  36: EXPR OP PLUS <NUM>
                    36: EXPR ARRAY arr[exp] <NUM>
                      36: EXPR NUM 0\1 <INT>
                    36: EXPR ARRAY arr[exp] <NUM>
                      36: EXPR NUM 1\1 <INT>
                  36: EXPR ARRAY arr[exp] <NUM>
                    36: EXPR NUM 2\1 <INT>
                36: EXPR ARRAY arr[exp] <NUM>
                  36: EXPR NUM 3\1 <INT>
              36: EXPR ARRAY arr[exp] <NUM>
                36: EXPR NUM 4\1 <INT>
        37: STMT EXPR
          37: EXPR CALL printstr(params) <TBD>
            37: LIST ARG
              37: EXPR OP PLUS <STRING>
                37: EXPR OP PLUS <STRING>
                  37: EXPR STR "(arr[0]+arr[1]-arr[2])*arr[3]/arr[4] = " <STRING>
                  37: EXPR CALL num2str(params) <STRING>
                    37: LIST ARG
                      37: EXPR ID s <TBD>
                37: EXPR STR "\n" <STRING>
        38: STMT EXPR
          38: EXPR CALL printstr(params) <TBD>
            38: LIST ARG
              38: EXPR STR "Bye" <STRING>
  41: STMT EXPR
    41: EXPR CALL driver(params) <TBD>
      41: LIST ARG
