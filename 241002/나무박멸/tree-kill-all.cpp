#include <iostream>
#include <vector>
#include <deque>
using namespace std;

int N, M, K, C;  // 격자크기, 턴수, 제초제 대각선 범위, 제초제 유지 기간
const int dy[] = { -1,0,1,0 };
const int dx[] = { 0,-1,0,1};
const int h_dy[] = { -1,1,-1,1 };
const int h_dx[] = { -1,-1,1,1 };
int result = 0; // m년 동안 박멸한 나무의 그루수
struct info {
	int now_c = 0;
	vector<vector<int>> cut_map = vector<vector<int>>(N, vector<int>(N, 0));
};
vector<vector<int>> field;
vector<vector<int>> herbicide_map;
deque<info> herbicide; // 굳이 이런식으로 안하고 맵을 만들고 점점 낮아지는 방향으로 해도 될듯
void tree_grow() {
	vector<vector<int>> plus_map = vector<vector<int>>(N, vector<int>(N, 0));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (field[i][j] <= 0) continue;
			int cnt = 0;
			for (int z = 0; z < 4; z++) {
				int ny = i + dy[z];
				int nx = j + dx[z];
				if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
				if (field[ny][nx] > 0) {
					cnt++;
				}
			}
			plus_map[i][j] = cnt;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			field[i][j] += plus_map[i][j];
		}
	}
}
void tree_breed() {
	// 제초제 없고 벽이 아니고 갈곳이 있다면 n빵 한거를 해당 칸에 추가함
	// 제초제 맵 만들기
	herbicide_map = vector<vector<int>>(N, vector<int>(N, 0));
	for (int i = 0; i < herbicide.size(); i++) {
		for (int y = 0; y < N; y++) {
			for (int x = 0; x < N; x++) {
				if (herbicide[i].cut_map[y][x] == 1) {
					herbicide_map[y][x] = 1;
				}
			}
		}
	}
	vector<vector<int>> plus_map = vector<vector<int>>(N, vector<int>(N, 0));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (field[i][j] <= 0) continue;
			int cnt = 0;
			vector<pair<int, int>> vc;
			for (int z = 0; z < 4; z++) {
				int ny = i + dy[z];
				int nx = j + dx[z];
				if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
				if (herbicide_map[ny][nx] == 1) continue;
				if (field[ny][nx] == 0) {
					vc.push_back({ ny,nx });
					cnt++;
				}
			}
			if (cnt == 0) continue;
			int plus_tree = field[i][j]/cnt;
			for (int z = 0; z < vc.size(); z++) {
				plus_map[vc[z].first][vc[z].second] += plus_tree;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			field[i][j] += plus_map[i][j];
		}
	}
}
int get_score(int y,int x) {
	int cnt = 0;
	cnt += field[y][x];
	for (int d = 0; d < 4; d++) {
		for (int i = 1; i <= K; i++) {
			int ny = y + i * h_dy[d];
			int nx = x + i * h_dx[d];
			if (ny < 0 || nx < 0 || ny >= N || nx >= N) break;
			if (field[ny][nx] == -1) break;
			if (field[ny][nx] == 0) break;
			cnt += field[ny][nx];
		}
	}
	return cnt;
}
void herbicide_spread() {
	//대각선으로 K만큼 했을때 가장 많이 죽이는 칸 찾기. 해당 칸 0으로 바꾸고 제초제 맵 저장.
	int max = 0;
	pair<int, int> best_location;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (field[i][j] > 0) {
				int tmp = get_score(i,j);
				if (tmp > max) {
					max = tmp;
					best_location = { i,j };
				}
			}
		}
	}
	vector<vector<int>> cut_map = vector<vector<int>>(N, vector<int>(N, 0));
	cut_map[best_location.first][best_location.second] = 1;
	field[best_location.first][best_location.second] = 0;
	for (int d = 0; d < 4; d++) {
		for (int i = 1; i <= K; i++) {
			int ny = best_location.first + i * h_dy[d];
			int nx = best_location.second + i * h_dx[d];
			if (ny < 0 || nx < 0 || ny >= N || nx >= N) break;
			if (field[ny][nx] == -1) break;
			if (field[ny][nx] == 0) { // 해당 칸 까지는 뿌려짐
				cut_map[ny][nx] = 1;
				break;
			}
			cut_map[ny][nx] = 1;
			field[ny][nx] = 0;
		}
	}
	herbicide.push_back({ 0,cut_map });
	result += max;

}
void init() {
	cin >> N >> M >> K >> C;
	field = vector<vector<int>>(N, vector<int>(N, 0));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> field[i][j];
		}
	}
}
void dbg_print() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << field[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

int main() {
	init();
	for (int t = 0; t < M; t++) {
		tree_grow();
		tree_breed();
		herbicide_spread();
		//dbg_print();
		// 제초제 기한 끝났다면 맵 삭제
		if (herbicide.size() == 0) continue;
		else {
			if (herbicide[0].now_c == C) {
				herbicide.pop_front();
			}
		}
		for (int i = 0; i < herbicide.size(); i++) {
			herbicide[i].now_c++;
		}
	}
	cout << result;
}