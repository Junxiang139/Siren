/*
A Melusine can predict future.
Yet her futures are only something random. Though they may make her stronger.
Stronger enough to kill her fellow Siren.
Now she knows to count area like a Siren.
*/
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <list>
#include"jsoncpp/json.h"
using namespace std;
int n, m;
const int maxn = 25;
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};
bool invalid[maxn][maxn];
struct point {
	int x, y;
	point(int _x, int _y) {
		x = _x;
		y = _y;
	}
};
list<point> snake[2];
int possibleDire[10];
int posCount;
int allstep = 0;
int vis[55][55], ax[25005], ay[25005], vc = 0;
int lim = 8;
bool whetherGrow(int num) {
	if (num <= 9) return true;
	if ((num - 9) % 3 == 0) return true;
	return false;
}
void deleteEnd(int id) {
	snake[id].pop_back();
	return;
}
void move(int id, int dire, int num) {
	point p = *(snake[id].begin());
	int x = p.x + dx[dire];
	int y = p.y + dy[dire];
	snake[id].push_front(point(x, y));
	if (!whetherGrow(num)) {
		deleteEnd(id);
	}
	return;
}
void outputSnakeBody(int id) {
	cout << "Snake No." << id << endl;
	for (list<point>::iterator iter = snake[id].begin(); iter != snake[id].end(); ++iter) {
		cout << iter->x << " " << iter->y << endl;
	}
	cout << endl;
	return;
}
bool isInBody(int x, int y, int nd) {
	for (int id = 0; id <= 1; id++)
		for (list<point>::iterator iter = snake[id].begin(); iter != snake[id].end(); ++iter) {
			if (id == nd && iter == snake[id].begin())
				continue;
			if (x == iter->x && y == iter->y)
				return true;
		}
	return false;
}
/*
bool validDirection(int id,int k) {
	point p = *(snake[id].begin());
	int x = p.x + dx[k];
	int y = p.y + dy[k];
	if (x > n || y > m || x < 1 || y < 1) return false;
	if (invalid[x][y]) return false;
	if (isInBody(x, y)) return false;
	return true;
}
*/
int Rand(int p) {
	return rand() * rand() * rand() % p;
}

int countArea(int x, int y, int id) {
	vc++;
	int ans = 1, h = 0, t = 1, x1, y1;
	ax[1] = x, ay[1] = y;
	vis[x][y] = vc;
	while (h < t) {
		h++;
		x = ax[h], y = ay[h];
		for (int k = 0; k < 4; k++) {
			x1 = x + dx[k], y1 = y + dy[k];
			if (x1 > n || y1 > m || x1 < 1 || y1 < 1 || vis[x1][y1] == vc || invalid[x1][y1] || isInBody(x1, y1, id)) continue;
			ans++;
			t++;
			vis[x1][y1] = vc;
			ax[t] = x1, ay[t] = y1;
		}
	}
	return ans;
}

int dfs1(int x, int y, int z);
int dfs2(int x, int y, int z);

