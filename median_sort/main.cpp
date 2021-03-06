#include <bits/stdc++.h>

using namespace std;

int query(int a, int b, int c) { // queries the median of {a,b,c}
    cout << a << " " << b << " " << c << endl;
    int l;
    cin >> l;
    assert(l > 0);
    return l;
}

int guess(vector<int> &arr) {
    for (auto i : arr) {
        cout << i << " ";
    }
    cout << endl;
    int ret;
    cin >> ret;
    return ret;
}

vector<int> solve_partition(vector<int> arr) {
    if (arr.size() <= 1) {
        return arr;
    } else {
        int ref1 = arr[0];
        int ref2 = arr[1];
        vector<int> mid;
        vector<int> left;
        vector<int> right;

        // 3 partitions
        for (int i = 2; i < arr.size(); ++i) {
            int val = arr[i];
            int response = query(ref1, ref2, val);
            if (response == val) {
                mid.push_back(val);
            } else if (response == ref1) {
                left.push_back(val);
            } else if (response == ref2) {
                right.push_back(val);
            } else {
                assert(false);
            }
        }

        mid = solve_partition(mid);
        left = solve_partition(left);
        right = solve_partition(right);

        // adjust
        if (mid.size() > 1) {
            int response = query(ref1, mid[0], mid[1]);
            if (response != mid[0]) {
                reverse(mid.begin(), mid.end());
            }
        }
        if (left.size() > 1) {
            int response = query(left[0], left[1], ref1);
            if (response != left[1]) {
                reverse(left.begin(), left.end());
            }
        }
        if (right.size() > 1) {
            int response = query(ref2, right[0], right[1]);
            if (response != right[0]) {
                reverse(right.begin(), right.end());
            }
        }

        // collect
        vector<int> ret;
        for (auto i : left) {
            ret.push_back(i);
        }
        ret.push_back(ref1);
        for (auto i : mid) {
            ret.push_back(i);
        }
        ret.push_back(ref2);
        for (auto i : right) {
            ret.push_back(i);
        }
        return ret;
    }
}

vector<int> solve_iterative(int n) {

    vector<int> ret;
    int response = query(1, 2, 3);

    if (response == 2) {
        ret = {0, 1, 2};
    } else if (response == 1) {
        ret = {1, 0, 2};
    } else if (response == 3) {
        ret = {0, 2, 1};
    } else {
        assert(false);
    }

    // process remaining indices of input
    for (int i = 3; i < n; ++i) {

        int val = i + 1; // adjust for 1-index

        // bounding indices
        int left = 0;
        int right = ret.size();

        // bounding search
        while (left < right) {
            int count = right - left;

            // partition into 3 parts: [left, ref1), [ref1, ref2), [ref2, right)
            int ref1 = left + count / 3;
            int ref2 = left + count * 2 / 3;

            if (ref2 == ref1) {
                if (ref1 > 0) {
                    ref1--;
                } else {
                    ref2++;
                }
            }

            int val_ref1 = ret[ref1] + 1;
            int val_ref2 = ret[ref2] + 1;

            assert(val_ref1 >= 1 && val_ref1 <= n);
            assert(val_ref2 >= 1 && val_ref2 <= n);

            // determine to which partition val belongs
            int q = query(val_ref1, val_ref2, val);
            if (q == val) { // falls into mid partition
                left = ref1 + 1;
                right = ref2;
            } else if (q == val_ref1) { // falls into left partition
                right = ref1;
            } else if (q == val_ref2) { // falls into right partition
                left = ref2 + 1;
            } else {
                assert(false);
            }
        }

        // insert into current partition [left,right)
        ret.insert(ret.begin() + right, val - 1);
    }

    assert(ret.size() == n);

    // adjust for 1-indexing
    for (auto &i : ret) {
        i++;
    }
    return ret;
}

int main() {
    int t, n, q;
    cin >> t >> n >> q;
    vector<int> arr(n);
    for (int i = 0; i < t; ++i) {

        // vector<int> arr(n);
        // iota(arr.begin(), arr.end(), 1);
        // vector<int> ans = solve_partition(arr);

        // alternative
        auto ans = solve_iterative(n);
        if (1 != guess(ans)) {
            break;
        }
    }
    return 0;
}
