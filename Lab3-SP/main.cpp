#include <iostream>
#include <fstream>

using namespace std;

enum Token{
    PREPROCESSOR_DIRECTIVE,
    LIBRARY,
    NUMBER,
    CONSTANT_STRING,
    CONSTANT_CHAR,
    COMMENT,
    RESERVED_WORD,
    OPERATOR,
    PUNCTUATION,
    IDENTIFIER,
    ERROR
};

vector<string> classes = {"PREPROCESSOR_DIRECTIVE", "LIBRARY", "NUMBER", "CONSTANT_STRING", "CONSTANT_CHAR", "COMMENT", "RESERVED_WORD", "OPERATOR", "PUNCTUATION", "IDENTIFIER", "ERROR"};

class TockenLoader{
private:
    vector<string> reserved_words{"class", "enum", "func", "import", "init", "let", "open", "operator", "private",
        "public", "static", "struct", "var", "break", "case", "catch", "continue", "default", "do", "else", "for", "if",
        "in", "return", "throw", "switch", "where", "while", "try", "self", "as", "is", "override", "prefix", "mutating",
        "weak", "lazy", "willSet", "final", "Protocol", "print"};
    vector<string> reserved_hash{"available", "colorLiteral", "elseif", "else", "endif", "fileLiteral", "if", "imageLiteral", "keyPath", "selector", "sourceLocation"};
    vector<string> operators{"+", "-", "/", "=", "!", "*", "%", "<", ">", "&", "|", "^", "?", "~"};
    vector<string> punctuation{"(", ")", "{", "}", "[", "]", ".", ",", ":", ";", "@", "?", "!"};
    vector<char> hex_num{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F', 'X', 'x'};
    vector<char> casual_num{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '-'};
    bool prevIsImport = false;
    
    bool token_belongs(vector<string> data, string token){
        for (string word: data){
            if (word == token){
                return true;
            }
        }
        return false;
    }
    
    bool char_belongs(vector<char> data, char token){
        for (char c: data){
            if (c == token){
                return true;
            }
        }
        return false;
    }
    
    bool token_hex_num(string token){
        if (token.substr(0, 2) != "0x"){
            return false;
        }
        for (char c: token){
            if (!char_belongs(hex_num, c)){
                return false;
            }
        }
        return true;
    }
    
    bool token_casual_num(string token){
        for (char c: token){
            if (!char_belongs(casual_num, c)){
                return false;
            }
        }
        return true;
    }

public:
    Token processToken(string token){
        if (token_hex_num(token) || token_casual_num(token)){
            return NUMBER;
        }
        if ((token.at(0) == '"') && (token.at(size(token)-1) == '"')){
            return CONSTANT_STRING;
        }
        if (prevIsImport){
            prevIsImport = false;
            return LIBRARY;
        }
        if ((token.at(0) == '\'') && (token.at(size(token)-1) == '\'')){
            if (size(token)!=3){
                return ERROR;
            }
            return CONSTANT_CHAR;
        }
        if (token.at(0) == '#'){
            return PREPROCESSOR_DIRECTIVE;
        }
        if (token.at(0) == '/' && token.at(1) == '/'){
            return COMMENT;
        }
        if (token_belongs(reserved_words, token)){
            if (token == "import"){
                prevIsImport = true;
            }
            return RESERVED_WORD;
        }
        if (token_belongs(operators, token)){
            return OPERATOR;
        }
        if (token_belongs(punctuation, token)){
            return PUNCTUATION;
        }
        if (isalpha(token[0]) || (token[0] == '_')){
            for (char c: token) {
                    if (!isalnum(c)) {
                        return ERROR;
                    }
                }
                return IDENTIFIER;
            }
        
        return ERROR;
    }
};

bool separatorFinder(char c){
    vector<char> delimeters{ '(', ')', '{', '}', '[', ']', ';', ':', ','};
    for (char check: delimeters){
        if (check == c){
            return true;
        }
    }
    return false;
}

void wordCheck(vector<string>& data, string& added_word){
    if (added_word.length() != 0){
        data.push_back(added_word);
        added_word = "";
    }
}

void getWord(vector<string>& data, string word, bool& cycle){
    string added_word = "";
    for (int i = 0; i < word.length(); i++){
        if (word[i] == ' '){
            wordCheck(data, added_word);
        }
        else if (word[i] == '/' && word[i + 1] == '/'){
            data.push_back(word.substr(i, word.length()));
            return;
        }
        else if (word[i] == '\"'){
            string end_part = word.substr(i+1, word.size());
            int pos = end_part.find("\"");
            if (pos < 0){
                data.push_back(end_part);
                return;
            }
            string result = "\"" + end_part.substr(0, pos+1);
            data.push_back(result);
            i += pos+1;
        }
        else if (separatorFinder(word[i])){
            wordCheck(data, added_word);
            string single_char{word[i]};
            data.push_back(single_char);
        }
        else{
            added_word += word[i];
        }
    }
    wordCheck(data, added_word);
}

void getData(vector<string>& data, ifstream& file){
    string line;
    bool cycle = false;
    while (getline(file, line)){
        getWord(data, line, cycle);
    }
}

void getClasses(ifstream& file){
    vector<string> data;
    TockenLoader loader;
    getData(data, file);
    
    for (auto word: data){
        enum Token type = loader.processToken(word);
        cout << word << " - " << classes[type] << endl;
    }
    
}



int main(int argc, const char * argv[]) {
    ifstream file("data.txt");
    
    cout << "Task 13" << endl << endl;
    getClasses(file);
    
    
    return 0;
}
