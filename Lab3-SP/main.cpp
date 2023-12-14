
#include <iostream>
#include <fstream>
using namespace std;
enum TokenClass{
    NUMBER,
    CONSTANT_STRING,
    CONSTANT_CHAR,
    PREPROCESSOR_DIRECTIVE,
    LIBRARY,
    COMMENT,
    RESERVED_WORD,
    OPERATOR,
    PUNCTUATION,
    IDENTIFIER,
    ERROR
};
vector<string> classes_names = {"NUMBER", "CONSTANT_STRING", "CONSTANT_CHAR", "PREPROCESSOR_DIRECTIVE", "LIBRARY", "COMMENT", "RESERVED_WORD", "OPERATOR",
                                "PUNCTUATION", "IDENTIFIER", "ERROR"};
class TokenProcessor{
private:
    vector<string> reserved_words{"asm", "auto", "break", "case", "catch", "char", "class", "const", "continue", "default", 				"delete", "do",
                                    "double", "else", "enum", "extern", "float", "for", "friend", "goto","if", "inline","int","long","new",
                                    "operator", "private", "protected", "public", "register","return","short","signed","sizeof","static",
                                    "struct","switch", "string", "template", "this", "default", "throw", "try", "typedef", "union", "unsigned",
                                    "virtual", "void", "volatile", "while", "int", "cout", "cin"};
    vector<string> operators{"+", "-", "*", "/", "%", "=", "+=", "-=", "*=", "/=", "%=", "==", "!=", ">", "<", ">=", "<=", "!", "||", "&&",
                                ">>", "<<", "&"};
    vector<string> punctuation{";", ":", ",", "(", ")", "{", "}"};
    vector<char> hexadecimal_num{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F', 'X', 'x'};
    vector<char> ar_and_fl_num{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', '-'};
    
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
            if (!char_belongs(hexadecimal_num, c)){
                return false;
            }
        }
        return true;
    }
    
    bool token_ar_fl_num(string token){
        for (char c: token){
            if (!char_belongs(ar_and_fl_num, c)){
                return false;
            }
        }
        return true;
    }
public:
    TokenClass processToken(string token){
        if (token_hex_num(token) || token_ar_fl_num(token)){
            return NUMBER;
        }
        if ((token.at(0) == '"') && (token.at(size(token)-1) == '"')){
            return CONSTANT_STRING;
        }
        if ((token.at(0) == '<') && (token.at(size(token)-1) == '>')){
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
        if ((token.at(0) == '/' && token.at(1) == '/') || (token.substr(0, 2) == "/*" && token.substr(size(token)-2, size(token)) == "*/")){
            return COMMENT;
        }
        if (token_belongs(reserved_words, token)){
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
bool separator(char c){
    vector<char> delimeters{';', ':', ',', '(', ')', '{', '}'};
    for (char check: delimeters){
        if (check == c){
            return true;
        }
    }
    return false;
}
void check_word(vector<string>& data, string& added_word){
    if (added_word.length() != 0){
        data.push_back(added_word);
        added_word = "";
    }
}
void separateWord(vector<string>& data, string word, bool& cycle){
    string added_word = "";
    for (int i = 0; i < word.length(); i++){
        if (!cycle){
            if (word[i] == ' '){
                check_word(data, added_word);
            }
            else if (word[i] == '/' && word[i + 1] == '/'){
                data.push_back(word.substr(i, word.length()));
                return;
            }
            else if (word[i] == '/' && word[i + 1] == '*'){
                cycle = true;
                data.push_back("/*");
                i += 1;
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
            else if (separator(word[i])){
                check_word(data, added_word);
                string single_char{word[i]};
                data.push_back(single_char);
            }
            else{
                added_word += word[i];
            }
        }
        else{
            if (word[i] == '*' && word[i + 1] == '/'){
                data.back() += "*/";
                cycle = false;
                i += 1;
            }
            else{
                if (i == 0){
                    data.back() += " ";
                }
                data.back() += word[i];
            }
        }
    }
    check_word(data, added_word);
}
void getData(vector<string>& data, ifstream& file){
    string line;
    bool cycle = false;
    while (getline(file, line)){
        separateWord(data, line, cycle);
    }
}
void task3(ifstream& file){
    vector<string> data;
    TokenProcessor processor;
    getData(data, file);
    
    for (auto word: data){
        enum TokenClass type = processor.processToken(word);
        cout << "< " << word << ", " << classes_names[type] << ">" << endl;
    }
    
}
int main(int argc, const char * argv[]) {
    ifstream file("data.txt");
    
    cout << "Task 3. <Lexem, lexem type>" << endl << endl;
    task3(file);
    
    
    return 0;
}

