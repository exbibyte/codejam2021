#include <bits/stdc++.h>

using namespace std;

void inner(int x, int y, string &s, int idx, vector<vector<int>> &cost,
           vector<vector<bool>> &feasible) {
    if (idx < 0) {
        return;
    } else {
        if (idx + 1 < s.size()) { // look at idx and idx+1
            char current = s[idx];
            for (int i = 0; i < 2; ++i) { // 0->C, 1->J
                if (feasible[i][idx + 1]) {
                    if (i == 0) { //_C
                        if (current == '?') {
                            // ?C

                            // JC
                            if (cost[0][idx + 1] !=
                                numeric_limits<int>::max()) {
                                cost[1][idx] =
                                    min(cost[1][idx], cost[0][idx + 1] + y);
                            }

                            // CC
                            cost[0][idx] = min(cost[0][idx], cost[0][idx + 1]);

                        } else if (current == 'J') {
                            // JC
                            if (cost[0][idx + 1] !=
                                numeric_limits<int>::max()) {
                                cost[1][idx] =
                                    min(cost[1][idx], cost[0][idx + 1] + y);
                            }
                        } else {
                            // CC
                            cost[0][idx] = min(cost[0][idx], cost[0][idx + 1]);
                        }
                    } else { //_J
                        if (current == '?') {
                            // ?J

                            // CJ
                            if (cost[1][idx + 1] !=
                                numeric_limits<int>::max()) {
                                cost[0][idx] =
                                    min(cost[0][idx], cost[1][idx + 1] + x);
                            }

                            // JJ
                            cost[1][idx] = min(cost[1][idx], cost[1][idx + 1]);

                        } else if (current == 'C') {
                            // CJ
                            if (cost[1][idx + 1] !=
                                numeric_limits<int>::max()) {
                                cost[0][idx] =
                                    min(cost[0][idx], cost[1][idx + 1] + x);
                            }
                        } else {
                            // JJ
                            cost[1][idx] = min(cost[1][idx], cost[1][idx + 1]);
                        }
                    }
                } else {
                    // pass
                }
            }
        }
        return inner(x, y, s, idx - 1, cost, feasible);
    }
}

int solve(int x, int y, string &s) {
    vector<vector<bool>> feasible(2, vector<bool>(s.size(), false));

    vector<vector<int>> cost(2,
                             vector<int>(s.size(), numeric_limits<int>::max()));
    cost[0][s.size() - 1] = 0;
    cost[1][s.size() - 1] = 0;

    int idx = 0;
    for (int i : s) {
        if (i == 'C') {
            feasible[0][idx] = true;
        } else if (i == 'J') {
            feasible[1][idx] = true;
        } else {
            assert(i == '?');
            feasible[0][idx] = true;
            feasible[1][idx] = true;
        }
        idx++;
    }
    inner(x, y, s, s.size() - 1, cost, feasible);
    return min(cost[0][0], cost[1][0]);
}

int main() {
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; ++i) {
        int x, y;
        char s[1001];
        scanf("%d %d %s", &x, &y, s);
        string ss(s);
        int ret = solve(x, y, ss);
        printf("Case #%d: %d\n", i + 1, ret);
    }
    return 0;
}
