/*
 * @author:metastableB
 * pdg.cpp
 * 19 October 2015 (Monday) 
 *
 * Please read README.md before any usage.
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
 	if(make_program_map())
 		valid = true;
 	else return;
 	if(!allocate_g_cdg())
 		valid = false;
 }
 PDG::~PDG(){
 	in.close();
 	free(g_cdg);
 	free(g_ddg);
 }

 bool PDG::make_program_map(){
 	if(!valid) {
 		std::cout << "INTERNAL ERROR!\n";
 		return false;
 	}

 	std::string s;
 	int line_no = 1;
 	int scope = 0;
 	Line_Type type;
 	Line *temp;
 	program_map.push_back({0,0,BEGIN});
 	while(std::getline(in,s)){
 		type = get_line_type(s);
 		temp = (Line*) malloc(sizeof(Line));
 		program_map.push_back({line_no,scope,type});
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
 				(--program_map.end())->scope -= 1;
 				break;
 			default :
 				std::cout << "Unknown Line Type at L:" << line_no << "\""+s+"\""<<std::endl;
 				return false;
 		}
 		//std::cout << "L " << line_no << " scope " << (--program_map.end())->scope << " type " << type << std::endl;
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
 	for(int i = 0; i <= total_lines;i++){
 		g_cdg[i] = (int*) malloc(sizeof(int)*(total_lines+1));
 		for(int j = 0;j <= total_lines;j++) g_cdg[i][j] = 0;
 		if(g_cdg[i] == nullptr) return false;
 	}
 	return true;
 }
 bool PDG::allocate_g_ddg(){
 	/* Allocate a graph of total_lines^2 size to **g_cdg */
 	g_ddg = (int**)malloc(sizeof(int*) * (total_lines+1));
 	if(g_ddg == nullptr) return false;
 	for(int i = 0; i <= total_lines;i++){
 		g_ddg[i] = (int*) malloc(sizeof(int)*(total_lines+1));
 		for(int j = 0;j <= total_lines;j++) g_ddg[i][j] = 0;
 		if(g_ddg[i] == nullptr) return false;
 	}
 	return true;
 }
 bool PDG::is_valid() {return valid;}
 
 void PDG::cdg(){
 	bool added[total_lines+1];
 	for(int i = 0;i < total_lines + 1; added[i++]=false);
 	for(auto i = program_map.begin();i!= program_map.end();i++){
 		if(added[i->line_no]) continue;
 		else if(i->type == Line_Type::BEGIN){
 			for(auto j = i + 1; j != program_map.end(); j++){
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
 			 for(; j!= program_map.begin();j--)
 			 	if(j->type ==Line_Type::IF || j->type==Line_Type::ELSE || j->type == Line_Type::WHILE) break;
 			 if(j== program_map.begin()){
 			 	std::cout << "WOW WOW! Something weird happened\n";
 			 	return ;
 			 }
 			 auto k = j;
 			 int tr = 1;
 			 if(k->type == Line_Type::ELSE) {
 			 	tr = 2;
 			 	for(; k!=program_map.begin();k--)
 			 		if(k->type == Line_Type::IF) break;
 			 }
 			 j++;
 			 for(;j !=program_map.end();j++){
 			 	if(added[j->line_no]) break;
 			 	if(j->scope == k->scope + 1 && j->type != Line_Type::ELSE){
 			 		added[j->line_no] = true;
 			 		g_cdg[k->line_no][j->line_no] = tr;
 			 	}
 			 }
 		}
 	}
 }

void PDG::print_cdg(){
 	std::printf("CDG\n   ");
 	for(int i = 0; i <= total_lines;i++)
 		std::printf("%3d ",i);
 	std::cout << std::endl;
 	for(int i = 0; i <= total_lines;i++){
 		std::printf("%3d",i);
 		std::cout.flush();
 		for(int j = 0; j <= total_lines; j++){
 			std::printf("%3d ",g_cdg[i][j]);
 			std::cout.flush();
 		}
 		std::cout << std::endl;
 	}
}

void PDG::plot_cdg(){
	std::string edges;
	for(int i = 0; i <= total_lines;i++)
		for(int j = 0; j <= total_lines;j++)
			if(g_cdg[i][j] != 0)
				edges += "("+std::to_string(i) + "," + std::to_string(j) + ")|";
	std::cout << edges << std::endl;
	std::string command = "python ./networkX/simplePlot.py \'" + edges + "\'";
	system(command.c_str());
}
void PDG::ddg(){
	if(!allocate_g_ddg())
 		valid = false;
 	if(make_variable_map())
 		valid = true;
 }

bool PDG::make_variable_map(){
	if(!valid) { std::cout << "INTERNAL ERROR! EXITING\n"; return false;}
	std::string s;
	//std::cout << "RUNNING";
	in.clear();
	in.seekg(0,std::ios::beg);
	for(int i = 1; i <= total_lines; i++){
		/*
		 * If its an assignment statement. Update the assignment value
		 * else check any variable has been used
		 * this variable should be defined previously
		 * update dependency of this graph
		 */
		 getline(in,s);
		 //std::cout << s << std::endl;
		 auto &it = program_map[i];
		 auto variables = get_var(s,it);
		 if(variables.empty()) continue;
		 //std::cout << "got " << variables[0] << std::endl;
		 if(it.type == Line_Type::ASSIGNMENT){
		 	variable_map[variables[0]] = it.line_no;
		 	auto i = variables.begin();
		 	i++;
		 	while(i!= variables.end()) {g_ddg[it.line_no][variable_map[*i]] = 1;i++;}
		 } else {
		 	for(auto v : variables)
		 		g_ddg[it.line_no][variable_map[v]] = 1;
		 }
	}
	return true;
}

std::vector<std::string>
PDG::get_var(std::string s, struct Line it) {
	std::vector<std::string> var;
	/*
	 * If an assignment statement,
	 * the string to the left of an = is a variable.
	 * else search for existing variables in the string
	 * make sure they are not substrings and add tit */
	 s= " " + s + " ";
	 int pos = -1;
	 if(it.type == Line_Type::SCOPE_CLOSE) return var;
	 else if(it.type == Line_Type::ASSIGNMENT){
	 	//std::cout << "s" << s <<std::endl;
	 	int m = s.find("=");
	 	pos = m;
	 	m -= 1;
	 	int cnt = 0;
	 	while(m>=0 && std::isspace(s[m])) m--;
	 	while(m>=0 && std::isalnum(s[m])) {m--;cnt++;}
	 	if(m < 0){ 
	 		std::cout << "ERROR Extracting variable " << 
	 	 			"from assignment L : " << it.line_no << std::endl;
	 	 	return var;
	 	 }
	 	 var.push_back(s.substr(++m,cnt));
	 } 
	 for(auto v:variable_map){
	 	while((pos = s.find(v.first,pos+1)) != std::string::npos) {
	 		/*
	 		 * we might have a match. This can be a
	 		 * false positive as well.
	 		 */
	 		 if(!std::isalnum(s[pos-1]) && !std::isalnum(s[pos+v.first.length()])){
	 		 	/* WE have a variable people! */
	 		 	var.push_back(v.first);
	 		 	break;
	 		 }
	 	}
	 }
	 return var;
}

void PDG::print_ddg(){
 	std::printf("\nDDG\n   ");
 	for(int i = 1; i <= total_lines;i++)
 		std::printf("%3d ",i);
 	std::cout << std::endl;
 	for(int i = 1; i <= total_lines;i++){
 		std::printf("%3d",i);
 		std::cout.flush();
 		for(int j = 1; j <= total_lines; j++){
 			std::printf("%3d ",g_ddg[i][j]);
 			std::cout.flush();
 		}
 		std::cout << std::endl;
 	}
}

void PDG::plot_ddg(){
	std::string edges;
	for(int i = 1; i <= total_lines;i++)
		for(int j = 1; j <= total_lines;j++)
			if(g_ddg[i][j] != 0){
				if(i < j)
					edges += "("+std::to_string(i) + "," + std::to_string(j) + ")|";
				else
					edges += "("+std::to_string(i) + "," + std::to_string(j) + ")|";
			}
	std::cout << edges << std::endl;
	std::string command = "python ./networkX/simplePlot.py \'" + edges + "\'";
	system(command.c_str());
}