#include <iostream>
#include <string>
using namespace std;
long long int preHash(string value,int currLevel) {
    string result = "";
    result += to_string(currLevel);
    int length = value.length();
    for(int i = 0 ; i < length ; i++) {
        int temp = value[i] - 48;
        result += to_string(temp);
    }
    return stoll(result);
}
int main() {
    cout << preHash("mA_AD",0);
}