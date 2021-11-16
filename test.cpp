#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cin >> s;
    s = s.substr(0,s.length()-1);
    s+=")";
    cout << s << endl;
}