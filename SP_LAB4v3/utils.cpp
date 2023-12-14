//
//  utils.cpp
//  SP_LAB4v3
//
//  Created by Georgy Petriv on 04.11.2023.
//

#include "utils.hpp"

set<NonTerminal> FindEpsilonNonTerminals(Grammar &G){
    set<NonTerminal> previousEpsilonSet = {};
    set<NonTerminal> currentEpsilonSet = {};
    
    bool continueCycle = true;
    while (continueCycle){
        for (auto &rule : G.Rules){
            //cout << rule.lhs.value << endl;
            if (rule.IsEpsilon(previousEpsilonSet)){
                currentEpsilonSet.insert(rule.lhs);
            }
        }
        
        if (previousEpsilonSet == currentEpsilonSet){
            continueCycle = false;
        }
        
        previousEpsilonSet = currentEpsilonSet;
        currentEpsilonSet.clear();
    }
    
    return previousEpsilonSet;
}

set<NonTerminal> FindLeftRecursiveNonTerminals(Grammar &G, set<NonTerminal> EpsilonSet){
    set<NonTerminal> recursiveSet = {};

    for (auto& nonTern : G.NonTerminals){
        set<NonTerminal> previousLeftRecursiveSet = {nonTern};
        set<NonTerminal> currentLeftRecursiveSet = {};
        
        bool continueCycle = true;
        while (continueCycle){
            for (auto& rule : G.Rules){
                if (previousLeftRecursiveSet.count(rule.lhs)){
                    bool previousIsEpsilonNonTerminal = true;
                    for (auto& rightNonTerminal : rule.rhs){
                        if (previousIsEpsilonNonTerminal){
                            if (typeid(*rightNonTerminal) == typeid(NonTerminal)){
                                currentLeftRecursiveSet.insert(*dynamic_cast<NonTerminal*>(rightNonTerminal));
                                if (!EpsilonSet.count(*dynamic_cast<NonTerminal*>(rightNonTerminal))){
                                    previousIsEpsilonNonTerminal = false;
                                }
                            }
                            else{
                                previousIsEpsilonNonTerminal = false;
                            }
                        }
                    }
                }
            }
            
            if (previousLeftRecursiveSet == currentLeftRecursiveSet){
                continueCycle = false;
            }
            
            previousLeftRecursiveSet = currentLeftRecursiveSet;
            currentLeftRecursiveSet.clear();
        }
        
        if (previousLeftRecursiveSet.count(nonTern)){
            recursiveSet.insert(nonTern);
        }
    }
    
    return recursiveSet;
}

vector<Terminal> ConcatinateK(Terminal &t1, Terminal &t2, int k){
    vector<Terminal> res;
    if(!t1.IsEpsilon()){
        res.push_back(t1);
    }
    if(!t2.IsEpsilon()){
        res.push_back(t2);
    }
    if(res.size()==0){
        res.push_back(t1);
    }
    if(res.size() > k){
        res.resize(k);
    }
    return res;
}

vector<Terminal> ConcatinateK(vector<Terminal> &w1, vector<Terminal> &w2, int k){
    vector<Terminal> res;
    for(auto x:w1){
        if(!x.IsEpsilon()){
            res.push_back(x);
        }
    }
    for(auto x:w2){
        if(!x.IsEpsilon()){
            res.push_back(x);
        }
    }
    if(res.size()==0){
        res.push_back(Terminal());
    }
    if(res.size() > k){
        res.resize(k);
    }
    return res;
}

vector<Terminal> ConcatinateK(vector<Terminal> &w, Terminal &t, int k){
    vector<Terminal> res;
    for(auto x:w){
        if(!x.IsEpsilon()){
            res.push_back(x);
        }
    }
    if(!t.IsEpsilon()){
        res.push_back(t);
    }
    if(res.size()==0){
        res.push_back(Terminal());
    }
    if(res.size() > k){
        res.resize(k);
    }
    return res;
}

vector<Terminal> ConcatinateK(Terminal &t, vector<Terminal> &w, int k){
    vector<Terminal> res;
    if(!t.IsEpsilon()){
        res.push_back(t);
    }
    for(auto x:w){
        if(!x.IsEpsilon()){
            res.push_back(x);
        }
    }
    if(res.size()==0){
        res.push_back(Terminal());
    }
    if(res.size() > k){
        res.resize(k);
    }
    return res;
}

set<vector<Terminal>> ConcatinateK(set<vector<Terminal>> &set1, set<vector<Terminal>> &set2, int k){
    set<vector<Terminal>> res;
    for(auto x: set1){
        for(auto y: set2){
            res.insert(ConcatinateK(x, y, k));
        }
    }
    for(auto x:res){
        if(x.size() > k){
            x.resize(k);
        }
    }
    return res;
}

set<vector<Terminal>> ConcatinateK(vector<set<vector<Terminal>>> sets, int k){
    set<vector<Terminal>> res;
    if(sets.size()==0){
        return res;
    }
    vector<set<vector<Terminal>>>::iterator it = sets.begin();
    res = *it;
    it++;
    while(it!=sets.end()){
        res = ConcatinateK(res, *it, k);
        it++;
    }
    for(auto x:res){
        if(x.size() > k){
            x.resize(k);
        }
    }
    return res;
}

