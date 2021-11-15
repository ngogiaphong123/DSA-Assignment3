#include "SymbolTable.h"
//TOKENIZE
int numberOfWords(string input,char del = ' ') {
    int count = 0;
    int n = input.length();
    for(int i = 0 ; i < n ; i++) {
        if(input[i] == del) count++;
    }
    return count+1;
}
string* tokenize(string input, string del = " ") {
    char s = del[0];
    int numberOfWord = numberOfWords(input,s);
    int i=0;
    string *cmd = new string[numberOfWord];
    int start = 0;
    int end = (int)input.find(del); 
    while (end != -1) {
        cmd[i] = input.substr(start, end - start);
        start = (int)end + (int)del.size();
        end = (int)input.find(del, start);
        i++;
    }
    cmd[i] = input.substr(start, end - start);
    return cmd;
}
//CHECK CONDITION
bool checkStringValue(string value) {
    regex archetype("\\'[a-zA-Z0-9 ]*\\'");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkNumberValue(string value) {
    regex archetype("\\d+");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkFunctionValue(string value) {
    regex archetype("[a-z]\\w*(\\(\\)|\\(([a-z]\\w*|\\d+|\\'[a-zA-Z0-9 ]*\\')(,([a-z]\\w*|\\d+|\\'[a-zA-Z0-9 ]*\\'))*\\))");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkIdentifierName(string value) {
    if(value == "string" || value == "number" || value == "true" || value == "false") return false;
    regex archetype("[a-z]\\w*");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
bool checkPrimitiveType(string value) {
    if(value == "number" || value == "string") return true;
    return false;
}
bool checkFunctionType(string value) {
    regex archetype("(\\(\\)|\\((number|string)(,(number|string))*\\))(->(number|string))");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
//Process
int preHash(string value,int currLevel) {
    string result = "";
    result += to_string(currLevel);
    int length = value.length();
    for(int i = 0 ; i < length ; i++) {
        int temp = value[i] - 48;
        result += to_string(temp);
    }
    return stoi(result);
}
bool hashTable :: insert(Data data,int& across) {
    int i = 0;
    while(i != this->size) {
        int j = hash(data.key,i);
        if(table[j].name == data.name) {
            across = -1;
            return false;
        }
        if(table[j].flag == "NIL" || table[j].flag == "DELETED") {
            table[j] = data;
            across = i;
            return true;
        }
        else i++;
    }
    return false;
}
void SymbolTable::run(string filename)
{
    int currLevel = 0;
    hashTable table;
    string temp="";
    ifstream file(filename);
    getline(file, temp);
    string* cmd = tokenize(temp);
    if(numberOfWords(temp) == 3) {
        table.setLinearDouble(cmd[0],cmd[1],cmd[2]);
    }
    else table.setQuadratic(cmd[0],cmd[1],cmd[2],cmd[3]);
    while(!file.eof()) {
        getline(file, temp);
        if(temp == "") {
            throw InvalidInstruction(temp);
        }
        cmd = tokenize(temp);
        if(cmd[0] == "INSERT") {
            if(numberOfWords(temp) == 2) {
                int key = preHash(cmd[1],currLevel);
                Data data(cmd[1],currLevel,"auto",key,"","OCCUPIED",0);
                int across = 0;
                if(table.insert(data,across) == false) {
                    if(across == -1) {
                        delete[] cmd;
                        throw Redeclared(data.name);
                    }
                    else throw Overflow(temp);
                }
                else cout << across << endl;
            }
            else if(numberOfWords(temp) == 3) {
                if(currLevel != 0) throw InvalidDeclaration(temp);
                int key = preHash(cmd[1],currLevel);
                Data data(cmd[1],currLevel,"function",key,"","OCCUPIED",cmd[2]);
                int across = 0;
                if(table.insert(data,across) == false) {
                    if(across == -1) {
                        delete[] cmd;
                        throw Redeclared(data.name);
                    }
                    else throw Overflow(temp);
                }
                else cout << across << endl;
            }
        }
        else if(cmd[0] == "ASSIGN") {

        }
        else if(cmd[0] == "CALL") {

        }
        else if(cmd[0] == "BEGIN") {

        }
        else if(cmd[0] == "END") {

        }
        else if(cmd[0] == "LOOKUP") {

        }
        else if(cmd[0] == "PRINT") {

        }
        else {
            delete[] cmd;
            throw InvalidInstruction(temp);
        }
        delete[] cmd;
    }
    if(currLevel > 0) throw UnclosedBlock(currLevel);
}