//
//  grammar.cpp
//  SP_LAB4v3
//
//  Created by Georgy Petriv on 03.11.2023.
//

#include "grammar.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <cctype>

using namespace std;

//Symbol implementation
bool Symbol::operator<(const Symbol &x)const{
    return value<x.value;
}
bool Symbol::operator==(const Symbol &x)const{
    return value==x.value;
}
//Terminal implementation
string Terminal::get_string(){
    if(value=="$"){
        return "";
    }
    return value;
}
//Rule implementation
bool Rule::operator<(const Rule &x) const{
    if(lhs!=x.lhs){
        return lhs<x.lhs;
    }
    else{
        string s1,s2;
        for(auto &S: rhs){
            s1+=(*S).value;
        }
        for(auto &S: x.rhs){
            s2+=(*S).value;
        }
        return s1<s2;
    }
}
bool Rule::Contains(Symbol &x){
    for(auto &s: rhs){
        if(*s == x){
            return true;
        }
    }
    return false;
}
bool Rule::IsEpsilon(set<NonTerminal> EpsilonSet) const{
    if((*rhs[0]).IsEpsilon()){
        return true;
    }
    bool check = true;
    for(auto &s: rhs){
        if((typeid(*s))==typeid(NonTerminal)){
            if(!EpsilonSet.count(*dynamic_cast<NonTerminal*>(s))){
                check = false;
            }
        }
        else{
            check = false;
        }
    }
    return check;
}

