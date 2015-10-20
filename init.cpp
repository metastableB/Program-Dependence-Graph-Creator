/*
 * @author:metastableB
 * 19 October 2015 (Monday)
 * 
 * Program Dependecy Graph Generator
 * 
 */
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
#include <climits>

#include "pdg.h"

int print_exit(){
	std::cout << "CRITICAL ERROR\n";
	return 1;
}
int main(int argc, char** argv) {
	if(argc != 2){
		std::cout << "Usage ./pdg_cs17 [FileName]\n";
		return 1;
	}
	PDG p(argv[1]);
	if(!p.is_valid()) return print_exit();
	p.cdg();
	p.print_cdg();
	p.plot_cdg();
	p.ddg();
	if(!p.is_valid()) return print_exit();
	p.print_ddg();
	p.plot_ddg();
    return 0;
}
