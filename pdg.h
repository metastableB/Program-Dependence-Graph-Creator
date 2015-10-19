#ifndef _PDG_H_
#define _PDG_H_

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
#include <climits>

enum Line{
	ASSIGNMENT,
	IF,
	ELSE,
	WHILE
};
class PDG{
	bool valid;
	int* program;
	int* CDG;
	int* DDG;
	
	void makeProgram();
public:
	void CDG();
	void DDG();
};
#endif // _PDG_H_