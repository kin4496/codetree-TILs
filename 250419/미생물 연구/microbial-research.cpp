#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

int N, Q;
int board[20][20];

int dx[] = {0,0,1,-1};
int dy[] = {1,-1,0,0};

void setBoard(int,int,int,int,int,set<int>&);
int moreThanOne(int);
void removeRect(int);

void clearBoard();
vector<vector<pair<int,int>>> makeRects();

pair<int,int> getValidPos(vector<pair<int,int>>&);
void relocate(int,int, int ,vector<pair<int,int>>&);

vector<vector<pair<int,int>>> makeRealRects();
set<int> getNearRects(vector<pair<int,int>>& rect);

void printBoard(){
    cout<<endl;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int main() {
    cin>>N>>Q;

    for(int i=0;i<Q;i++)
    {
        int x1,y1,x2,y2;
        cin>>x1>>y1>>x2>>y2;

        set<int> metRect;
        setBoard(x1,y1,x2,y2,i+1,metRect);

        //printBoard();
        
        for(auto rect : metRect)
        {
            if(moreThanOne(rect))
            {
                removeRect(rect);
            }
        }

        

        vector<vector<pair<int,int>>> rects = makeRects();
        clearBoard();

        auto compare = [](pair<int,int>& a, pair<int,int>& b){
            if(a.first == b.first) return !(a.second < b.second);
            return !(a.first > b.first);
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(compare)> pq(compare);

        for(int i=0;i<rects.size();i++)
        {
            int size = int(rects[i].size());
            if(size == 0) continue;
            pq.push({size, i});
        }

        while(!pq.empty())
        {
            int rect = pq.top().second;
            //cout<<pq.top().first<<" "<<pq.top().second<<endl;
            
            pq.pop();

            

            pair<int,int> res = getValidPos(rects[rect]);
            if(res.first == -1 && res.second == -1) continue;

            relocate(res.first, res.second, rect, rects[rect]);
        }

        //printBoard();

        rects = makeRealRects();

        int ans = 0;
        for(int j=0;j<int(rects.size());j++)
        {
            int size = int(rects[j].size());
            if(size == 0) continue;

            int rect = j;

            set<int> nearRects = getNearRects(rects[rect]);

            //cout<<rect<<endl;


            for(auto nearRect : nearRects)
            {
                //cout<<int(rects[rect].size()) * int(rects[nearRect].size())<<endl;
                ans += int(rects[rect].size()) * int(rects[nearRect].size());
            }

            //cout<<endl;
        }

        ans /= 2;

        cout<<ans<<endl;
    }
    return 0;
}


void setBoard(int x1,int y1,int x2,int y2,int rect,set<int>& metRects)
{
    for(int i=x1;i<x2;i++)
    {
        for(int j=y1;j<y2;j++)
        {
            if(board[i][j] != 0) metRects.insert(board[i][j]);
            board[i][j] = rect;
        }
    }
}
int isInValid(int x,int y)
{
    return (x<0 || x>=N || y < 0 || y >= N);
}
void dfs(int x,int y,vector<vector<int>> & visited)
{
    visited[x][y] = 1;
    int rect = board[x][y];

    for(int d = 0;d<4;d++)
    {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if(isInValid(nx,ny)) continue;
        if(visited[nx][ny]) continue;

        if(board[nx][ny] == rect)
            dfs(nx,ny,visited);
    }
}
int moreThanOne(int rect)
{
    int cnt=0;
    vector<vector<int>> visited(N, vector<int>(N,0));

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(visited[i][j]) continue;
            if(board[i][j] == rect)
            {
                cnt++;
                dfs(i,j,visited);
            }
        }
    }

    return cnt > 1;
}
void removeRect(int rect)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(board[i][j] == rect)
            {
                board[i][j] = 0;
            }
        }
    }
}

void clearBoard()
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            board[i][j] = 0;
        }
    }
}
vector<vector<pair<int,int>>> makeRects()
{
    vector<vector<pair<int,int>>> ret(60, vector<pair<int,int>>());

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            int rect = board[i][j];
            if(rect == 0) continue;

            ret[rect].push_back({i, j});
        }
    }

    for(int i=0;i<60;i++)
    {
        if(int(ret[i].size()) == 0) continue;

        int minX = ret[i][0].first;
        int minY = ret[i][0].second;

        for(int j=0;j<int(ret[i].size());j++)
        {
            minX = min(minX, ret[i][j].first);
            minY = min(minY, ret[i][j].second);
        }

        for(int j=0;j<int(ret[i].size());j++)
        {
            ret[i][j].first -= minX;
            ret[i][j].second -= minY;
        }
    }
    return ret;
}

pair<int,int> getValidPos(vector<pair<int,int>>& rect)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            int isFind = 1;
            for(int k=0;k<int(rect.size());k++)
            {
                int x = i + rect[k].first;
                int y = j + rect[k].second;

                if(isInValid(x,y) || board[x][y] != 0)
                {
                    isFind = 0;
                    break;
                }
            }

            if(isFind) return {i, j};
        }
    }

    return {-1, -1};
}
void relocate(int x ,int y, int r,vector<pair<int,int>>& rect)
{
    for(int i=0;i<int(rect.size());i++)
    {
        board[x + rect[i].first][y + rect[i].second] = r;
    }
}

vector<vector<pair<int,int>>> makeRealRects()
{
    vector<vector<pair<int,int>>> ret(60, vector<pair<int,int>>());

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            int rect = board[i][j];
            if(rect == 0) continue;
            ret[rect].push_back({i, j});
        }
    }

    return ret;
}
set<int> getNearRects(vector<pair<int,int>>& rect)
{
    set<int> ret;

    for(int i=0;i<int(rect.size());i++)
    {
        int x = rect[i].first;
        int y = rect[i].second;

        int r = board[x][y];

        for(int d=0;d<4;d++)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if(isInValid(nx,ny)) continue;

            if(board[nx][ny] != 0 && board[nx][ny] != r)
                ret.insert(board[nx][ny]);
        }
    }

    return ret;
}