#include <bits/stdc++.h>

using namespace std;

double inverse_sigmoid(double y) {
    double temp = log(y / (1.0 - y));
    return min(max(-3., temp), 3.); // bound in [-3,3]
}

double sigmoid(double x) { return 1. / (1. + exp(-x)); }

constexpr int NUM_QUESTION = 10000;
constexpr int NUM_PEOPLE = 100;

int main() {
    int t, p;
    scanf("%d", &t);
    scanf("%d", &p);
    for (int i = 0; i < t; ++i) {
        vector<vector<bool>> arr(
            100,
            vector<bool>(
                NUM_QUESTION)); // player id -> question id -> result (1/0)
        for (int j = 0; j < NUM_PEOPLE; ++j) {
            for (int k = 0; k < NUM_QUESTION; ++k) {
                char c;
                cin >> c;
                if (c == '1') {
                    arr[j][k] = true; // answered correctly
                } else if (c == '0') {
                    arr[j][k] = false; // answered incorrectly
                } else {
                    assert(false && "unexpected input");
                }
            }
        }

        // idea: predict skill per player, estimate hardness per question using
        // either:
        // 1. inverse function of sigmoid, or
        // 2. bounding search using the forward sigmoid function and observed
        // accuracy over some adequate iterations
        //
        // take top N hardest question, and evaluate answer outcome probability
        // using predicted skill and question hardness using given probability
        // function (sigmoid) take observation of those top N hardest quesetion
        // calculate difference of observation - prediction, the player with
        // highest difference is guessed to be the cheater

        vector<double> skills;
        for (int j = 0; j < NUM_PEOPLE; ++j) { // per person
            double correct = 0.;
            for (int k = 0; k < NUM_QUESTION; ++k) { // per question
                correct += arr[j][k];
            }
            double accuracy = correct / NUM_QUESTION;
            double skill = inverse_sigmoid(accuracy);
            skills.push_back(skill);
        }

        vector<double> hardness;
        for (int k = 0; k < NUM_QUESTION; ++k) { // per question
            double correct = 0.;
            for (int j = 0; j < NUM_PEOPLE; ++j) { // per person
                correct += arr[j][k];
            }
            double accuracy =
                correct / NUM_PEOPLE; // proportion of people getting current
                                      // question correctly
            double h = -inverse_sigmoid(
                accuracy); // negative this value since hardness is inverse to
                           // accuracy of people's answers and the domain [-3,3]
                           // is symmetric about 0.0
            hardness.push_back(h);
        }

        vector<int> indices_question(NUM_QUESTION);
        iota(indices_question.begin(), indices_question.end(), 0);

        // sort in non-increasing hardness
        sort(indices_question.begin(), indices_question.end(),
             [&hardness](int a, int b) -> bool {
                 return hardness[a] > hardness[b];
             });

        // take top 10% of the questions in hardness

        vector<double> diffs;
        for (int j = 0; j < NUM_PEOPLE; ++j) { // consider per player
            double player_skill = skills[j];
            double diff_accum = 0.;

            double accum_predicted_accuracy = 0.;
            int num_samples = NUM_QUESTION * 0.1;
            for (int k = 0; k < num_samples; ++k) {
                double question_hardness = hardness[indices_question[k]];
                accum_predicted_accuracy +=
                    sigmoid(player_skill - question_hardness);
            }
            accum_predicted_accuracy /= num_samples;

            double accum_observed_accuracy = 0.;
            for (int k = 0; k < num_samples; ++k) {
                accum_observed_accuracy += arr[j][indices_question[k]];
            }
            accum_observed_accuracy /= num_samples;

            double d = accum_observed_accuracy -
                       accum_predicted_accuracy; // more positive if person does
                                                 // better than prediction

            diffs.push_back(d);
        }

        int max_index = max_element(diffs.begin(), diffs.end()) - diffs.begin();

        printf("Case #%d: %d\n", i + 1, max_index + 1);
    }
    return 0;
}
