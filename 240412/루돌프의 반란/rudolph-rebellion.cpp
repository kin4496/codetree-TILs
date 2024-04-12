#include <iostream>
#include <vector>
using namespace std;
typedef pair<int,int> Pos;

int board[50][50];
int N,M,P,C,D;

const int INF = 987654321;
int dx[] = {0,-1,0,1,0,1,1,-1,-1};
int dy[] = {0,0,1,0,-1,1,-1,1,-1};

vector<Pos> santas;
vector<int> isWake;
int rx,ry;
vector<int> points,outOfBoard;

int getDist(int,int,int,int);
pair<int,int> findNearSanta();
int findNearR(int);
void play();
void onCollison(int,int,int);
bool isInValid(int,int);

int main() {
    
    cin>>N>>M>>P>>C>>D;
    cin>>rx>>ry;
    rx--;
    ry--;
    board[rx][ry] = 2;
    
    isWake = vector<int>(P,1);
    points = vector<int>(P,0);
    outOfBoard = vector<int>(P,0);

    for(int i=0;i<P;i++)
    {
        int num,x,y;
        cin>>num>>x>>y;
        x--;
        y--;
        board[x][y] = 1;
        santas.push_back(make_pair(x,y));
    }

    play();

    for(int i=0;i<P;i++)
        cout<<points[i]<<" ";
    
    return 0;
}

void play()
{
    pair<int,int> result = findNearSanta();
    int santa = result.first;
    int dir = result.second;

    cout<<santa<<" "<<dir<<endl;
    
    board[rx][ry] = 0;

    rx += dx[dir];
    ry += dy[dir];

    if(board[rx][ry] == 1)
    {
        onCollison(santa,dir,0);
    }

    board[rx][ry] = 2;

    for(int i=0;i<P;i++)
    {
        if(isWake[i] == 0)
        {
            isWake[i] = 1;
            continue;
        }

        int x = santas[i].first;
        int y = santas[i].second;

        dir = findNearR(i);

        board[x][y] = 0;
        
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if(board[nx][ny] == 2)
        {
            onCollison(i,dir,1);
        }else{
            board[nx][ny] = 1;
            santas[i].first = nx;
            santas[i].second = ny;
        }
    }

    for(int i=0;i<P;i++)
    {
        if(outOfBoard[i] == 1) continue;
        points[i] += 1;
    }
}

pair<int,int> findNearSanta()
{
    pair<int,int> ret;
    int nearSanta = 0;
    int minDist = INF;
    for(int i=0;i<P;i++)
    {
        if(outOfBoard[i] == 0) continue;
        int x = santas[i].first;
        int y = santas[i].second;
        int dist = getDist(rx,ry,x,y);
        if(dist <= minDist)
        {
            minDist = dist;
            if(santas[nearSanta].first == x)
            {
                nearSanta = santas[nearSanta].second < y ? i : nearSanta;
            }else if(santas[nearSanta].first < x)
            {
                nearSanta = i;
            }
        }
    }

    int dir = 0;
    minDist = INF;
    for(int i = 1;i<=8;i++)
    {
        
        int nx = santas[nearSanta].first + dx[i];
        int ny = santas[nearSanta].second + dy[i];
        if(isInValid(nx,ny)) continue;
        if(minDist > getDist(nx,ny,santas[nearSanta].first,santas[nearSanta].second))
        {
            dir = i;
            minDist = getDist(nx,ny,santas[nearSanta].first,santas[nearSanta].second);
        }
    }
    ret = make_pair(nearSanta,dir);
    return ret;
}

int findNearR(int santa)
{
    int x = santas[santa].first;
    int y = santas[santa].second;

    int dir = 0;
    int minDist = getDist(x,y,rx,ry);
    for(int i=1;i<=4;i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(isInValid(nx,ny)) continue;
        if(minDist > getDist(nx,ny,rx,ry))
        {
            minDist = getDist(nx,ny,rx,ry);
            dir = i;
        }
    }

    return dir;
}

void onCollison(int santa,int dir, int bySanta)
{
    int range = bySanta == 1 ? D : C;
    
    points[santa] += range;
    isWake[santa] = 0;

    int x = santas[santa].first;
    int y = santas[santa].second;

    int nx = x + dx[dir] * range;
    int ny = y + dy[dir] * range;

    while(!isInValid(nx,ny) && board[nx][ny] == 1)
    {
        nx += dx[dir];
        ny += dy[dir];
    }

    if(isInValid(nx,ny))
    {
        outOfBoard[santa] = 1;
    }else{
        board[nx][ny] = 1;
    }
}

int getDist(int x1,int y1,int x2,int y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool isInValid(int x,int y)
{
    return (x < 0 || y < 0 || x >= N || y >= N);
}