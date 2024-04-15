#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int board[70][70];
int R, C, K;
int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 };
int ans = 0;

vector<pair<int, int>> rockets;
vector<int> exitOfRocket;

void dropRocket(int);
void initBoard();
int calc_point(int);

void printBoard()
{
	cout << endl;
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
int main() {
	cin >> R >> C >> K;

	for (int i = 0; i < K; i++)
	{
		int col, dir;

		cin >> col >> dir;
		col--;

		rockets.push_back(make_pair(-2, col));
		exitOfRocket.push_back(dir);

		dropRocket(i);

		if (rockets[i].first <= 0)
		{
			initBoard();
			//printBoard();
			continue;
		}

		int row = rockets[i].first;
		col = rockets[i].second;
		board[row][col] = i + 1;

		for (int j = 0; j < 4; j++)
		{
			int nr = row + dx[j];
			int nc = col + dy[j];
			board[nr][nc] = i + 1;
		}

		//printBoard();
		int point = calc_point(i) + 1;
		ans += point;
	}

	cout << ans << endl;
	return 0;
}

bool tryDown(int row, int col)
{

	if (row + 1 >= R || row + 2 >= R) return false;
	if (row + 1 > 0 && board[row + 1][col - 1] >= 1) return false;
	if (row + 1 > 0 && board[row + 1][col + 1] >= 1) return false;
	if (row + 2 > 0 && board[row + 2][col] >= 1) return false;

	return true;
}

bool tryLeft(int row, int col)
{
	if (col - 1 < 0 || col - 2 < 0) return false;
	if (row - 1 > 0 && board[row - 1][col - 1] >= 1) return false;
	if (row + 1 > 0 && board[row + 1][col - 1] >= 1) return false;
	if (row > 0 && board[row][col - 2] >= 1) return false;

	return tryDown(row, col - 1);
}

bool tryRight(int row, int col)
{
	if (col + 1 >= C || col + 2 >= C) return false;
	if (row - 1 > 0 && board[row - 1][col + 1] >= 1) return false;
	if (row + 1 > 0 && board[row + 1][col + 1] >= 1) return false;
	if (row > 0 && board[row][col + 2] >= 1) return false;

	return tryDown(row, col + 1);
}

void dropRocket(int rocket)
{
	int row = rockets[rocket].first;
	int col = rockets[rocket].second;

	if (tryDown(row, col))
	{
		rockets[rocket].first += 1;
		dropRocket(rocket);
	}
	else if (tryLeft(row, col))
	{
		rockets[rocket].first += 1;
		rockets[rocket].second -= 1;
		int dir = exitOfRocket[rocket];
		exitOfRocket[rocket] = dir - 1 < 0 ? 3 : dir - 1;

		dropRocket(rocket);
	}
	else if (tryRight(row, col))
	{
		rockets[rocket].first += 1;
		rockets[rocket].second += 1;
		int dir = exitOfRocket[rocket];
		exitOfRocket[rocket] = dir + 1 > 3 ? 0 : dir + 1;

		dropRocket(rocket);
	}
}

int calc_point(int rocket)
{
	int maxRow = 0;
	int row = rockets[rocket].first;
	int col = rockets[rocket].second;

	vector<vector<int>> visited(R, vector<int>(C, 0));

	queue<pair<int, int>> q;
	q.push(make_pair(row, col));

	while (!q.empty())
	{
		int r = q.front().first;
		int c = q.front().second;
		int curRocket = board[r][c] - 1;

		int dir = exitOfRocket[curRocket];
		int exitR = rockets[curRocket].first + dx[dir];
		int exitC = rockets[curRocket].second + dy[dir];

		bool isExit = (r == exitR && c == exitC);
		q.pop();

		if (r > maxRow) maxRow = r;

		for (int i = 0; i < 4; i++)
		{
			int nr = r + dx[i];
			int nc = c + dy[i];

			if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
			if (visited[nr][nc] || board[nr][nc] == 0) continue;

			int nextRocket = board[nr][nc] - 1;
			if (curRocket == nextRocket)
			{
				visited[nr][nc] = 1;
				q.push(make_pair(nr, nc));
				continue;
			}

			if (!isExit) continue;

			visited[nr][nc] = 1;
			q.push(make_pair(nr, nc));
		}
	}

	return maxRow;
}

void initBoard()
{
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			board[i][j] = 0;
		}
	}
}