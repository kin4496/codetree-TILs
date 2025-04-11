#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int n, m, f;

int board3d[1010][1010];
int board2d[30][30];

int sx3d, sy3d;
int ex3d, ey3d;

int sx2d, sy2d;
int ex2d, ey2d;


vector<vector<int>> extra;

int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1 ,0, 0};

const int TELEPORT = 8;
const int OBSTACLE = 1;
const int EXTRA = 9;

pair<int,int> findExitFrom3dBoard();
void mark3dBoard(int ,int);
void markObstacle(int ,int, int);
int isInValid(int, int, int);
void rotate(int x,int y);
void findStartPoint();
int isInValid(int, int, int);
void findStartPointFrom2dBoard();
void findExitFrom2dBoard();
void teleport(int&, int&, int ,int );

void print3dBoard();

void moveExtra(int turn){
    for(auto arr : extra){
        int& r = arr[0];
        int& c = arr[1];
        int d = arr[2];
        int v = arr[3];

        if(turn % v != 0) continue;

        int nr = r + dx[d];
        int nc = c + dy[d];

        if(isInValid(nr,nc, n)) continue;
        if(board2d[nr][nc] == OBSTACLE || board2d[nr][nc] == 4) continue;

        r = nr;
        c = nc;

        board2d[r][c] = EXTRA;
    }
}
int main() {

    int turn = 0;
    

    cin>>n>>m>>f;

    
    int isFindExitFrom3dBoard = 0;

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cin>>board2d[i][j];

            
        }
    }

    pair<int,int> exitFrom3dBoard = findExitFrom3dBoard();



    mark3dBoard(0, 0);
    mark3dBoard(0, 2 * m);
    mark3dBoard(2 * m, 0);
    mark3dBoard(2 * m, 2 * m);

    markObstacle(m, 2 * m, 3);//동
    markObstacle(m, 0, 1);//서
    markObstacle(2 * m, m, 0);//남
    markObstacle(0, m, 2);//북
    markObstacle(m, m, 0);//탑

    for(int i=0;i<f;i++)
    {
        int r, c, d, v;
        cin>>r>>c>>d>>v;
        vector<int> temp{r,c,d,v};
        extra.push_back(temp);
        board2d[r][c] = EXTRA;
    }

    
    findStartPoint();
    findExitFrom2dBoard();
    findStartPointFrom2dBoard();

    queue<pair<int,int>> q;
    q.push({sx3d, sy3d});

    vector<vector<int>> visited(3 * m + 10, vector<int>(3 * m + 10));
    visited[sx3d][sy3d] = 1;

    

    int isFind = 0;
    while(!q.empty())
    {
        int size = int(q.size());
        
        for(int i = 0; i < size;i++)
        {
            int x = q.front().first;
            int y = q.front().second;
            q.pop();
            
            //cout<<x<<" "<<y

            if(x == exitFrom3dBoard.first && y == exitFrom3dBoard.second)
            {
                isFind = 1;
                break;
            } 
            
            for(int j=0;j<4;j++)
            {
                int nx = x + dx[j];
                int ny = y + dy[j];

                if(isInValid(nx, ny, 3 * m)) continue;
                teleport(nx,ny, x, y);
                if(visited[nx][ny] || board3d[nx][ny] == OBSTACLE) continue;

                visited[nx][ny] = 1;
                q.push({nx, ny});
            }
        }
        if(isFind) break;
        turn++;
        moveExtra(turn);       
    }

    if(isFind == 0) 
    {
        cout<<-1<<endl;
        return 0;
    }

    q = queue<pair<int,int>>();
    visited = vector<vector<int>>(n + 10, vector<int>(n + 10, 0));

    q.push({sx2d, sy2d});
    visited[sx2d][sy2d] = 1;

    isFind = 0;
    while(!q.empty())
    {
        int size = int(q.size());
        for(int i=0;i<size;i++)
        {
            int x = q.front().first;
            int y = q.front().second;
            q.pop();

            if(board2d[x][y] == EXTRA){
                continue;
            }

            if(x == ex2d && y == ey2d){
                isFind = 1;
                break;
            }

            

            for(int j=0;j<4;j++)
            {
                int nx = x + dx[j];
                int ny = y + dy[j];

                if(isInValid(nx,ny,n)) continue;
                if(visited[nx][ny]) continue;
                if(board2d[nx][ny] == OBSTACLE || board2d[nx][ny] == EXTRA) continue;

                visited[nx][ny] = 1;
                q.push({nx,ny});
            }
        }
        if(isFind) break;
        turn++;
        moveExtra(turn);
    }

    if(isFind == 0){
        cout<<"2d not found"<<endl;
        cout<<-1<<endl;
        return 0;
    }
    cout<<turn<<endl;

    return 0;
}

