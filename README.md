# PDG : Program Dependency Graph

A program dependency graph creator for a very basic imperative program defined by the following rules.

```
	assignment := x=Aexpr;
    conditional1 := if(Bexpr){ stmts; } else {stmts; } endif
    conditional2 := if(Bexpr){ stmts; } endif
    iterative := while(Bexpr){stmts; }
    stmt := assignment | conditionsal1 | conditional2 | iterative
    stmts := stmt | stmts; stmt
    Aexpr: = constant | variable | Aexpr + Aexpr | Aexpr – Aexpr | Aexpr * Aexpr | Aexpr / Aexpr | (Aexpr)
    Bexpr := Aexpr == Aexpr | Aexpr <= Aexpr | Aexpr >= Aexpr | Aexpr < Aexpr | Aexpr > Aexpr
```

The program strictly assumes the above rules. 
### Installation and Usage
This has only been tested on Linux. Other operating systems most probably wont be able to build the files, though porting should be easy to major platforms. The only OS dependent part of the program is the graph plotting functions `PDG::cdg_plot()` and `PDG::gdg_plot()` calling system resources to execute the python script.

First make sure you have [netowrkX]( https://networkx.github.io/) on your computer. Installation instructions are given [here]( https://networkx.github.io/documentation/latest/install.html)

After you have installed `networkX`, download or clone these source files.
In the source directory run,
```
$ make
```
This will compile and build your files. The `Makefile` is setup to use `clang++` for compilation, you can switch to `g++` or any compiler of your choice by redefining the `CC` variable in the `Makefile` (line 5).

To run, type
```
$ ./pdg_cs17 [FILENAME]
```
where **FILENAME** is the path to a file containing the program to be processed.

### Notes
##### <u>Limitations</u>
The current version is just a prototype and does not entirely cover all cases even for the above rule structure. Though this can be easily implemented by modifying the `PDG::cdg()` and `PDG::ddg()` functions defined in `pdg.h` and `pdg.cpp`.

Further, due to some ambiguity in the above rules, the following has been assumed (the list, most definitely, is not exhaustive).

 * Assumptions  PDG
	 - `{` on the same line as `if/else/while`
	 - `{}` always present for inner blocks
	 - `}` always on different line
	 - No function definitions/declarations allowed
 * Assumptions DDG
 	- Since data types are not defined, we assume the left
 	  side of every `=` sign as a variables.
 	- Since scope rules are not defined, we assume that no variable
 	  re-declaration is allowed and variables are declared as
 	  they are used. Variable scope is assumed to be till the end of a file.
 	- Multiple assignments in the same statement are not allowed
 	- all variables have to be initialized before use
 	
The graph plotting currently uses the python `networkX` library. The graph to be plotted is sent to the `simplePlot.py` program in `networkX` upon request. 

Further `networkX` does not allow for tree style plots as of now.

##### <u>TODO</u>
- [ ] `while` loop dependencies are not covered
- [ ]  Move on to `graphviz` library for a tree structured plot and other enhancements
- [ ]  Command line arguments

**&copy; 2015 Don Dennis (metastableB)**