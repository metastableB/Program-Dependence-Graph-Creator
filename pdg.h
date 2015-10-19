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
#include <fstream>
enum Line_Type{
	ASSIGNMENT,
	IF,
	ELSE,
	WHILE,
	SCOPE_CLOSE,
	ERROR
};
struct Line{
	int line_no;
	int scope;
	enum Line_Type type;
};
class PDG{
	std::fstream in;
	bool valid;
	int total_lines;
	std::vector<Line> program;

	int **g_cdg, **g_ddg;
	Line_Type get_line_type(std::string s);
	bool makeProgram();
	bool allocate_g_cdg();
public:
	~PDG();
	PDG(std::string s);
	bool is_valid();
	void cdg();
	void ddg();

};
#endif // _PDG_H_