//
//  main.cpp
//  Lab1-SP
//
//  Created by vika on 25.09.2023.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;


int isVowel(char ch)
{
    string str = "aeiouAEIOU";
    return (str.find(ch) != string::npos);
}

vector<string> getWords(ifstream& file){
    const int maxWordLength = 30;
    char c;
    string word;
    bool isInWord = false;
    
    vector<string> wordVector;
    
    while (file.get(c)){
        if (isalpha(c)){
            isInWord = true;
            word += c;
        }
        else if (isInWord){
            if (word.length() <= maxWordLength){
                wordVector.push_back(word);
            }
            else{
                cout << "Found word with length more than 30: " << word << endl;
                cout << "Ignoring it" << endl << endl;
            }
            word.clear();
            isInWord = false;
        }
        
    }
    return wordVector;
}

void task3(vector<string> wordVector){
    
    vector<string> result;

    for (auto elem : wordVector) {
        bool element_is_vowel = true;
        for(char& c : elem) {
            if (not isVowel(c)){
                element_is_vowel = false;
            }
        }
        if (element_is_vowel){
            result.push_back(elem);
        }
    }
    
    cout << "Words with only vowels:" << endl;
    if (result.size()==0){
        cout << "No words with only vowels" << endl;
        return;
    }
    for (auto word : result) {
        cout << word << endl;
    }
}

int calcDist(string word1, string word2){
    int minLen = min(word1.length(), word2.length());
    int maxLen = max(word1.length(), word2.length());
    int res = maxLen - minLen;
    
    for (int i = 0; i < minLen; i++){
        if (tolower(word1[i]) != tolower(word2[i])){
            res++;
        }
    }
    
    return res;
}

int get_M(vector<string> wordVector){
    vector<int> mVector ;
    
    for (int i = 0; i < wordVector.size(); i++){
        for (int j = i; j < wordVector.size(); j++){
            mVector.push_back(calcDist(wordVector[i], wordVector[j]));
        }
    }
    
    return *max_element(mVector.begin(), mVector.end());
}

bool isValSet(const vector<string> &vectorCheck, int M) {
    for (size_t i = 0; i < vectorCheck.size(); ++i) {
        for (size_t j = i + 1; j < vectorCheck.size(); ++j) {
            if (calcDist(vectorCheck[i], vectorCheck[j]) != M) {
                return false;
            }
        }
    }
    return true;
}

bool vector_check(vector<string> check, vector<string> input){
    for (auto a_elt: input) {
        if (find(check.begin(), check.end(), a_elt) == check.end()) {
            return false;
        }
    }
    return true;
}

bool vector_vector_check(vector<vector<string>> fin, vector<string> check){
    for (int i = 0; i < fin.size(); i++){
        if (vector_check(fin[i], check)){
            return false;
        }
    }
    return true;
}

void findLargestSet(const vector<string> wordVector, vector<string> &curr, vector<string> &res, vector<vector<string>> &fin,
                    int M, int currInd)
{
    if (not isValSet(curr, M)){
        return;
    }
    
    if (currInd == wordVector.size()) {
        if (isValSet(curr, M)) {
            if (curr.size() > res.size()){
                res = curr;
            }
            if (vector_vector_check(fin, curr) && curr.size()>1){
                fin.push_back(curr);
            }
        }
        return;
    }

    curr.push_back(wordVector[currInd]);
    findLargestSet(wordVector, curr, res, fin, M, currInd + 1);
    curr.pop_back();
    
    findLargestSet(wordVector, curr, res, fin, M, currInd + 1);
}

void task14(vector<string> wordVector){
    int M = get_M(wordVector);
    
    vector<string> curr = {};
    vector<string> res = {};
    vector<vector<string>> fin = {};
    
    findLargestSet(wordVector, curr, res, fin, M, 0);
    cout << "M = " << to_string(M) << endl;
    for (int i = 0; i < fin.size(); i++){
        cout << "{ ";
        for (int j = 0; j < fin[i].size(); j++){
            cout << fin[i][j] << " ";
        }
        cout <<" }" << endl;
    }
    cout << endl;
}

int main(int argc, const char * argv[]) {
    ifstream file("test.txt");
    vector<string> wordVector = getWords(file);
    
    cout << "Task3" << endl;
    task3(wordVector);
    
    cout << endl << "Task14" << endl;
    task14(wordVector);
}