int dfs1(int x, int y, int z) {
	snake[0].push_front(point(x, y));
	point p1 = snake[0].back();
	if (!whetherGrow(allstep + z - 1)) {
		snake[0].pop_back();
	}
	int ans = 1000000000;
	point p = *(snake[1].begin());
	for (int k = 0; k < 4; k++) {
		int x1 = p.x + dx[k];
		int y1 = p.y + dy[k];
		ans = min(ans, dfs2(x1, y1, z));
	}
	if (!whetherGrow(allstep + z - 1)) {
		snake[0].push_back(p1);
	}
	snake[0].pop_front();
	return ans;
}
int dfs2(int x, int y, int z) {
	snake[1].push_front(point(x, y));
	point p1 = snake[1].back();
	if (!whetherGrow(allstep + z - 1)) {
		snake[1].pop_back();
	}
	int s1, s2;
	int x1, y1;
	point p = *(snake[0].begin());
	x1 = p.x, y1 = p.y;
	if (x1 > n || y1 > m || x1 < 1 || y1 < 1 || invalid[x1][y1] || isInBody(x1, y1, 0)) s1 = 0;
	else s1 = 1;
	p = *(snake[1].begin());
	x1 = p.x, y1 = p.y;
	if (x1 > n || y1 > m || x1 < 1 || y1 < 1 || invalid[x1][y1] || isInBody(x1, y1, 1)) s2 = 0;
	else s2 = 1;
	int ans = -1000000000;
	int doit = 1;
	if (s1 == 1 && s2 == 1 && z == lim) {
		ans = 11;
	} else if (s1 == 1 && s2 == 1) {
		p = *(snake[0].begin());
		if (double(clock())/double(CLOCKS_PER_SEC) > 0.9) {
			ans = 11;
		} else {
			for (int k = 0; k < 4; k++) {
				int x1 = p.x + dx[k];
				int y1 = p.y + dy[k];
				ans = max(ans, dfs1(x1, y1, z + 1));
			}
			doit = 0;
		}
	} else if (s1 == 1 && s2 == 0) {
		ans = 35;
	} else if (s1 == 0 && s2 == 1) {
		ans = -25;
	} else if (s1 == 0 && s2 == 0) {
		ans = 9;
	}
	int v1 = 0, v2 = 0;
	
	if (doit == 1) {
		p = *(snake[0].begin());
		x1 = p.x, y1 = p.y;
		v1 = countArea(x1, y1, 0);
	
		p = *(snake[1].begin());
		x1 = p.x, y1 = p.y;
		v2 = countArea(x1, y1, 1);
	
		ans *= (10 - z);
		ans += (v1 - v2);
	
		p = *(snake[0].begin());
		x1 = p.x, y1 = p.y;
		if (x1 == 1 || x1 == n || y1 == 1 || y1 == m) ans -= 20;
		ans -= abs(x1 - (n >> 1)) + abs(y1 - (m >> 1));
	}
	snake[1].pop_front();
	if (!whetherGrow(allstep + z - 1)) {
		snake[1].push_back(p1);
	}
	return ans;
}
int main() {
	memset(invalid, 0, sizeof(invalid));
	string str;
	string temp;
	while (getline(cin,temp)) str+=temp;
	Json::Reader reader;
	Json::Value input;
	reader.parse(str,input);
	n = input["requests"][(Json::Value::UInt) 0]["height"].asInt();
	m = input["requests"][(Json::Value::UInt) 0]["width"].asInt();
	int sum = n * m;
	int x = input["requests"][(Json::Value::UInt) 0]["x"].asInt();
	int ct[4], tc[4];
//int dx[4] = {-1, 0, 1, 0};
//int dy[4] = {0, 1, 0, -1};
	if (x == 1) {
		snake[0].push_front(point(1, 1));
		snake[1].push_front(point(n, m));
		dx[0] = 0, dx[1] = 1, dx[2] = 0, dx[3] = -1;
		dy[0] = 1, dy[1] = 0, dy[2] = -1, dy[3] = 0;
		ct[0] = 1, ct[1] = 2, ct[2] = 3, ct[3] = 0;
		tc[0] = 3, tc[1] = 0, tc[2] = 1, tc[3] = 2;
	} else {
		snake[1].push_front(point(1, 1));
		snake[0].push_front(point(n, m));
		dx[0] = 0, dx[1] = -1, dx[2] = 0, dx[3] = 1;
		dy[0] = -1, dy[1] = 0, dy[2] = 1, dy[3] = 0;
		ct[0] = 3, ct[1] = 0, ct[2] = 1, ct[3] = 2;
		tc[0] = 1, tc[1] = 2, tc[2] = 3, tc[3] = 0;
	}
	int obsCount = input["requests"][(Json::Value::UInt) 0]["obstacle"].size();
	sum -= obsCount;
	for (int i = 0; i < obsCount; i++) {
		int ox = input["requests"][(Json::Value::UInt) 0]["obstacle"][(Json::Value::UInt) i]["x"].asInt();
		int oy = input["requests"][(Json::Value::UInt) 0]["obstacle"][(Json::Value::UInt) i]["y"].asInt();
		invalid[ox][oy] = 1;
	}
	int total = input["responses"].size();
	int dire;
	for (int i = 0; i < total; i++) {
		dire = input["responses"][i]["direction"].asInt();
		move(0, tc[dire], i);
		dire = input["requests"][i + 1]["direction"].asInt();
		move(1, tc[dire], i);
		if (whetherGrow(i)) {
			sum--;
		}
	}
	if (sum > 97) {
		lim = 5;
	} else if (sum > 77) {
		lim = 6;
	} else if (sum > 67) {
		lim = 7;
	} else if (sum > 57) {
		lim = 8;
	} else if (sum > 47) {
		lim = 9;
	} else if (sum > 41) {
		lim = 10;
	} else if (sum > 35) {
		lim = 11;
	} else if (sum > 27) {
		lim = 12;
	} else if (sum > 21) {
		lim = 13;
	} else {
		lim = 14;
	}
	/*
	
	*/
	allstep = total;
	int territory[5];
	srand((unsigned)time(0) + total);
	for (int k = 0; k < 4; k++) {
		possibleDire[k] = k;
	}
	posCount = 4;
	point p = *(snake[0].begin());
	for (int k = 0; k < 4; k++) {
		int x = p.x + dx[k];
		int y = p.y + dy[k];
		territory[k] = dfs1(x, y, 1);
	}
	for (int i = 0; i < posCount - 1; i++) {
		for (int j = i + 1; j < posCount; j++) {
			if (territory[i] < territory[j]) {
				swap(territory[i], territory[j]);
				swap(possibleDire[i], possibleDire[j]);
			}
		}
	}
	Json::Value ret;
	int pos2 = 1;
	while (pos2 < posCount && territory[pos2 - 1] == territory[pos2]) {
		pos2++;
	}
	ret["response"]["direction"] = ct[possibleDire[rand() % pos2]];
	Json::FastWriter writer;
	cout << writer.write(ret) << endl;
	return 0;
}
