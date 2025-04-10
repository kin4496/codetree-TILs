#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int n;
vector<int> rests;
int leader, follower;

int solve(int);

int main() {
    cin>>n;

    for(int i=0;i<n;i++)
    {
        int cust;
        cin>>cust;
        rests.push_back(cust);
    }

    cin>>leader>>follower;

    long long ans = 0;
    for(int i=0;i<n;i++)
    {
        ans += solve(rests[i]);
    }

    cout <<ans<<endl;
    return 0;
}

int solve(int cust)
{
    if(cust <= leader) return 1;

    cust -= leader;

    return ((cust + follower - 1) / follower) + 1;
}