#include <bits/stdc++.h>

using namespace std;

vector<int8_t> convert(int v) {
    vector<int8_t> ret;
    while (v) {
        int d = v % 10;
        v = v / 10;
        ret.push_back(d);
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

bool geq(vector<int8_t> const &a, vector<int8_t> const &b) {
    assert(a.size() > 0);
    assert(b.size() > 0);
    if (a.size() == b.size()) {
        for (int i = 0; i < a.size(); ++i) {
            if (a[i] > b[i]) {
                return true; // bigger
            } else if (a[i] < b[i]) {
                return false; // smaller
            }
        }
        return true; // equal
    } else {
        return a.size() > b.size();
    }
}

vector<int8_t> add(vector<int8_t> in) {
    bool carry = true;
    for (int i = 0; i < in.size(); ++i) {
        int j = in.size() - 1 - i;
        if (!carry) {
            break;
        } else {
            in[j]++;
            if (in[j] == 10) {
                in[j] = 0;
            } else {
                carry = false;
            }
        }
    }
    if (carry) {
        in.insert(in.begin(), 1);
    }
    return in;
}

ostream &operator<<(ostream &os, vector<int8_t> const &arr) {
    for (int i = 0; i < arr.size(); ++i) {
        os << (int)arr[i];
    }
    return os;
}
pair<vector<int8_t>, int64_t> update(vector<int8_t> const &cur,
                                     vector<int8_t> const &atleast) {
    // make cur >= atleast by appending digits to the right
    // cur is guaranteed to be < atleast
    int64_t ops{0};
    vector<int8_t> ret = cur;

    if (atleast.size() == cur.size()) {
        ret.push_back(0); // append 0 to the right
        ops = 1;
    } else if (atleast.size() > cur.size()) {
        // check prefix:
        bool is_bigger = false;
        bool is_smaller = false;
        int common = min(atleast.size(), cur.size());
        for (int i = 0; i < common; ++i) {
            int a = cur[i];
            int b = atleast[i];
            if (a > b) {
                // bigger
                is_bigger = true;
                break;
            } else if (a < b) {
                // smaller
                is_smaller = true;
                break;
            }
            // equal so far
        }
        if (is_bigger) {
            // append 0s
            for (int i = 0; i < atleast.size() - cur.size(); ++i) {
                ret.push_back(0);
            }
            ops = atleast.size() - cur.size();
        } else if (is_smaller) {
            // append an extra 0
            for (int i = 0; i < atleast.size() - cur.size() + 1; ++i) {
                ret.push_back(0);
            }
            ops = atleast.size() - cur.size() + 1;
        } else {
            // equal
            // set it to atleast
            ops = atleast.size() -
                  cur.size(); // append digits to make it equal to atleast
            ret = atleast;
        }
    } else {
        assert(false);
    }

    return {ret, ops};
}
int64_t solve(vector<int> &arr) {
    int64_t ret{0};
    assert(arr.size() >= 2);

    vector<vector<int8_t>> arr_v;
    for (auto i : arr) {
        auto converted = convert(i);
        arr_v.push_back(converted);
    }

    auto last = arr_v[0];

    for (int i = 1; i < arr_v.size(); ++i) {
        auto cur = arr_v[i];
        // cur need to be >= atleast
        auto atleast = add(last); // increment last by 1
        if (!geq(cur, atleast)) {
            // cur need to be updated to be >= atleast
            auto [cur_updated, ops] = update(cur, atleast);
            cur = cur_updated;
            ret += ops;
        }
        last = cur;
    }
    return ret;
}

int main() {
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; ++i) {
        int n;
        cin >> n;
        vector<int> arr(n);
        for (auto &x : arr) {
            cin >> x;
        }
        auto x = solve(arr);
        cout << "Case #" << i + 1 << ": " << x << endl;
    }
    return 0;
}
