//
//  grammar.hpp
//  SP_LAB4v3
//
//  Created by Georgy Petriv on 03.11.2023.
//

#ifndef grammar_hpp
#define grammar_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <cctype>


using namespace std;

#endif /* grammar_hpp */

class Symbol{
public:
    string value;
    Symbol(): value("$"){};
    Symbol(string s): value(s){};
    virtual bool IsEpsilon(){ if(value=="$"){return true;}return false;};
    bool operator ==(const Symbol &x) const;
    bool operator<(const Symbol &x) const;
};
class Terminal : public Symbol {
public:
    bool IsEpsilon(){ if(value=="$"){return true;}return false;};
    Terminal(){
        value="$";
    };
    Terminal(char c){
        if(tolower(c)==c){
            value = {c};
        }
        else{
            //cout << "Error: invalid character assigned to terminal\n";
            throw 2;
        }
    };
    string get_string();
    
};

class NonTerminal : public Symbol {
public:
    bool IsEpsilon(){return false;};
    NonTerminal(){
        value = "$";
    };
    NonTerminal(char c){
        if(int(c)>64 && int(c)<91){
            value = {c};
        }
        else{
            //cout << "Error: invalid character assigned to nonterminal!!!\n";
            throw 1;
        }
    };

};
class Rule {
public:
    NonTerminal lhs;
    vector<Symbol*> rhs;
    Rule(NonTerminal T, vector<Symbol*> &R){
        lhs = T;
        rhs = vector<Symbol*>(R.begin(), R.end());// ! ! !
        R.clear();
    };
    Rule() = default;
    Rule(const Rule & other) = default;
    Rule& operator=(const Rule & other)= default;
    Rule(Rule && other){
        lhs=other.lhs;
        rhs = vector<Symbol*>(other.rhs.begin(), other.rhs.end());// ! ! !
        other.rhs.clear();
    }
    bool Contains(Symbol &x);
    bool IsEpsilon(set<NonTerminal> EpsilonSet) const;
    bool operator<(const Rule &x) const;
    ~Rule(){
        for(auto x: rhs){
            delete x;
        }
    };
};

class Grammar{
public:
    set<Terminal> Terminals;
    set<NonTerminal> NonTerminals;
    set<Rule> Rules;
    NonTerminal Axiom;
    Grammar(ifstream& input){
        string strline;
        int k = 1;
        while(getline(input, strline)){
            stringstream line(strline);
            char curr;
            line >> curr;
            NonTerminal A(curr);
            NonTerminals.insert(A);
            vector<Symbol*> temp;
            if(k++==1){
                Axiom=A;
            }
            line >> curr; //skipping '-'
            while(line >> curr){
                if(curr!='|'){
                    if(64< curr && curr <91){
                        NonTerminal* x = new NonTerminal(curr);
                        temp.push_back(x);
                        NonTerminals.insert(*x);
                    }
                    else{
                        Terminal* x = new Terminal(curr);
                        temp.push_back(x);
                        Terminals.insert(*x);
                    }
                }
                else{
                    Rules.insert(Rule(A, temp));
                    temp.clear();
                }
            }
            Rules.insert(Rule(A, temp));
        }
    };
    };
