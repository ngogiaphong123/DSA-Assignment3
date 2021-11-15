#include <iostream>
#include <string>
using namespace std;

int main() {
    string result = "";
    result += to_string(0);
    string s= "xB";
    for(int i = 0 ; i < s.length() ; i++) {
        int temp = s[i] - 48;
        result += to_string(temp);
    }
    cout << stoi(result) << endl;
}