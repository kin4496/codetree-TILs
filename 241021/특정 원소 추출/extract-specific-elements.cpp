#include <iostream>
#include <vector>
#include <deque>
using namespace std;

int n,m;
vector<int> arr;

int getLeft(deque<int> dq,int);
int getRight(deque<int> dq,int);
int getLeft2(deque<int>& dq,int);
int getRight2(deque<int>& dq,int);

int main() {
    // 여기에 코드를 작성해주세요.
    cin>>n>>m;
    deque<int> dq;
    
    for(int i=0;i<n;i++) dq.push_back(i+1);

    int ans = 0;
    for(int i=0;i<m;i++)
    {
        int pos;
        cin>>pos;

        int left = getLeft(dq,pos);
        int right = getRight(dq,pos);
        
        int cnt = min(left,right);
        //cout<<cnt<<endl;
        
        if(left < right)
        {
            getLeft2(dq,pos);
        }else{
            getRight2(dq,pos);
        }

        ans += cnt;
    }

    cout<<ans<<endl;
    return 0;
}

int getLeft(deque<int> dq,int pos)
{
    int ret = 0;
    while(dq.front() != pos)
    {
        int p = dq.front();
        dq.pop_front();

        dq.push_back(p);

        ret++;
    }


    return ret;
}

int getRight(deque<int> dq,int pos)
{
    int ret = 0;
    //cout<<"START"<<endl;
    while(dq.back() != pos)
    {
        int p = dq.back();
        
        //cout<<p<<endl;
        dq.pop_back();

        dq.push_front(p);

        ret++;
    }
    //cout<<"ENDL"<<endl;
    return ret + 1;
}

int getLeft2(deque<int>& dq,int pos)
{
    int ret = 0;
    while(dq.front() != pos)
    {
        int p = dq.front();
        dq.pop_front();

        dq.push_back(p);

        ret++;
    }

    dq.pop_front();
    return ret;
}

int getRight2(deque<int>& dq,int pos)
{
    int ret = 0;
    while(dq.back() != pos)
    {
        int p = dq.back();
        dq.pop_back();

        dq.push_front(p);

        ret++;
    }

    dq.pop_back();
    return ret + 1;
}