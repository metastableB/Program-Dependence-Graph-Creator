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
	ERROR,
	BEGIN
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
	/* Line vise disection of the program
	 * Each input program line is converted to an object
	 * of struct Line 
	 */
	std::vector<Line> program_map;
	/*
	 * Variable name, last line of assignment
	 */
	std::map<std::string,int> variable_map;

	int **g_cdg, **g_ddg;
	Line_Type get_line_type(std::string s);
	bool make_program_map();
	bool make_variable_map();
	bool allocate_g_cdg();
	bool allocate_g_ddg();
	
	std::vector<std::string> get_var(std::string s,struct Line it);
public:
	~PDG();
	PDG(std::string s);
	bool is_valid();
	
	void cdg();
	void print_cdg();
	void plot_cdg();
	
	void ddg();
	void print_ddg();
	void plot_ddg();


};
#endif // _PDG_H_