#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    string s;
    cin>>s;

    int num = 0;
    int tmp = 1;

    int idx = s.size() - 1;
    for(idx;idx>=0;idx--)
    {
        num += (s[idx] - '0') * tmp;
        tmp *= 2;
    }

    num *= 17;
    string str="";
    while(num > 0)
    {
        str += to_string(num % 2);
        num /= 2;
    }

    reverse(str.begin(),str.end());
    cout<<str;
    return 0;
}