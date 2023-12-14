//
//  utils.hpp
//  SP_LAB4v3
//
//  Created by Georgy Petriv on 04.11.2023.
//
#include "grammar.hpp"
#ifndef utils_hpp
#define utils_hpp

using namespace std;

#include <stdio.h>

#endif /* utils_hpp */
set<NonTerminal> FindEpsilonNonTerminals(Grammar &G);
set<NonTerminal> FindLeftRecursiveNonTerminals(Grammar &G, set<NonTerminal> epsilonSet);

vector<Terminal> ConcatinateK(Terminal &t1, Terminal &t2, int k);
vector<Terminal> ConcatinateK(vector<Terminal> &w1, vector<Terminal> &w2, int k);
vector<Terminal> ConcatinateK(vector<Terminal> &w, Terminal &t, int k);
vector<Terminal> ConcatinateK(Terminal &t, vector<Terminal> &w, int k);
set<vector<Terminal>> ConcatinateK(set<vector<Terminal>> &set1, set<vector<Terminal>> &set2, int k);
set<vector<Terminal>> ConcatinateK(vector<set<vector<Terminal>>> sets, int k);



map<NonTerminal, set<vector<Terminal>>> FirstK(Grammar &G, int k);
map<NonTerminal, set<vector<Terminal>>> FollowK(Grammar &G, map<NonTerminal, set<vector<Terminal>>> First_K, int k);
set<vector<Terminal>> FirstKSet(const vector<Symbol*> &w, map<NonTerminal, set<vector<Terminal>>> &Firsts, int k);
