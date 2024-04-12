#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int board[10][10];
int recentAttack[10][10];
int N, M, k;

int dx[] = { 0,1,0,-1, 1,1,-1, - 1 };
int dy[] = { 1,0,-1,0, 1,-1,1,-1 };

void play(int);
void printBoard()
{
	cout << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
pair<int, int> getStrongTower();
pair<int, int> getWeakTower();
int main()
{
	cin >> N >> M >> k;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cin >> board[i][j];
		}
	}

	//printBoard();
	for (int i = 0; i < k; i++)
	{
		int cnt = 0;
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<M;j++)
			{
				if(board[i][j] > 0) cnt++;
			}
		}

		if(cnt < 2) break;
		play(i);
		//printBoard();
	}


	pair<int, int> pos = getStrongTower();
	cout << board[pos.first][pos.second] << endl;
	return 0;
}
pair<int, int> getWeakTower()
{
	int power = 10000;
	int x = 0;
	int y = 0;
	int recent = -10000;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (board[i][j] == 0) continue;
			if (power > board[i][j])
			{
				power = board[i][j];
				x = i;
				y = j;
				recent = recentAttack[i][j];
			}
			else if (power == board[i][j])
			{
				if (recentAttack[i][j] > recent)
				{
					power = board[i][j];
					x = i;
					y = j;
					recent = recentAttack[i][j];
				}
				else if (recentAttack[i][j] == recent)
				{
					if (x + y == i + j)
					{
						if (y < j)
						{
							power = board[i][j];
							x = i;
							y = j;
							recent = recentAttack[i][j];
						}
					}
					else if (x + y < i + j)
					{
						power = board[i][j];
						x = i;
						y = j;
						recent = recentAttack[i][j];
					}
				}
			}
		}
	}

	return make_pair(x, y);
}

pair<int, int> getStrongTower()
{
	int power = -10000;
	int x = 10;
	int y = 10;
	int recent = 10000;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (power < board[i][j])
			{
				power = board[i][j];
				x = i;
				y = j;
				recent = recentAttack[i][j];
			}
			else if (power == board[i][j])
			{
				if (recent > recentAttack[i][j])
				{
					power = board[i][j];
					x = i;
					y = j;
					recent = recentAttack[i][j];
				}
				else if (recent == recentAttack[i][j])
				{
					if (x + y == i + j)
					{
						if (y > j)
						{
							power = board[i][j];
							x = i;
							y = j;
							recent = recentAttack[i][j];
						}
					}
					else if (x + y > i + j)
					{
						power = board[i][j];
						x = i;
						y = j;
						recent = recentAttack[i][j];
					}
				}
			}
		}
	}

	return make_pair(x, y);
}
bool findPath = true;
vector<pair<int, int>> lazerAttack(pair<int, int>weakTower, pair<int, int> strongTower)
{
	int minDist = 10000;
	findPath = false;
	vector<vector<int>> visited(N, vector<int>(M, 0));
	visited[weakTower.first][weakTower.second] = 1;

	queue<pair<vector<pair<int, int>>, pair<int, pair<int, int>>>> q;
	q.push(make_pair(vector<pair<int, int>>(), make_pair(0, make_pair(weakTower.first, weakTower.second))));

	int destx = strongTower.first;
	int desty = strongTower.second;
	vector<pair<int, int>> minPath;
	while (!q.empty())
	{
		vector<pair<int, int>> path = q.front().first;
		int d = q.front().second.first;
		int x = q.front().second.second.first;
		int y = q.front().second.second.second;
		q.pop();
		path.push_back(make_pair(x, y));
		if (x == destx && y == desty && minDist > d)
		{
			findPath = true;
			minPath = path;
			minDist = d;
			continue;
		}

		if (d >= minDist) continue;

		for (int i = 0; i < 4; i++)
		{
			int nx = x + dx[i];
			int ny = y + dy[i];

			if (nx < 0) nx = N - 1;
			if (nx >= N) nx = 0;
			if (ny >= M) ny = 0;
			if (ny < 0) ny = M - 1;

			if (board[nx][ny] == 0) continue;
			if (visited[nx][ny]) continue;

			visited[nx][ny] = 1;
			q.push(make_pair(path, make_pair(d + 1, make_pair(nx, ny))));
		}
	}

	return minPath;
}
void play(int time)
{
	pair<int, int> weakTower = getWeakTower();
	pair<int, int> strongTower = getStrongTower();
	//cout<<"weak Tower " << weakTower.first << " " << weakTower.second<<" "<<board[weakTower.first][weakTower.second] << endl;
	//cout << "strong Tower " << strongTower.first << " " << strongTower.second<<" "<<board[strongTower.first][strongTower.second] << endl;

	board[weakTower.first][weakTower.second] += (N + M);
	recentAttack[weakTower.first][weakTower.second] = time + 1;


	vector<pair<int, int>> path = lazerAttack(weakTower, strongTower);

	int power = board[weakTower.first][weakTower.second];

	int size = path.size();
	for (int i = 1; i < size - 1; i++)
	{
		if (i >= path.size()) break;
		int x = path[i].first;
		int y = path[i].second;
		//cout << x << " " << y << endl;
		board[x][y] -= power / 2;
		if (board[x][y] < 0) board[x][y] = 0;
	}



	board[strongTower.first][strongTower.second] -= power;
	if (board[strongTower.first][strongTower.second] < 0) board[strongTower.first][strongTower.second] = 0;

	/*for (int i = 0; i < path.size(); i++)
	{
		cout << path[i].first << " " << path[i].second << endl;
	}
	cout << endl;*/

	if (path.size() == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			int x = strongTower.first;
			int y = strongTower.second;

			int nx = x + dx[i];
			int ny = y + dy[i];

			if (nx < 0) nx = N - 1;
			if (nx >= N) nx = 0;
			if (ny >= M) ny = 0;
			if (ny < 0) ny = M - 1;

			//cout << "nx " << nx << " ny " << ny << endl;
			board[nx][ny] -= power / 2;
			path.push_back(make_pair(nx, ny));
			if (board[nx][ny] < 0) board[nx][ny] = 0;
		}
	}

	vector<vector<int>> peaceTower(N, vector<int>(M, 1));
	for (int i = 0; i < path.size(); i++)
	{
		int x = path[i].first;
		int y = path[i].second;
		//cout << "x " << x << " y " << y << endl;
		peaceTower[x][y] = 0;
	}

	peaceTower[weakTower.first][weakTower.second] = 0;
	peaceTower[strongTower.first][strongTower.second] = 0;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (board[i][j] == 0) continue;
			if (peaceTower[i][j] == 1) board[i][j] += 1;
		}
	}

}