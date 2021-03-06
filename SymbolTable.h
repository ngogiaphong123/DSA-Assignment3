#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);
};
struct Data {
    string name;
    int level;
    string type;
    long long int key;
    int numParameters;
    string value;
    string flag; // 3 states : NIL,OCCUPIED,DELETED
    Data() {
        this->name ="";
        this->level = -1;
        this->type = "";
        this->key = -1;
        this->value = "";
        this->flag = "NIL";
        this->numParameters = 0;
    }
    Data(string name, int level, string type, long long int key, string value,string flag,string numParameters) {
        this->name = name;
        this->level = level;
        this->type = type;
        this->key = key;
        this->value = value;
        this->flag = flag;
        this->numParameters = stoi(numParameters);
    }
};
struct hashTable {
    Data* table;
    long int size;
    string type;
    long int c1;
    long int c2;
    void LinearDouble(string type,string size,string c1) {
        this->type = type;
        this->size = stol(size);
        table = new Data[this->size];
        this->c1 = stol(c1);
        this->c2 = 0;
    }
    void Quadratic(string type,string size,string c1,string c2) {
        this->type = type;
        this->size = stol(size);
        table = new Data[this->size];
        this->c1 = stol(c1);
        this->c2 = stol(c2);
    }
    int linearProbing(long long int key,int i) {
        return ((key%size)+c1*i)%size;
    }
    int quadraticProbing(long long int key,int i) {
        return ((key%size)+c1*i+c2*i*i)%size;
    }
    int doubleProbing(long long int key,int i) {
        int h1 = key%size;
        int h2 = 1 + (key%(size-2));
        return (h1+h2*c1*i)%size;
    }
    int hash(long long int key,int i) {
        if(this->type == "LINEAR") {
            return linearProbing(key,i);
        }
        else if(this->type == "QUADRATIC") {
            return quadraticProbing(key,i);
        }
        else if(this->type == "DOUBLE") {
            return doubleProbing(key,i);
        }
        else return -1;
    }
    bool insert(Data data,int& across);
    void print();
    int search(string name,int currLevel,int& across);
    ~hashTable();
};
#endif