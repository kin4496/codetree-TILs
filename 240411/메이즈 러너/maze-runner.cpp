#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int board[10][10];
int dists[10];
int n, m, k;
int ex, ey;
bool isAllOut = true;
pair<int, int> runner[10];
int isOut[10];

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,1,-1 };

const int UP = 0, LEFT = 3;

bool isInValid(int x, int y)
{
	return (x < 0 || x >= n || y < 0 || y >= n);
}
int getDist(int x1, int y1,int x2,int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}
void printBoard()
{
	cout<<endl;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<<board[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}
void printRunner()
{
	cout<<endl;
		cout<<"After play runner are"<<endl;
		for(int i=0;i<m;i++)
		{
			if(isOut[i])
			{
				cout<<"runner "<<i<<" is Out "<<endl;
				continue;
			}
			cout<<"runner "<<i<<" is at "<<runner[i].first << " " <<runner[i].second<<endl;
		}
		cout<<endl;
}
void play();
int main()
{
	cin >> n >> m >> k;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> board[i][j];
		}
	}

	for (int i = 0; i < m; i++)
	{
		cin >> runner[i].first >> runner[i].second;
		runner[i].first-= 1;
		runner[i].second -= 1;
	}
	
	cin>>ex>>ey;
	ex--;
	ey--;
	board[ex][ey] = -1;

	//printBoard();
	for (int i = 0; i < k; i++)
	{
		play();
		//printBoard();
		//printRunner();
	}

	int sum = 0;
	for (int i = 0; i < m; i++) sum += dists[i];
	
	cout << sum << endl;
	cout << ex + 1<< " "<< ey + 1<< endl;
	return 0;
}
void moveRunner()
{
	isAllOut = true;
	for (int i = 0; i < m; i++)
	{
		if (isOut[i]) continue;
		isAllOut = false;

		int rx = runner[i].first;
		int ry = runner[i].second;
		int curDist = getDist(rx, ry, ex, ey);


		for (int j = 0; j < 4; j++)
		{
			int nx = rx + dx[j];
			int ny = ry + dy[j];

			if (isInValid(nx, ny) || board[nx][ny] > 0) continue;

			int dist = getDist(nx, ny, ex, ey);
			if (dist >= curDist) continue;

			rx = nx;
			ry = ny;

			runner[i].first = nx;
			runner[i].second = ny;
			//cout<<"runner"<<i<<" move to "<<nx<<" "<<ny<<endl;
			dists[i] += 1;
			break;
		}

		if (rx == ex && ry == ey)
		{
			isOut[i] = 1;
		}
	}
}

pair<int, int> getSquare(int rx, int ry,int dir,int length)
{
	int sx = min(ex, rx);
	int sy = min(ey, ry);

	for (int i = 0; i < length - 1; i++)
	{
		int nsx = sx + dx[dir];
		int nsy = sy + dy[dir];
		if (isInValid(nsx, nsy)) break;
		if (nsx + length <= ex || nsy + length <= ey) break;
		if (nsx + length <= rx || nsy + length <= ry) break;

		sx = nsx;
		sy = nsy;
	}
	
	return make_pair(sx, sy);
}
void rotate(int x, int y, int length)
{
	vector<vector<int>> tempBoard(length, vector<int>(length, 0));

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			int value = board[x + i][y + j];
			if(value > 0) value-=1;
			tempBoard[j][(length - i - 1)] = value;
		}
	}

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			board[x + i][y + j] = tempBoard[i][j];
			if(board[x + i][y + j] == -1)
			{
				ex = x + i;
				ey = y + j;
			}
		}
	}


	for(int i=0;i<m;i++)
	{
		int rx = runner[i].first;
		int ry = runner[i].second;

		if(rx < x || rx >= x + length) continue;
		if(ry < y || ry >= y + length) continue;

		int nx = (rx - x);
		int ny = (ry - y);

		runner[i].first = x + ny;
		runner[i].second = y + (length - nx - 1);
	}
}
void play()
{
	moveRunner();
	if(isAllOut) return;

	int minLength = 1000;
	int sx = 1000, sy = 1000;
	for (int i = 0; i < m; i++)
	{
		if (isOut[i]) continue;

		int rx = runner[i].first;
		int ry = runner[i].second;

		int length = max(abs(ex - rx), abs(ey - ry)) + 1;
		
		if (minLength < length) continue;
		if (minLength > length)
		{
			pair<int,int> spos = getSquare(rx, ry, UP, length);
			sx = spos.first, sy = spos.second;
			spos = getSquare(rx, ry, LEFT, length);
			if (sx == spos.first)
			{
				sy = min(sy, spos.second);
			}
			else if (sx > spos.first)
			{
				sx = spos.first;
				sy = spos.second;
			}
			minLength = length;
		}
		else if (minLength == length)
		{
			pair<int, int> spos = getSquare(rx, ry, UP, length);
			if (sx == spos.first)
			{
				sy = min(sy, spos.second);
			}
			else if (sx > spos.first)
			{
				sx = spos.first;
				sy = spos.second;
			}
			spos = getSquare(rx, ry, LEFT, length);
			if (sx == spos.first)
			{
				sy = min(sy, spos.second);
			}
			else if (sx > spos.first)
			{
				sx = spos.first;
				sy = spos.second;
			}
		}
		//cout<<"runner "<<i<<" length "<<length<<" minLength "<<minLength<<" sx = "<<sx<<" sy = "<<sy<<endl;
	}
	if(sx == 1000 && sy == 1000) return;
	rotate(sx, sy, minLength);
}