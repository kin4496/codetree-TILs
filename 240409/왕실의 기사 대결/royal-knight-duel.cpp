#include <iostream>
#include <vector>
using namespace std;

typedef pair<int, int> Pos;


int L;//chess board size
int N;// num of knight
int Q;// num of cmd

int board[40][40];
int origin[40][40];

int dx[] = {-1,0,1,0};
int dy[] = {0,1,0,-1};

vector<vector<Pos>> knights;
vector<int> knightOfHp;
vector<int> knightOfDmg;

void play(int,int);
bool isInValid(int, int);
void printBoard();


int main()
{
	cin >> L >> N >> Q;

	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			cin >> origin[i][j];
		}
	}
	knightOfDmg = vector<int>(N, 0);
	for (int i = 0; i < N; i++)
	{
		vector<Pos> pos;
		int x, y, h, w,k;
		cin >> x >> y >> h >> w >> k;
		x--;
		y--;
		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				pos.push_back(make_pair(x + j, y + k));
				board[x + j][y + k] = i + 3;
			}
		}
		knights.push_back(pos);
		knightOfHp.push_back(k);
	}

	//printBoard();

	for (int i = 0; i < Q; i++)
	{
		int knight, dir;
		cin >> knight >> dir;
		knight--;
		play(knight,dir);
		//printBoard();
	}
	int ans = 0;

	for (int i = 0; i < N; i++)
	{
		if (knightOfHp[i] <= 0) continue;
		ans += knightOfDmg[i];
	}
	cout << ans << endl;

	return 0;
}

void getCollisionKnight(int knight, int dir,vector<int>&collisionKnight)
{
	vector<int> nearKnights = vector<int>(N,0);
	for (int i = 0; i < knights[knight].size(); i++)
	{
		int x = knights[knight][i].first;
		int y = knights[knight][i].second;

		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (isInValid(nx, ny)) continue;//change return
		if (board[nx][ny]<3) continue;
		
		int nearKnight = board[nx][ny] - 3;
		//cout << "near " << nearKnight << endl;
		if (knightOfHp[nearKnight] <= 0) continue;

		nearKnights[nearKnight] = 1;
	}

	for (int i = 0; i < N; i++)
	{
		if (nearKnights[i] == 0 || i == knight) continue;
		getCollisionKnight(i, dir, collisionKnight);
	}

	for (int i = 0; i < N; i++)
	{
		if (nearKnights[i] == 0 || i == knight) continue;
		collisionKnight.push_back(i);
	}
}

bool canMove(int knight, int dir)
{
	for (int i = 0; i < knights[knight].size(); i++)
	{
		int x = knights[knight][i].first;
		int y = knights[knight][i].second;

		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (isInValid(nx, ny)) return false;
		if (origin[nx][ny] == 2) return false;
	}
	return true;
}

void goMove(int knight, int dir)
{
	for (int i = 0; i < knights[knight].size(); i++)
	{
		int x = knights[knight][i].first;
		int y = knights[knight][i].second;
		board[x][y] = 0;
	}

	for (int i = 0; i < knights[knight].size(); i++)
	{
		int x = knights[knight][i].first;
		int y = knights[knight][i].second;

		int nx = x + dx[dir];
		int ny = y + dy[dir];
		board[nx][ny] = knight + 3;

		knights[knight][i].first = nx;
		knights[knight][i].second = ny;
	}
}

int calcDamage(vector<int>&collisionKnight)
{
	int ret = 0;

	for (int i = 0; i < collisionKnight.size(); i++)
	{
		int knight = collisionKnight[i];
		int damage = 0;
		for (int j = 0; j < knights[knight].size(); j++)
		{
			int x = knights[knight][j].first;
			int y = knights[knight][j].second;

			if (origin[x][y] == 1)
			{
				damage++;
			}
		}
		knightOfDmg[knight] += damage;
		ret += damage;
		knightOfHp[knight] -= damage;
		knightOfHp[knight] = knightOfHp[knight] < 0 ? 0 : knightOfHp[knight];
	}

	return ret;
}

void play(int knight, int dir)
{
	if (knightOfHp[knight] <= 0) return;

	vector<int> collisionKnights;
	getCollisionKnight(knight, dir, collisionKnights);
	
	bool allCanGo = true;
	int size = collisionKnights.size();
	for (int i = size - 1; i >= 0; i--)
	{
		int goKnight = collisionKnights[i];
		if (!canMove(goKnight, dir))
		{
			allCanGo = false;
			break;
		}
	}

	if (!allCanGo) return;

	for (int i = size - 1; i >= 0; i--)
	{
		int goKnight = collisionKnights[i];
		goMove(goKnight, dir);
	}

	if (canMove(knight, dir)) goMove(knight, dir);

	calcDamage(collisionKnights);
}

bool isInValid(int x, int y)
{
	return (x < 0 || x >= L || y < 0 || y >= L);
}

void printBoard()
{
	cout << endl;

	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
		{
			if (board[i][j] >= 3 && knightOfHp[board[i][j]-3] > 0)
			{
				cout << board[i][j] - 2 << " ";
			}
			else if (origin[i][j] == 1)
			{
				cout << "T ";
			}
			else if (origin[i][j] == 2)
			{
				cout << "W ";
			}
			else if (board[i][j] >= 3 && knightOfHp[board[i][j] - 3] <= 0)
			{
				cout << "0 ";
			}
			else
				cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}