bool check_rhs(vector<Symbol*> w, map<NonTerminal, set<vector<Terminal>>> &Firsts){
    for (auto Symbol : w){
        if (typeid(*Symbol) == typeid(NonTerminal)){
            if (Firsts[*dynamic_cast<NonTerminal*>(Symbol)].size() == 0){
                return false;
            }
        }
    }
    return true;
}

set<vector<Terminal>> FirstK(const vector<Symbol*>& w, map<NonTerminal, set<vector<Terminal>>> &Firsts, int k){
    vector<set<vector<Terminal>>> sets_vector;
    set<vector<Terminal>> temporar_set;
    
    for (auto Symbol : w){
        if (typeid(*Symbol) == typeid(Terminal)){
            temporar_set.insert(vector<Terminal>(1, *dynamic_cast<Terminal*>(Symbol)));
            sets_vector.push_back(temporar_set);
            temporar_set.clear();
        }
        else{
            sets_vector.push_back(Firsts[*dynamic_cast<NonTerminal*>(Symbol)]);
        }
    }
    
    return ConcatinateK(sets_vector, k);
}

map<NonTerminal, set<vector<Terminal>>> FirstK(Grammar &G, int k){
    map<NonTerminal, set<vector<Terminal>>> previousFirstK;
    map<NonTerminal, set<vector<Terminal>>> currentFirstK;
    for (auto nonTerm : G.NonTerminals){
        previousFirstK[nonTerm] = {};
        currentFirstK[nonTerm] = {};
    }
    
    for (auto& rule: G.Rules){
        if ((rule.rhs.size() == 1) && (typeid(*rule.rhs[0]) == typeid(Terminal))){
            previousFirstK[rule.lhs].insert(vector(1, *dynamic_cast<Terminal*>(rule.rhs[0])));
        }
    }
    
    bool continueCycle = true;
    while(continueCycle){
        for (auto& rule: G.Rules){
            if (check_rhs(rule.rhs, previousFirstK)){
                set<vector<Terminal>> concationstion = FirstK(rule.rhs, previousFirstK, k);
                for (auto& elem : concationstion){
                    currentFirstK[rule.lhs].insert(elem);
                }
            }
        }
        
        if (previousFirstK == currentFirstK){
            continueCycle = false;
        }
        
        previousFirstK = currentFirstK;
        for (auto nonTerm : G.NonTerminals){
            currentFirstK[nonTerm] = {};
        }
        
    }
    
    return previousFirstK;
}

map<NonTerminal, set<vector<Terminal>>> FollowK(Grammar &G, map<NonTerminal, set<vector<Terminal>>> First_K, int k){
    map<NonTerminal, set<vector<Terminal>>> previousFollowK;
    map<NonTerminal, set<vector<Terminal>>> currentFollowK;
    
    set<NonTerminal> Nonterm_rules_previuos = {G.Axiom};
    set<NonTerminal> Nonterm_rules_current = {};
    
    vector<Symbol*> temporar_concat;
    for (auto nonTerm : G.NonTerminals){
        previousFollowK[nonTerm] = {};
        currentFollowK[nonTerm] = {};
    }
    previousFollowK[G.Axiom].insert(vector(1, Terminal()));
    currentFollowK[G.Axiom].insert(vector(1, Terminal()));
    
    bool continueCycle = true;
    while (continueCycle){
        for (auto NonTerm: Nonterm_rules_previuos){
            for (auto& rule: G.Rules){
                if (rule.lhs == NonTerm){
                    bool add_out = true;
                    for (int i = 0; i < rule.rhs.size(); i++){
                        if (typeid(*rule.rhs[i]) == typeid(NonTerminal)){
                            if (add_out){
                                Nonterm_rules_current.insert(*dynamic_cast<NonTerminal*>(rule.rhs[i]));
                                add_out = false;
                            }
                            temporar_concat.insert(temporar_concat.end(), rule.rhs.begin() + i + 1, rule.rhs.end());
                            
                            set<vector<Terminal>> first;
                            first = FirstK(temporar_concat, First_K, k);
                            set<vector<Terminal>> concatination = {};
                            if (first.size() == 0){
                                concatination = previousFollowK[rule.lhs];
                                for(auto x:concatination){
                                    if(x.size() > k){
                                        x.resize(k);
                                    }
                                }
                            }
                            else{
                                concatination = ConcatinateK(first, previousFollowK[rule.lhs], k);
                            }
                            for (auto& elem : concatination){
                                currentFollowK[*dynamic_cast<NonTerminal*>(rule.rhs[i])].insert(elem);
                            }
                            
                            temporar_concat.clear();
                            first.clear();
                            
                        }
                    }
                }
            }
        }
        
        if (previousFollowK == currentFollowK){
            continueCycle = false;
        }
        for (auto& row : currentFollowK){
            for (auto& symbol : row.second){
                previousFollowK[row.first].insert(symbol);
            }
        }
        Nonterm_rules_previuos = Nonterm_rules_current;
        Nonterm_rules_current.clear();
    }
    
    return previousFollowK;
}
