//
//  main.cpp
//  Lab2-SP
//
//  Created by vika on 06.10.2023.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <set>

using namespace std;

string is_end_state(string state, unordered_set<int> accept_states){
    for (auto end: accept_states){
        if (state == to_string(end)){
            return to_string(end);
        }
    }
    return "";
}

vector<vector<string>> epsilon_closure(unordered_set<int> accept_states, vector<vector<string>>& results, vector<vector<string>> ongoing, vector<vector<string>> transitions){
    vector<vector<string>> current;
    for (auto vector_ongoing: ongoing){
        for (auto vector_transit: transitions){
            if (vector_ongoing[0] == vector_transit[0]){
                string check = is_end_state(vector_transit[2], accept_states);
                if (check != ""){
                    results.push_back({vector_ongoing[1]+vector_transit[1], check, vector_transit[0], vector_ongoing[1]});
                }
                else{
                    current.push_back({vector_transit[2], vector_ongoing[1]+vector_transit[1], vector_transit[0], vector_ongoing[1]});
                }
            }
        }
    }
    return current;
}

vector<vector<string>> simulate(vector<vector<string>> transitions,
              int start_state, unordered_set<int> accept_states, bool print_data)
{
    vector<vector<string>> results;
    vector<vector<string>> ongoing = {{to_string(start_state), ""}};
    
    while (ongoing.size()){
        ongoing = epsilon_closure(accept_states, results, ongoing, transitions);
        if (print_data){
            for (auto vector: ongoing){
                cout << "Start state: " << vector[2] << ", word: " << vector[3] << " ---> "
                << "New state: " << vector[0] << " word: " << vector[1] << endl;
            }
            cout << endl;
        }
    }
    
    if (print_data){
        for (auto vector: results){
            cout << "Start state: " << vector[2] << ", word: " << vector[3] << " ---> "
            << "End state: " << vector[1] << " word: " << vector[0] << endl;
        }
        cout << endl;
    }
    
    return results;
}

bool check_input(int alph_size, int st_size, set<string> alph, set<string> st){
    if ((alph.size()-1 != alph_size) || (st.size()-1 != st_size)){
        return false;
    }
    return true;
}

vector<string> getChars(string s){
    vector<string> res;
    int pos = 0;
    while(pos < s.size()){
        pos = s.find(" ");
        res.push_back(s.substr(0,pos));
        s.erase(0,pos+1);
    }
    res.push_back(s.substr(0, s.size()));
    return res;
}

void getData(ifstream& file, int& alphabet_size, int& states_size,
             vector<vector<string>>& transitions,
             int& start_state, unordered_set<int>& accept_states,
             set<string>& alphabet, set<string>& states){
    
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string keyword;
        iss >> keyword;

        if (keyword == "Alphabet_size:") {
            iss >> alphabet_size;
        } else if (keyword == "States_size:") {
            iss >> states_size;
        } else if (keyword == "Start_state:") {
            iss >> start_state;
        } else if (keyword == "Final_states:") {
            int fin_states_number;
            iss >> fin_states_number;

            int accept_state;
            while (iss >> accept_state) {
                accept_states.insert(accept_state);
            }
        } else if (keyword == "Transitions:") {
            while (getline(file, line)){
                vector<string> data = getChars(line);
                transitions.push_back(data);
                states.insert(data[0]);
                states.insert(data[2]);
                alphabet.insert(data[1]);
            }
        }
    }
}

vector<string> get_intersectiron(vector<vector<string>> results1, vector<vector<string>> results2){
    vector<string> res;
    for (auto vector1: results1){
        for (auto vector2: results2){
            if (vector1[0] == vector2[0]){
                res.push_back(vector1[0]);
            }
        }
    }
    return res;
}

long get_minimal_word_length(vector<string> words){
    long min = words[0].size();
    for (auto word: words){
        if (word.size() < min){
            min = word.size();
        }
    }
    return min;
}

void task10(){
    ifstream file1("/Users/vika/Documents/XCode_projs/Lab2-SP/Lab2-SP/data.txt");
    ifstream file2("/Users/vika/Documents/XCode_projs/Lab2-SP/Lab2-SP/data10.txt");
    
    int alphabet_size1, alphabet_size2;
    int states_size1, states_size2;
    vector<vector<string>> transitions1, transitions2;
    int start_state1, start_state2;
    unordered_set<int> accept_states1, accept_states2;
    
    set<string> alphabet1, alphabet2;
    set<string> states1, states2;

    
    getData(file1, alphabet_size1, states_size1, transitions1, start_state1, accept_states1, alphabet1, states1);
    getData(file2, alphabet_size2, states_size2, transitions2, start_state2, accept_states2, alphabet2, states2);
    
    if (!check_input(alphabet_size1, states_size1, alphabet1, states1) || !check_input(alphabet_size2, states_size2, alphabet2, states2)){
        cout << "Incorrect data provided" << endl;
        return;
    }
    
    vector<vector<string>> results1 = simulate(transitions1, start_state1, accept_states1, false);
    vector<vector<string>> results2 = simulate(transitions2, start_state2, accept_states2, false);
    
    vector<string> intersection = get_intersectiron(results1, results2);
    long size = get_minimal_word_length(intersection);
    
    if (intersection.size() == 0){
        cout << "No words recognised by both automatons" << endl;
        return;
    }
    
    cout << "Words with minimal length recognised by both automatons" << endl;
    for (auto word: intersection){
        if (word.size() == size){
            cout << word << " ";
        }
    }
    cout << endl;
}

void task3(){
    ifstream file("/Users/vika/Documents/XCode_projs/Lab2-SP/Lab2-SP/data.txt");
    
    int alphabet_size;
    int states_size;
    vector<vector<string>> transitions;
    int start_state;
    unordered_set<int> accept_states;
    
    set<string> alphabet;
    set<string> states;
    
    getData(file, alphabet_size, states_size, transitions, start_state, accept_states, alphabet, states);
    
    if (!check_input(alphabet_size, states_size, alphabet, states)){
        cout << "Incorrect data provided" << endl;
        return;
    }
    
    vector<vector<string>> results = simulate(transitions, start_state, accept_states, true);
    
    cout << "Results" << endl;
    for (auto word: results){
        cout << word[0] << endl;
    }
    cout << endl;
}

int main(int argc, const char * argv[]) {
    
    cout << "Task3" << endl;
    task3();
    
    cout << "Task10" << endl;
    task10();
}
