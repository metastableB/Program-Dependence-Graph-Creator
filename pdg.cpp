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
 	} else valid = true;
 	total_lines = 0;
 	if(makeProgram())
 		valid = true;
 	else return;
 	if(!allocate_g_cdg())
 		valid = false;
 }
 PDG::~PDG(){
 	in.close();
 	free(g_cdg);
 	//free(g_ddg);
 }

 bool PDG::makeProgram(){
 	if(!valid) {
 		std::cout << "INTERNAL ERROR!\n";
 		return false;
 	}

 	std::string s;
 	int line_no = 1;
 	int scope = 0;
 	Line_Type type;
 	Line *temp;
 	program.push_back({0,0,BEGIN});
 	while(std::getline(in,s)){
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
 				scope++;
 				break;
 			case Line_Type::WHILE:
 				scope++;
 				break;
 			case Line_Type::SCOPE_CLOSE:
 				scope--;
 				(--program.end())->scope -= 1;
 				break;
 			default :
 				std::cout << "Unknown Line Type at L:" << line_no << "\""+s+"\""<<std::endl;
 				return false;
 		}
 		//std::cout << "L " << line_no << " scope " << (--program.end())->scope << " type " << type << std::endl;
 		line_no++;
 	}
 	total_lines = line_no - 1;
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
 	/* Allocate a graph of total_lines^2 size to **g_cdg */
 	g_cdg = (int**)malloc(sizeof(int*) * (total_lines + 1));
 	if(g_cdg == nullptr) return false;
 	for(int i = 0; i < total_lines;i++){
 		g_cdg[i] = (int*) malloc(sizeof(int)*(total_lines+1));
 		if(g_cdg[i] == nullptr) return false;
 	}
 	return true;
 }
 bool PDG::is_valid() {return valid;}
 void PDG::ddg(){}
 
 void PDG::cdg(){
 	bool added[total_lines+1];
 	for(int i = 0;i < total_lines + 1; added[i++]=false);
 	for(auto i = program.begin();i!= program.end();i++){
 		if(added[i->line_no]) continue;
 		else if(i->type == Line_Type::BEGIN){
 			for(auto j = i + 1; j != program.end(); j++){
 				if(j->scope == i->scope && j->type != Line_Type::ELSE){
 					added[j->line_no] = true;
 					g_cdg[i->line_no][j->line_no] = 1;
 				}
 			}		
 		} else{
 			/* First non added statement. Find the sub block
 			 * this statement belongs to and, evaluate the
 			 * truth of that statement and for that value
 			 * add this to the node
 			 */
 			 auto j = i - 1;
 			 for(; j!= program.begin();j--)
 			 	if(j->type ==Line_Type::IF || j->type==Line_Type::ELSE || j->type == Line_Type::WHILE) break;
 			 if(j== program.begin()){
 			 	std::cout << "WOW WOW! Something weird happened\n";
 			 	return ;
 			 }
 			 auto k = j;
 			 int tr = 1;
 			 if(k->type == Line_Type::ELSE) {
 			 	tr = 2;
 			 	for(; k!=program.begin();k--)
 			 		if(k->type == Line_Type::IF) break;
 			 }
 			 j++;
 			 for(;j !=program.end();j++){
 			 	if(added[j->line_no]) break;
 			 	if(j->scope == k->scope + 1 && j->type != Line_Type::ELSE){
 			 		added[j->line_no] = true;
 			 		g_cdg[k->line_no][j->line_no] = tr;
 			 	}
 			 }
 		}
 	}
 	print_cdg(); 	
 }

 void PDG::print_cdg(){
 	std::printf("CDG\n   ");
 	for(int i = 0; i <= total_lines;i++)
 		std::printf("%3d ",i);
 	std::cout << std::endl;
 	for(int i = 0; i < total_lines;i++){
 		std::printf("%3d",i);
 		for(int j = 0; j <= total_lines; j++){
 			std::printf("%3d ",g_cdg[i][j]);
 		}
 		std::cout << std::endl;
 	}
}