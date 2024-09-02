#include <iostream>
using namespace std;

struct node_info {
	int parent;
	int childnode[2];
	bool alarm_setting;
	int auth;
};
node_info tree_data[100001];
int cnt;
int N, Q; // 채팅방의 수, 공백의 수
void init() {
	cin >> N >> Q;
}
void dfs(int depth, int node) {

	if (depth >= 20) return;
	
	for (int i = 0; i < 2; i++) {
		int next_node = tree_data[node].childnode[i];
		if (next_node == 0) continue; // 마지막 노드 도달
		// 알람이 켜져있고, 깊이 보다 권한이 크거나 같아야한다.
		if (tree_data[next_node].alarm_setting == true) { // 알람 설정 되있으면 탐색 가능
			if (depth < tree_data[next_node].auth) { // 파워가 닿으면 수신 가능
				cnt++;
				dfs(depth + 1, next_node);
			}
			else { // 파워가 안닿으면 다음 노드 아래 자식들 dfs로 돌림
				dfs(depth + 2, tree_data[next_node].childnode[0]);
				dfs(depth + 2, tree_data[next_node].childnode[1]);
			}
			
		}
	}
}

int main() {
	// 여기에 코드를 작성해주세요.
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	init();
	for (int t = 0; t < Q; t++) {
		int order;
		cin >> order;
		int tmp, c1_parent, c2_parent;
		switch (order)
		{
			//1)메신저 준비
		case 100:
			tree_data[0].alarm_setting = true;
			tree_data[0].childnode[0] = 0;
			tree_data[0].childnode[1] = 0;
			for (int i = 1; i <= N; i++) {
				cin >> tree_data[i].parent;
				tree_data[i].alarm_setting = true;
				tree_data[i].childnode[0] = 0;
				tree_data[i].childnode[1] = 0;
			}
			for (int i = 1; i <= N; i++) {
				cin >> tree_data[i].auth;
			}
			for (int i = 1; i <= N; i++) {
				int parent = tree_data[i].parent;
				if (tree_data[parent].childnode[0] == 0) {
					tree_data[parent].childnode[0] = i;
				}
				else {
					tree_data[parent].childnode[1] = i;
				}

			}
			break;
			//2)알림망 설정 On Off 토글
		case 200:
			int c;
			cin >> c;
			tree_data[c].alarm_setting = tree_data[c].alarm_setting == true ? false : true;
			break;
			//3)권한 세기의 변경
		case 300:
			int c0, power;
			cin >> c0 >> power;
			tree_data[c0].auth = power;
			break;
			//4)부모 채팅방 교환
		case 400:
			int c1, c2;
			cin >> c1 >> c2;

			c1_parent = tree_data[c1].parent;
			c2_parent = tree_data[c2].parent;
			// c1과 c2의 부모가 가지는 left node 또는 right node가 바뀜
			if (tree_data[c1_parent].childnode[0] == c1) {
				tree_data[c1_parent].childnode[0] = c2;
			}
			else {
				tree_data[c1_parent].childnode[1] = c2;
			}
			if (tree_data[c2_parent].childnode[0] == c2) {
				tree_data[c2_parent].childnode[0] = c1;
			}
			else {
				tree_data[c2_parent].childnode[1] = c1;
			}
			// c1의 부모와 c2의 부모가 바뀜
			tmp = tree_data[c1].parent;
			tree_data[c1].parent = tree_data[c2].parent;
			tree_data[c2].parent = tmp;
			break;
			//5)알림을 받을수 있는 채팅방 수 조회
		case 500:
			int c3;
			cin >> c3;
			cnt = 0;
			dfs(0, c3);

			cout << cnt << "\n";
			break;
		default:
			break;
		}
	}

	return 0;
}