/*
A Lamia is far more clever from a normal Siren.
She would try to go to somewhere more wide, so her kind always has more territory.
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
const int dx[4]={-1, 0, 1, 0};
const int dy[4]={0, 1, 0, -1};
bool invalid[maxn][maxn];
int cnt[maxn][maxn], now = 0;
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
bool isInBody(int x, int y) {
	for (int id = 0; id <= 1; id++)
		for (list<point>::iterator iter = snake[id].begin(); iter != snake[id].end(); ++iter)
			if (x == iter->x && y == iter->y)
				return true;
	return false;
}
bool validDirection(int id,int k) {
	point p = *(snake[id].begin());
	int x = p.x + dx[k];
	int y = p.y + dy[k];
	if (x > n || y > m || x < 1 || y < 1) return false;
	if (invalid[x][y]) return false;
	if (isInBody(x, y)) return false;
	return true;
}
int Rand(int p) {
	return rand() * rand() * rand() % p;
}
int countArea(int x, int y) {
	int ans = 1, x1, y1;
	cnt[x][y] = now;
	for (int k = 0; k < 4; k++) {
		x1 = x + dx[k], y1 = y + dy[k];
		if (x1 > n || y1 > m || x1 < 1 || y1 < 1 || cnt[x1][y1] == now || invalid[x1][y1] || isInBody(x1, y1)) continue;
		ans += countArea(x1, y1);
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
	n=input["requests"][(Json::Value::UInt) 0]["height"].asInt();
	m=input["requests"][(Json::Value::UInt) 0]["width"].asInt();
	int x=input["requests"][(Json::Value::UInt) 0]["x"].asInt();
	if (x==1) {
		snake[0].push_front(point(1, 1));
		snake[1].push_front(point(n, m));
	} else {
		snake[1].push_front(point(1, 1));
		snake[0].push_front(point(n, m));
	}
	int obsCount=input["requests"][(Json::Value::UInt) 0]["obstacle"].size();
	for (int i = 0; i < obsCount; i++) {
		int ox=input["requests"][(Json::Value::UInt) 0]["obstacle"][(Json::Value::UInt) i]["x"].asInt();
		int oy=input["requests"][(Json::Value::UInt) 0]["obstacle"][(Json::Value::UInt) i]["y"].asInt();
		invalid[ox][oy]=1;
	}
	int total = input["responses"].size();
	int dire;
	for (int i = 0; i < total; i++) {
		dire = input["responses"][i]["direction"].asInt();
		move(0, dire, i);
		dire=input["requests"][i + 1]["direction"].asInt();
		move(1, dire, i);	
	}
	if (!whetherGrow(total)) {
		deleteEnd(0);
		deleteEnd(1);
	}
	int territory[5];
	for (int k = 0; k < 4; k++) {
		if (validDirection(0, k)) {
			possibleDire[posCount++] = k;
			point p = *(snake[0].begin());
			int x = p.x + dx[k];
			int y = p.y + dy[k];
			now++;
			territory[posCount - 1] = countArea(x, y);
		}
	}
	srand((unsigned)time(0)+total);
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
	ret["response"]["direction"] = possibleDire[rand() % pos2];
	Json::FastWriter writer;
	cout << writer.write(ret) << endl;
	return 0;
}