void teleport(int& x, int& y,int curx,int cury)
{
    if(board3d[x][y] != TELEPORT) return;
    
    
    x = curx;
    y = cury;

    //cout<<"("<<x<<", "<<y<<") => ";

    int border = 3 * m / 2;
   
    if(x == m && y < border){//왼상 아래
        int step = y % m + 1;
        x -= step;
        y += step;
    }else if(y == m && x < 2 * m){//왼상 위
        int step = m - (x % m);
        x += step;
        y -= step;
    }else if(y == 2 * m - 1 && x < border){//우상 위
        int step = m - x % m;
        x += step;
        y += step;
    }else if(x == m && y > border){//우상 아래
        int step = y % m + 1;
        x -= step;
        y -= step;
    }else if(x == 2 * m - 1 && y < border){//왼하 위
        int step = m - y % m;
        x += step;
        y += step;
    }else if(y == m && x > border){//왼하 아래
        int step = x % m + 1;
        x -= step;
        y -= step;
    }else if(y == 2 * m -1 && x > border){//우하 아래
        int step = x % m + 1;
        x -= step;
        y += step;
    }else if(x == 2 * m - 1 && y > border){//우하 위
        int step = y % m + 1;
        x += step;
        y -= step;
    }

    //cout<<"("<<x<<", "<<y<<")"<<endl;
}
void print3dBoard()
{
    for(int i=0;i< 3 * m;i++)
    {
        for(int j=0;j<3 * m;j++)
        {
            cout<<board3d[i][j]<<" ";
        }
        cout<<endl;
    }
}

void findStartPoint()
{
    for(int i=0;i<3 * m;i++)
    {
        for(int j=0;j<3 * m;j++)
        {
            if(board3d[i][j] == 2)
            {
                sx3d = i;
                sy3d = j;
            }
        }
    }
}
pair<int,int> findExitFrom3dBoard()
{

    int stx = -1, sty = -1;
    int tx, ty;
    int dir = 0;
    int findDir = 0;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(board2d[i][j] != 3) continue;
            for(int k=0;k<4;k++)
            {
                int nx = i + dx[k];
                int ny = j + dy[k];

                if(isInValid(nx, ny, n)) continue;
                if(stx == -1){
                    stx = i;
                    sty = j;
                }
                if(board2d[nx][ny] == 0){
                    dir = k;
                    findDir = 1;
                    tx = i;
                    ty = j;
                    break;
                }
            }
            if(findDir) break;
        }
        if(findDir) break;
    }

    vector<vector<int>> rect(m, vector<int>(m, 0));

    if(dir == 0)//동
    {
        int x = m - 1;
        int y = m - (tx - stx) - 1;

        //cout<<x<<" "<<y<<endl;

        for(int i=0;i<3;i++)
        {
            int temp = x;
            x = y;
            y = m - 1 - temp;
        }
        
        //cout<<x<<" "<<y<<endl;

        x += m;
        y += 2 * m;

        return {x, y};
    }else if(dir == 1){//서
        int x = m - 1;
        int y = tx - stx;
        
        //cout<<x<<" "<<y<<endl;

        for(int i=0;i<1;i++)
        {
            int temp = x;
            x = y;
            y = m - 1 - temp;
        }

        x += m;

        return {x, y};
    }else if(dir == 2){//남
        int x = m - 1;
        int y = ty - sty;

        x += 2 * m;
        y += m;

        return {x, y};
    }else if(dir == 3){//북
        int x = m - 1;
        int y = m - 1 - (ty - sty);
        //cout<<x<<" "<<y<<endl;
        for(int i=0;i<2;i++)
        {
            int temp = x;
            x = y;
            y = m - 1 - temp;
        }

        y += m;
        //cout<<x<<" "<<y<<endl;
        return {x, y};
    }

    return {-1, -1};
}

void findStartPointFrom2dBoard(){
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(board2d[i][j] != 3) continue;
            for(int k=0;k<4;k++)
            {
                int nx = i + dx[k];
                int ny = j + dy[k];

                if(isInValid(nx, ny, n)) continue;
                if(board2d[nx][ny] == 0){
                    sx2d = i;
                    sy2d = j;
                    return;
                }
            }
        }
    }
}

void findExitFrom2dBoard(){

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(board2d[i][j] == 4)
            {
                ex2d = i;
                ey2d = j;
                return;
            }
        }
    }
}

void mark3dBoard(int x,int y)
{
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
        {
            board3d[x + i][y + j] = TELEPORT;
        }
    }
}

void markObstacle(int x, int y,int cnt)
{
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
        {
            cin>>board3d[x + i][y + j];
        }
    }

    for(int i=0;i<cnt;i++) rotate(x, y);    
}

void rotate(int x,int y)
{
    vector<vector<int>> temp(m, vector<int>(m, 0));

    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
        {
            temp[j][m - i - 1] = board3d[x + i][y + j];
        }
    }

    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
        {
            board3d[x + i][y + j] = temp[i][j];
        }
    }
}

int isInValid(int x , int y, int outside)
{
    return (x < 0 || y < 0 || x >= outside || y >= outside);
}