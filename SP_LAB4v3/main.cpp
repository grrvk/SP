//
//  main.cpp
//  SP_LAB4v3
//
//  Created by Georgy Petriv on 03.11.2023.
//

#include <iostream>
#include "utils.hpp"
#include "output.h"

using namespace std;

int main() {
    int k = 2;
    set<NonTerminal> epsilonNonTerminals;
    set<NonTerminal> leftRecursiveNonTerminals;
    map<NonTerminal, set<vector<Terminal>>> FirstKTerminals;
    map<NonTerminal, set<vector<Terminal>>> FollowKTerminals;
    
    ifstream stream("input.txt");
    Grammar G(stream);
    
    cout << "EPSILON NONTERMINALS" << endl;
    epsilonNonTerminals = FindEpsilonNonTerminals(G);
    NonTerminals_set_otput(epsilonNonTerminals);
    cout << endl << endl;
    
    cout << "RECURSIVE NONTERMINALS" << endl;
    leftRecursiveNonTerminals = FindLeftRecursiveNonTerminals(G, epsilonNonTerminals);
    NonTerminals_set_otput(leftRecursiveNonTerminals);
    cout << endl << endl;
    
    cout << "FIRST_K" << endl;
    FirstKTerminals = FirstK(G, k);
    Map_output(FirstKTerminals);
    cout << endl << endl;
    
    cout << "FOLLOW_K" << endl;
    FollowKTerminals = FollowK(G, FirstKTerminals, k);
    Map_output(FollowKTerminals);
    return 0;
}
