#include <bits/stdc++.h>
using namespace std;
#define int long long
int n, m, s, t;
struct edge {
  int v, next, val;
} line[10010];
int cnt = 1, head[210], cur[210];
void add(int u, int v, int w) {
  cnt++;
  line[cnt].v = v;
  line[cnt].val = w;
  line[cnt].next = head[u];
  head[u] = cnt;
}
int depth[210];

bool bfs() {
  std::queue<int> q;
  memset(depth, -1, sizeof(depth));
  q.push(s);
  depth[s] = 0;
  while (!q.empty()) {
    int u = q.front();
    cur[u] = head[u];
    q.pop();
    for (int i = head[u]; i; i = line[i].next) {
      int v = line[i].v;
      if (depth[v] < 0 && line[i].val > 0) {
        depth[v] = depth[u] + 1;
        q.push(v);
      }
    }
  }
  return depth[t] >= 0;
}

int dfs(int u, int flow) {
  int allowflow = 0;
  if (u == t) {
    return flow;
  }
  for (int i = cur[u]; i && flow; i = line[i].next) {
    cur[u] = i;
    int v = line[i].v;
    if (depth[v] == depth[u] + 1 && line[i].val > 0) {
      int tmp = dfs(v, std::min(flow, line[i].val));
      if (tmp == 0) {
        depth[v] = -1;
      }
      line[i].val -= tmp;
      line[i ^ 1].val += tmp;
      allowflow += tmp;
      flow -= tmp;
    }
  }
  return allowflow;
}

int dinic() {
  int ans = 0;
  while (bfs()) {
    ans += dfs(s, LLONG_MAX);
  }
  return ans;
}
