#include <bits/stdc++.h>

using namespace std;

int main() {
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; ++i) {
        int n;
        scanf("%d", &n);
        vector<int> arr(n);
        for (auto &j : arr) {
            scanf("%d", &j);
        }
        int ans = 0;
        for (int ii = 0; ii < n - 1; ++ii) {
            int jj = ii;
            for (int k = ii + 1; k < n; ++k) {
                if (arr[k] < arr[jj]) {
                    jj = k;
                }
            }
            // reverse
            reverse(&arr[ii], &arr[jj + 1]);
            ans += jj + 1 - ii;
        }
        printf("Case #%d: %d\n", i + 1, ans);
    }
    return 0;
}
