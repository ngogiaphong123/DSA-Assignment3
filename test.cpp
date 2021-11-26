#include <iostream>
#include <string>
#include <regex>
using namespace std;
bool checkFunctionValue(string value) {
    regex archetype("[a-z]\\w*(\\(\\)|\\(([a-z]\\w*|\\d+|\\'[a-zA-Z0-9 ]*\\')(,([a-z]\\w*|\\d+|\\'[a-zA-Z0-9 ]*\\'))*\\))");
    if(regex_match(value,archetype)==true) return true;
    return false;
}
int main() {
    cout << checkFunctionValue("bo('goEd ',bo,bo)");
}