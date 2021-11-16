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
                if(index == -1) {
                    delete[] cmd;
                    throw Undeclared(cmd[1]);
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
                if(index == -1) {
                    delete[] cmd;
                    throw Undeclared(cmd[1]);
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
                    delete[] cmd;
                    throw Undeclared(cmd[2]);
                }
                if(table.table[indexValue].type != "number" && table.table[indexValue].type != "string" && table.table[indexValue].type != "auto") {
                    delete[] cmd;
                    throw TypeMismatch(temp);
                }
                if(indexAssigned == -1) {
                    delete[] cmd;
                    throw Undeclared(cmd[1]);
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
                if(indexValue != -1) {
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
                            int var = table.search(arg[i],currLevel,across);
                            if(var == -1) {
                                delete[] type;
                                delete[] arg;
                                delete[] cmd;
                                throw Undeclared(table.table[var].name);
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
                    int indexAssigned = table.search(cmd[1],currLevel,across);
                    if(indexAssigned == -1) {
                        delete[] type;
                        delete[] arg;
                        delete[] cmd;
                        throw Undeclared(cmd[1]);
                    }
                    if(table.table[indexAssigned].type == "auto" && returnType == "auto") {
                        delete[] type;
                        delete[] arg;
                        delete[] cmd;
                        throw TypeCannotBeInfered(temp);
                    }
                    else {
                        if(table.table[indexAssigned].type == "auto") {
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