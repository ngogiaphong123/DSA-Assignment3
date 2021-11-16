#include "SymbolTable.h"
//TOKENIZE
int numberOfWords(string input,char del = ' ') {
    if(input == "()") return 0;
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
bool checkHashTableType(string value) {
    if(value == "LINEAR" || value == "QUADRATIC" || value == "DOUBLE") {
        return true;
    }
    return false;
}
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
            return true;
        }
        else {
            across++;
            i++;
        }
    }
    return false;
}
void hashTable :: print() {
    bool first = true;
    for(int i = 0; i < this->size; i++) {
        if(table[i].flag == "OCCUPIED" && first == true && table[i].level != -1) {
            cout << i << " " << table[i].name << "//"<<table[i].level;
            first = false;

        }
        else if(table[i].flag == "OCCUPIED" &&table[i].level != -1) {
            cout << ";" << i << " " << table[i].name << "//" << table[i].level;
        }
    }
    if(first == false) cout << endl;
    return;
}
int hashTable :: search(string name,int currLevel,int& across) {
    for(int k = currLevel ; k >= 0; k--) {
        int i = 0;
        int key = preHash(name,k);
        while(i != this->size) {
            int j = hash(key,i);
            if(table[j].flag == "NIL") break;
            if(table[j].name == name && table[j].level == k) {
                return j;
            }
            else {
                across++;
                i++;
            }
        }
    }
    return -1;
}
void SymbolTable::run(string filename)
{
    int currLevel = 0;
    hashTable table;
    string temp="";
    ifstream file(filename);
    getline(file, temp);
    string* cmd = tokenize(temp);
    if(checkHashTableType(cmd[0]) == false) {
        delete[] cmd;
        throw InvalidInstruction(temp);
    }
    if(numberOfWords(temp) == 3) {
        table.LinearDouble(cmd[0],cmd[1],cmd[2]);
    }
    else table.Quadratic(cmd[0],cmd[1],cmd[2],cmd[3]);
    while(!file.eof()) {
        getline(file, temp);
        if(temp == "") {
            throw InvalidInstruction(temp);
        }
        cmd = tokenize(temp);
        if(cmd[0] == "INSERT") {
            if(checkIdentifierName(cmd[1]) == false) {
                delete[] cmd;
                throw InvalidInstruction(temp);
            }
            if(numberOfWords(temp) == 2) {
                int key = preHash(cmd[1],currLevel);
                Data data(cmd[1],currLevel,"auto",key,"","OCCUPIED","0");
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
            int numberWordsAssign = numberOfWords(temp);
            if(numberWordsAssign > 3) {
                for(int i = 3 ; i < numberWordsAssign ; i++) {
                    cmd[2] = cmd[2] + " " + cmd[i];
                }
                numberWordsAssign = 3;
            }
            if(checkIdentifierName(cmd[1]) == false) {
                delete[] cmd;
                throw InvalidInstruction(temp);
            }
            if(checkNumberValue(cmd[2]) == true) {
                int across = 0;
                int index = table.search(cmd[1],currLevel,across);
                string t = cmd[1];
                if(index == -1) {
                    delete[] cmd;
                    throw Undeclared(t);
                }
                if(!(table.table[index].type == "auto" || table.table[index].type == "number")) {
                    delete[] cmd;
                    throw TypeMismatch(temp);
                }
                else {
                    table.table[index].type = "number";
                    cout << across << endl;
                }
            }
            else if(checkStringValue(cmd[2]) == true) {
                int across = 0;
                int index = table.search(cmd[1],currLevel,across);
                string t = cmd[1];
                if(index == -1) {
                    delete[] cmd;
                    throw Undeclared(t);
                }
                if(!(table.table[index].type == "auto" || table.table[index].type == "string")) {
                    delete[] cmd;
                    throw TypeMismatch(temp);
                }
                else {
                    table.table[index].type = "string";
                    cout << across << endl;
                }
            }
            else if(checkIdentifierName(cmd[2])==true) {
                int across = 0;
                int indexValue = table.search(cmd[2],currLevel,across);
                int indexAssigned = table.search(cmd[1],currLevel,across);
                if(indexValue == -1) {
                    string t = cmd[2];
                    delete[] cmd;
                    throw Undeclared(t);
                }
                if(table.table[indexValue].type != "number" && table.table[indexValue].type != "string" && table.table[indexValue].type != "auto") {
                    delete[] cmd;
                    throw TypeMismatch(temp);
                }
                if(indexAssigned == -1) {
                    string t = cmd[1];
                    delete[] cmd;
                    throw Undeclared(t);
                }
                if(table.table[indexAssigned].type != "number" && table.table[indexAssigned].type != "string" && table.table[indexAssigned].type != "auto") {
                    delete[] cmd;
                    throw TypeMismatch(temp);
                }
                if(table.table[indexAssigned].type =="auto" && table.table[indexValue].type == "auto") {
                    delete[] cmd;
                    throw TypeCannotBeInfered(temp);
                }
                else {
                    if(table.table[indexAssigned].type == "auto") {
                        table.table[indexAssigned].type = table.table[indexValue].type;
                        cout << across << endl;
                    }
                    else if (table.table[indexValue].type == "auto") {
                        table.table[indexValue].type = table.table[indexAssigned].type;
                        cout << across << endl;
                    }
                    else {
                        if(table.table[indexValue].type != table.table[indexAssigned].type) {
                            delete[] cmd;
                            throw TypeMismatch(temp);
                        }
                        else cout << across << endl;
                    }
                }
            }
            else if(checkFunctionValue(cmd[2]) == true) {
                int across = 0;
                string tmp = cmd[2];
                int openBracket = tmp.find("(");
                string functionName = tmp.substr(0, openBracket);
                tmp = tmp.substr(openBracket);
                int sizeArg = numberOfWords(tmp,',');
                tmp = tmp.substr(1, tmp.length()-2);
                string* arg = tokenize(tmp, ",");
                int indexValue = table.search(functionName,currLevel,across);
                if(indexValue == -1) {
                    delete[] arg;
                    delete[] cmd;
                    throw Undeclared(functionName);
                }
                if(table.table[indexValue].type == "number" || table.table[indexValue].type == "auto" || table.table[indexValue].type == "string") {
                    delete[] cmd;
                    delete[] arg;
                    throw TypeMismatch(temp);
                }
                if(table.table[indexValue].numParameters != sizeArg) {
                    delete[] cmd;
                    delete[] arg;
                    throw TypeMismatch(temp);
                }
                string typeFunction = table.table[indexValue].type;
                if(typeFunction == "function") {
                    string paraList = "";
                    for(int i = 0 ; i < sizeArg ; i++) {
                        if(checkNumberValue(arg[i]) == true) {
                            paraList += "number,";
                        }
                        else if(checkStringValue(arg[i]) == true) {
                            paraList += "string,";
                        }
                        else if(checkIdentifierName(arg[i]) == true) {
                            int var = table.search(arg[i],currLevel,across);
                            if(var == -1) {
                                delete[] arg;
                                delete[] cmd;
                                throw Undeclared(arg[i]);
                            }
                            paraList += table.table[var].type;
                            paraList += ",";
                        }
                    }
                    paraList.insert(0,"(");
                    paraList = paraList.substr(0,paraList.length()-1);
                    paraList += ")->";
                    if(sizeArg == 0) paraList = "()->";
                    int indexAssigned = table.search(cmd[1],currLevel,across);
                    if(indexAssigned == -1) {
                        delete[] arg;
                        delete[] cmd;
                        string t = cmd[1];
                        throw Undeclared(t);
                    }
                    if(table.table[indexAssigned].type == "auto") {
                        delete[] arg;
                        delete[] cmd;
                        throw TypeCannotBeInfered(temp);
                    }
                    else if(table.table[indexAssigned].type == "number" || table.table[indexAssigned].type == "string") {
                        paraList += table.table[indexAssigned].type;
                        table.table[indexValue].type = paraList;
                        cout << across << endl;
                        delete[] arg;
                    }
                    else {
                        delete[] arg;
                        delete[] cmd;
                        throw TypeMismatch(temp);
                    }
                }
                else {
                    int arrowSignal = typeFunction.find("->");
                    string returnType = typeFunction.substr(arrowSignal+2);
                    typeFunction = typeFunction.substr(0, arrowSignal);
                    int sizeType = numberOfWords(typeFunction,',');
                    typeFunction = typeFunction.substr(1,typeFunction.length()-2);
                    string* type = tokenize(typeFunction, ",");
                    for(int i = 0 ; i < sizeArg ; i++) {
                        if(checkNumberValue(arg[i]) == true) {
                            if(!(type[i] == "number" || type[i] == "auto")) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw TypeMismatch(temp);
                            }
                            else type[i] = "number";
                        }
                        else if(checkStringValue(arg[i]) == true) {
                            if(!(type[i] == "string" || type[i] == "auto")) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw TypeMismatch(temp);
                            }
                            else type[i] = "string";
                        }
                        else if(checkIdentifierName(arg[i]) == true) {
                            string t = arg[i];
                            int var = table.search(arg[i],currLevel,across);
                            if(var == -1) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw Undeclared(t);
                            }
                            else {
                                if(table.table[var].type == "auto" && type[i] == "auto") {
                                    delete[] type;
                                    delete[] arg;
                                    delete[] cmd;
                                    throw TypeCannotBeInfered(temp);
                                }
                                else {
                                    if(type[i] == "auto") {
                                        type[i] = table.table[var].type;
                                    }
                                    else if(table.table[var].type == "auto") {
                                        table.table[var].type = type[i];
                                    }
                                    else {
                                        if(table.table[var].type != type[i]) {
                                            delete[] type;
                                            delete[] arg;
                                            delete[] cmd;
                                            throw TypeMismatch(temp);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    string paraList = "";
                    for(int i = 0 ; i < sizeType ; i++) {
                        paraList += type[i];
                        paraList += ",";
                    }
                    paraList.insert(0,"(");
                    paraList = paraList.substr(0,paraList.length()-1);
                    paraList += ")->";
                    if(sizeType == 0) paraList = "()->";
                    paraList += returnType;
                    table.table[indexValue].type = paraList;
                    int indexAssigned = table.search(cmd[1],currLevel,across);
                    if(indexAssigned == -1) {
                        delete[] type;
                        delete[] arg;
                        delete[] cmd;
                        string t = cmd[1];
                        throw Undeclared(t);
                    }
                    if(table.table[indexAssigned].type == "auto" && returnType == "auto") {
                        delete[] type;
                        delete[] arg;
                        delete[] cmd;
                        throw TypeCannotBeInfered(temp);
                    }
                    else {
                        if(table.table[indexAssigned].type == "auto") {
                            if(returnType == "void") {
                                delete[] cmd;
                                delete[] arg;
                                delete[] type;
                                throw TypeMismatch(temp);
                            }
                            table.table[indexAssigned].type = returnType;
                        }
                        else if(returnType == "auto") {
                            returnType = table.table[indexAssigned].type;
                        }
                        else {
                            if(returnType != table.table[indexAssigned].type) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw TypeMismatch(temp);
                            }
                        }
                    }
                    cout << across << endl;
                    delete[] type;
                    delete[] arg;
                }
            }
            else {
                delete[] cmd;
                throw InvalidInstruction(temp);
            }
        }
        else if(cmd[0] == "CALL") {
            if(checkFunctionValue(cmd[1]) == false) {
                delete[] cmd;
                throw TypeMismatch(temp);
            }
            else {
                int across = 0;
                string tmp = cmd[2];
                int openBracket = tmp.find("(");
                string functionName = tmp.substr(0, openBracket);
                tmp = tmp.substr(openBracket);
                int sizeArg = numberOfWords(tmp,',');
                tmp = tmp.substr(1, tmp.length()-2);
                string* arg = tokenize(tmp, ",");
                int indexValue = table.search(functionName,currLevel,across);
                if(indexValue == -1) {
                    delete[] arg;
                    delete[] cmd;
                    throw Undeclared(functionName);
                }
                if(table.table[indexValue].type == "number" || table.table[indexValue].type == "auto" || table.table[indexValue].type == "string") {
                    delete[] cmd;
                    delete[] arg;
                    throw TypeMismatch(temp);
                }
                if(table.table[indexValue].numParameters != sizeArg) {
                    delete[] cmd;
                    delete[] arg;
                    throw TypeMismatch(temp);
                }
                string typeFunction = table.table[indexValue].type;
                if(typeFunction == "function") {
                    string paraList = "";
                    for(int i = 0 ; i < sizeArg ; i++) {
                        if(checkNumberValue(arg[i]) == true) {
                            paraList += "number,";
                        }
                        else if(checkStringValue(arg[i]) == true) {
                            paraList += "string,";
                        }
                        else if(checkIdentifierName(arg[i]) == true) {
                            int var = table.search(arg[i],currLevel,across);
                            if(var == -1) {
                                delete[] arg;
                                delete[] cmd;
                                throw Undeclared(arg[i]);
                            }
                            paraList += table.table[var].type;
                            paraList += ",";
                        }
                    }
                    paraList.insert(0,"(");
                    paraList = paraList.substr(0,paraList.length()-1);
                    paraList += ")->";
                    if(sizeArg == 0) paraList = "()->";
                    paraList += "void";
                    table.table[indexValue].type = paraList;
                    cout << across << endl;
                    delete[] arg;
                }
                else {
                    int arrowSignal = typeFunction.find("->");
                    string returnType = typeFunction.substr(arrowSignal+2);
                    if(returnType != "void") {
                        delete[] arg;
                        throw TypeMismatch(temp);
                    }
                    typeFunction = typeFunction.substr(0, arrowSignal);
                    int sizeType = numberOfWords(typeFunction,',');
                    typeFunction = typeFunction.substr(1,typeFunction.length()-2);
                    string* type = tokenize(typeFunction, ",");
                    for(int i = 0 ; i < sizeArg ; i++) {
                        if(checkNumberValue(arg[i]) == true) {
                            if(!(type[i] == "number" || type[i] == "auto")) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw TypeMismatch(temp);
                            }
                            else type[i] = "number";
                        }
                        else if(checkStringValue(arg[i]) == true) {
                            if(!(type[i] == "string" || type[i] == "auto")) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw TypeMismatch(temp);
                            }
                            else type[i] = "string";
                        }
                        else if(checkIdentifierName(arg[i]) == true) {
                            string t = arg[i];
                            int var = table.search(arg[i],currLevel,across);
                            if(var == -1) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw Undeclared(t);
                            }
                            else {
                                if(table.table[var].type == "auto" && type[i] == "auto") {
                                    delete[] type;
                                    delete[] arg;
                                    delete[] cmd;
                                    throw TypeCannotBeInfered(temp);
                                }
                                else {
                                    if(type[i] == "auto") {
                                        type[i] = table.table[var].type;
                                    }
                                    else if(table.table[var].type == "auto") {
                                        table.table[var].type = type[i];
                                    }
                                    else {
                                        if(table.table[var].type != type[i]) {
                                            delete[] type;
                                            delete[] arg;
                                            delete[] cmd;
                                            throw TypeMismatch(temp);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    string paraList = "";
                    for(int i = 0 ; i < sizeType ; i++) {
                        paraList += type[i];
                        paraList += ",";
                    }
                    paraList.insert(0,"(");
                    paraList = paraList.substr(0,paraList.length()-1);
                    paraList += ")->";
                    if(sizeType == 0) paraList = "()->";
                    paraList += returnType;
                    table.table[indexValue].type = paraList;
                    cout << across << endl;
                    delete[] arg;
                    delete[] type;
                }
            }
        }
        else if(cmd[0] == "BEGIN") {
            currLevel ++;
        }
        else if(cmd[0] == "END") {
            int levelDeleted = currLevel;
            currLevel--;
            if(currLevel < 0) {
                delete[] cmd;
                throw UnknownBlock();
            }
            for(int i = 0 ; i < table.size ; i++) {
                if(table.table[i].level == levelDeleted) {
                    table.table[i].flag = "DELETED";
                    table.table[i].level = -1;
                }
            }
        }
        else if(cmd[0] == "LOOKUP") {
            if(checkIdentifierName(cmd[1]) == false) {
                delete[] cmd;
                throw InvalidInstruction(temp);
            }
            string name = cmd[1];
            int across = 0;
            int index = table.search(name,currLevel,across);
            if(index == -1) {
                delete[] cmd;
                throw Undeclared(name);
            }
            cout << index << endl;
        }
        else if(cmd[0] == "PRINT") {
            table.print();
        }
        else {
            delete[] cmd;
            throw InvalidInstruction(temp);
        }
        delete[] cmd;
    }
    if(currLevel > 0) throw UnclosedBlock(currLevel);
}