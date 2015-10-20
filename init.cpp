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

int main(int argc, char** argv) {
	if(argc != 2){
		std::cout << "Usage ./pdg_cs17 [FileName]\n";
		return 1;
	}
	PDG p(argv[1]);
	if(!p.is_valid()) return 1;
	p.cdg();
	p.ddg();
    return 0;
}
