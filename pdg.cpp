/*
 * @author:metastableB
 * pdg.cpp
 * 19 October 2015 (Monday) 
 */
 /* Assumptions 
 - { on the same line as if/else/while
 - {} alwas present for block closes
 - } always on differnt line
 */
#include "./pdg.h"

 PDG::PDG(std::string file_name){
 	valid = false;
 	in.open(file_name,std::ios::in);
 	if(in.fail()){
 		std::cout << "Could not open file\n" << file_name << std::endl;
 		return;
 	}
 	total_lines = 0;
 	if(makeProgram())
 		valid = true;
 	if(!allocate_g_cdg())
 		valid = false;
 }
 PDG::~PDG(){
 	in.close();
 	free(g_cdg);
 	free(g_ddg);
 }

 bool PDG::makeProgram(){
 	if(!valid) {
 		std::cout << "INTERNAL ERROR!\n";
 		return false;
 	}

 	std::string s;
 	int line_no = 0;
 	int scope = 0;
 	Line_Type type;
 	Line *temp;
 	while(in >> s){
 		type = get_line_type(s);
 		temp = (Line*) malloc(sizeof(Line));
 		program.push_back({line_no,scope,type});
 		switch(type){
 			case Line_Type::ASSIGNMENT:
 				break;
 			case Line_Type::IF:
 				scope++;
 				break;
 			case Line_Type::ELSE:
 				break;
 			case Line_Type::WHILE:
 				break;
 			case Line_Type::SCOPE_CLOSE:
 				scope--;
 				break;
 			default :
 				std::cout << "Unknown Line Type at L:" << line_no << std::endl;
 				return false;
 		}
 		line_no++;
 	}
 	total_lines = line_no;
 	if(scope != 0){
 		std::cout << "Scopes did not close properly\n";
 		return false;
 	}
 	in.seekg(0);
 	return true;
 }

 Line_Type PDG::get_line_type(std::string s){
 	decltype(s.find("")) f;
 	/* Check if string is an if */
 	if((f = s.find("if")) != std::string::npos){
 		if(s[f+2] == ' ' || s[f+2] == '(')
 			return Line_Type::IF;
 	} 	/* Check for else */
 	else if ((f = s.find("else"))!= std::string::npos){
 		if(s[f+4] == ' ' || s[f+4] == '(')
 			return Line_Type::ELSE;
 	} 	/* Checking for while */
 	else if ((f = s.find("while")) != std::string::npos){
 		if(s[f+5] == ' ' || s[f+5] == '(')
 			return Line_Type::WHILE;
 	} 	/* Checking for assignment */
 	else if((f = s.find("=")) != std::string::npos){
 		if(s[f+1]!= '=' && s[f-1] != '!')
 			return Line_Type::ASSIGNMENT;
 	} 	/* Checking for blocl close */
 	else if((f = s.find("}"))!= std::string::npos){
 		return Line_Type::SCOPE_CLOSE;
 	}
 	return ERROR;
 }

 bool PDG::allocate_g_cdg(){
 	/* Allocate a graph of total_lines^2 size to **g_gdg */
 	g_cdg = (int**)malloc(sizeof(int*) * total_lines);
 	if(g_cdg == nullptr) return false;
 	for(int i = 0; i < total_lines;i++){
 		g_cdg[i] = (int*) malloc(sizeof(int)*total_lines);
 		if(g_cdg[i] == nullptr) return false;
 	}
 	return true;
 }
 bool PDG::is_valid() {return valid;}
 void PDG::ddg(){}
 void PDG::cdg(){}