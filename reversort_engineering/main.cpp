#include <bits/stdc++.h>

using namespace std;

ostream &operator<<(ostream &o, vector<int> &arr) {
    for (auto i : arr) {
        o << i << " ";
    }
    return o;
}

int main() {
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; ++i) {
        int n, c;
        scanf("%d %d", &n, &c);
        if (c < (n - 1) || c > (n * (n + 1) / 2 - 1)) { // check feasibility
            printf("Case #%d: IMPOSSIBLE\n", i + 1);
        } else {
            // guaranteed to be solvable
            vector<int> arr(n);
            iota(arr.begin(), arr.end(), 1); // make original sorted sequence

            int left = -1;
            int right = n;
            int index = n - 2;
            bool rotate_left = true;

            for (int cc = 0; cc < c - (n - 1); ++cc) { // make c-(n-1) rounds

                assert(index > left && index < right);

                if (rotate_left) {
                    // rotate values in [index,right) left
                    rotate(arr.begin() + index, arr.begin() + index + 1,
                           arr.begin() + right);
                    index--;
                } else {
                    // rotate values in (left,index] right
                    rotate(arr.begin() + (left + 1), arr.begin() + index,
                           arr.begin() + index + 1);
                    index++;
                }

                if (index == left) {
                    // alternate rotation direction and constrain range to
                    // (left,right-1) when it hits end
                    right--;
                    index = left + 2;
                    rotate_left = !rotate_left;

                } else if (index == right) {
                    // alternate rotation direction and constrain range to
                    // (left+1,right) when it hits end
                    left++;
                    index = right - 2;
                    rotate_left = !rotate_left;
                }
                // cout << "round: " << cc << ": " << arr << endl;
            }

            // printf("1\n");
            // printf("%d\n", (int)arr.size());

            printf("Case #%d: ", i + 1);
            for (auto &x : arr) {
                printf("%d ", x);
            }
            printf("\n");
        }
    }
    return 0;
}
