#include <bits/stdc++.h>

using namespace std;

string stringify(__int128 v) {
    string s;
    while (v != 0) {
        char c = (char)(v % 10) + '0';
        s.push_back(c);
        v /= 10;
    }
    reverse(s.begin(), s.end());
    if (s.empty()) {
        s.push_back('0');
    }
    return s;
}

int main() {

    // n choose x precomputation
    vector<vector<__int128>> comb_table(121, vector<__int128>(121, 0));
    comb_table[0][0] = 1;
    for (int i = 1; i <= 120; ++i) {
        comb_table[i][0] = 1;
        comb_table[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            comb_table[i][j] = comb_table[i - 1][j - 1] + comb_table[i - 1][j];
        }
    }

    auto n_choose_x = [&](int n, int x) -> __int128 {
        assert(n >= 0);
        assert(x >= 0);
        assert(x <= n);
        return comb_table[n][x];
    };
    assert(n_choose_x(3, 1) == __int128{3});
    assert(n_choose_x(3, 3) == __int128{1});
    assert(n_choose_x(3, 0) == __int128{1});
    assert(n_choose_x(4, 2) == __int128{6});
    assert(n_choose_x(120, 7) == __int128(59487568920));
    assert(stringify(__int128(345)) == string("345"));
    int t;
    cin >> t;
    for (int _ = 0; _ < t; ++_) {
        int n, q;
        cin >> n >> q;
        vector<string> answers(3); // 3 scores for students as strings
        vector<int> score(3, 0);   // student -> score
        assert(n <= 3);
        vector<int> q_type(q);  // question -> question type
        vector<int> freq(4, 0); // question type -> freq

        for (int j = 0; j < n; ++j) { // per student
            cin >> answers[j];
            cin >> score[j];
            assert(answers[j].size() == q);
            assert(score[j] >= 0 && score[j] <= q);
        }

        // if (n == 1) {
        //     if (score[0] <= q / 2) {
        //         string flipped;
        //         for (auto i : answers[0]) {
        //             flipped.push_back((i == 'T') ? 'F' : 'T');
        //         }
        //         cout << "Case #" << _ + 1 << ": " << flipped << " "
        //              << q - score[0] << "/"
        //              << "1" << endl;
        //     } else {
        //         cout << "Case #" << _ + 1 << ": " << answers[0] << " "
        //              << score[0] << "/"
        //              << "1" << endl;
        //     }
        //     continue;
        // }

        // if (n == 2) {
        //     int numSame = 0, numDiff = 0;
        //     for (int i = 0; i < q; i++) {
        //         if (answers[0][i] == answers[1][i])
        //             numSame++;
        //         else
        //             numDiff++;
        //     }
        //     int aDiff = max(score[0] - score[1], 0);
        //     int bDiff = max(score[1] - score[0], 0);
        //     while (aDiff + bDiff != numDiff) {
        //         aDiff++;
        //         bDiff++;
        //     }
        //     int same = score[0] - aDiff;
        //     // System.out.println(aDiff + " " + bDiff + " " + same);
        //     int score = 0;
        //     cout << "Case #" << _ + 1 << ": ";
        //     for (int i = 0; i < q; i++) {
        //         if (answers[0][i] == answers[1][i]) {
        //             if (same > numSame / 2) {
        //                 cout << (answers[0][i] == 'T' ? 'T' : 'F');
        //             } else {
        //                 cout << (answers[0][i] == 'T' ? 'F' : 'T');
        //             }
        //         } else {
        //             if (aDiff > bDiff) {
        //                 cout << (answers[0][i] == 'T' ? 'T' : 'F');
        //             } else {
        //                 cout << (answers[1][i] == 'T' ? 'T' : 'F');
        //             }
        //         }
        //     }
        //     cout << " " << (max(aDiff, bDiff) + max(same, numSame - same))
        //          << "/1" << endl;
        //     continue;
        // }

        // copy
        for (int j = n; j < 3; ++j) { // per student
            answers[j] = answers[0];
            score[j] = score[0];
        }

        // n==2 or 3:

        // determine type of question based on answers of students
        for (int j = 0; j < q; ++j) { // per question
            int count = 0;
            for (int k = 0; k < 3; ++k) { // per student
                count += (answers[k][j] == 'T') ? 1 : 0;
            }
            if (count == 3 || count == 0) {
                // all student's answers are the same for the question
                q_type[j] = 3; // set question type
            } else {
                // 1 student answers differently than other 2 students
                bool found = false;
                if (count == 1) {
                    for (int k = 0; k < 3; ++k) {
                        if (answers[k][j] == 'T') {
                            // student k answered question as T
                            q_type[j] = k; // set question type
                            found = true;
                            break;
                        }
                    }
                } else {
                    assert(count == 2);
                    // 2 students answered question as T
                    for (int k = 0; k < 3; ++k) {
                        if (answers[k][j] == 'F') {
                            q_type[j] = k; // set question type
                            found = true;
                            break;
                        }
                    }
                }
                assert(found);
            }
        }
        for (auto j : q_type) {
            freq[j]++;
        }

        // a:=number of answers set to True for question type 0
        // b:=number of answers set to True for question type 1
        // c:=number of answers set to True for question type 2
        // d:=number of answers set to True for question type 3
        // constraints: a \in [0, freq[0]],
        //              b \in [0, freq[1]],
        //              c \in [0, freq[2]],
        //              d \in [0, freq[3]]
        //...

        __int128 total_comb = 0;
        // weight adjusted score for each question type
        vector<__int128> calculated(4, __int128(0));

        for (int a = 0; a <= freq[0]; ++a) {
            for (int b = 0; b <= freq[1]; ++b) {
                for (int c = 0; c <= freq[2]; ++c) {
                    int score_a = a + freq[1] - b + freq[2] - c;
                    int score_b = b + freq[0] - a + freq[2] - c;
                    int score_c = c + freq[0] - a + freq[1] - b;

                    int d = score[0] - score_a;

                    if (d < 0 || d > freq[3]) {
                        continue;
                    }
                    if (score[1] - score_b != d) {
                        continue;
                    }
                    if (score[2] - score_c != d) {
                        continue;
                    }

                    __int128 comb =
                        n_choose_x(freq[0], a) * n_choose_x(freq[1], b) *
                        n_choose_x(freq[2], c) * n_choose_x(freq[3], d);

                    calculated[0] += comb * __int128(a); // comb as weighing
                    calculated[1] += comb * __int128(b);
                    calculated[2] += comb * __int128(c);
                    calculated[3] += comb * __int128(d);
                    total_comb += comb;
                }
            }
        }

        __int128 sum_of_expected_vals = 0;

        vector<bool> flip(4, false);
        for (int i = 0; i < 4; ++i) {
            if (total_comb * __int128(freq[i]) - calculated[i] >
                calculated[i]) {
                flip[i] = true;
                sum_of_expected_vals +=
                    total_comb * __int128(freq[i]) - calculated[i];
            } else {
                flip[i] = false;
                sum_of_expected_vals += calculated[i];
            }
        }

        string ret;
        for (int i = 0; i < q; ++i) { // per question
            int student_index = min(q_type[i], 2);
            char ans = answers[student_index][i];
            if (flip[q_type[i]]) {
                assert(ans == 'T' || ans == 'F');
                char c = (ans == 'T') ? 'F' : 'T';
                ret.push_back(c);
            } else {
                ret.push_back(ans);
            }
        }

        // reduce probability fraction
        auto f_gcd = [](auto a, auto b) -> decltype(a) {
            if (a > b) {
                swap(a, b);
            }
            while (a > 0) {
                b = b % a;
                swap(a, b);
            }
            return b;
        };

        __int128 common = f_gcd(sum_of_expected_vals, total_comb);

        cout << "Case #" << _ + 1 << ": " << ret << " "
             << stringify(sum_of_expected_vals / common) << "/"
             << stringify(total_comb / common) << endl;
    }
    return 0;
}
