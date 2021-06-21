/*

You need your max_rating, current_rating and last 3 rounds rating changes. 

Enter the numbers in this order:

max_rating
current_rating 
ratings_change1 
ratings_change2 
ratings_change3 
lucky_number 


Sit back and get your rating prediction for as long as you are not satisfied with it.

*/


#include<bits/stdc++.h>
using namespace std;
// #pragma GCC optimize "trapv"


const int SEED_ACCEPTANCE_LT = 10;
const int MULT_ACCEPTANCE_LT = 301;
const int COUNT_PREV_CONTEST_DELTA = 3;
const int MULTIPLIER = 10;
const int UPP_LIM_FOR_MUL = 1000;
const int MAX_RATING_SO_FAR = 3797;
const int MY_FAV_REDUCING_AGENT = -3;

// 12
string title_on_rating[] = { 
    "Undated",
    "Noobie", 
    "Puppy-lil", "Spicy-list", "Eggs-puff",
    "Candyman-Disaster", "Maatsaab", "Entertainment-Mstrbator", 
    "Grandma's Sister", "Indigenous Gangbastad", "Legendary Grandma",
    "Insanity"
};

int ratings_list [] = {
    1, 
    1200, 1400, 1600, 
    1900, 2100, 2300, 
    2400, 2600, 3000, MAX_RATING_SO_FAR+1, 
    10000
};

class Rating {
public:
    int maxRating = 0, currRating = 0;
    int predictedRating = 0;
    int testCases = 1;
    int curr_rank = 0;

    vector<int> prev_rating_deltas;

    Rating (int r1, int r2) {
        maxRating = r1, currRating = r2;
        predictedRating = r2;
        curr_rank = (int)(upper_bound(ratings_list, ratings_list+12, currRating) - ratings_list);
    }

    void Get_PastRatings() {
        prev_rating_deltas.resize(COUNT_PREV_CONTEST_DELTA);
        cout << "Enter the ratings delta of past " << COUNT_PREV_CONTEST_DELTA << " contests:\n";
        bool rainboy_flag = false;
        for (int i = 0; i < COUNT_PREV_CONTEST_DELTA; i++) {
            cin >> prev_rating_deltas[i];
            rainboy_flag |= (abs(prev_rating_deltas[i]) >= 111);
        }
        if (rainboy_flag) cout << "\"rainboy delta is detected\"\n";
        sort(prev_rating_deltas.begin(), prev_rating_deltas.end());
        if (prev_rating_deltas[0] > 0) {
            prev_rating_deltas[0] = - prev_rating_deltas[0];
        }
        if (prev_rating_deltas[COUNT_PREV_CONTEST_DELTA-1] < 0) {
            prev_rating_deltas[COUNT_PREV_CONTEST_DELTA-1] = - prev_rating_deltas[COUNT_PREV_CONTEST_DELTA-1];
        }

    }

    void Set_TestCases() {
        cout << "Enter your lucky number duh\n";
        cin >> testCases;
        testCases = (testCases % 9) + 1;
        testCases *= MULTIPLIER;
    }

    void Prediction_Process() {
        vector<int> predicted_n_ratings(testCases);
        vector<long long> seeds(2);
        for (long long &seed: seeds) {
            seed = rand() % SEED_ACCEPTANCE_LT + 1;
        }
        while (seeds[0] == seeds[1]) {
            seeds[0] = rand() % SEED_ACCEPTANCE_LT + 1;
        }
        for (int tc = 0; tc < testCases; tc++) {

            int thisRating = 
            (int)(
                (seeds[0] * currRating + seeds[1] * maxRating) 
                    / 
                (seeds[0] + seeds[1])
            );

            vector<long long> multipliers_for_prev_rounds(COUNT_PREV_CONTEST_DELTA);
            for (long long &mult: multipliers_for_prev_rounds) {
                mult = rand() % UPP_LIM_FOR_MUL + 1;
            }

            sort(multipliers_for_prev_rounds.begin(), multipliers_for_prev_rounds.end());
            int medianPos = COUNT_PREV_CONTEST_DELTA / 2;
            for (int cc = 0; cc < COUNT_PREV_CONTEST_DELTA; cc++) {
                while (llabs(multipliers_for_prev_rounds[cc] - multipliers_for_prev_rounds[medianPos]) > MULT_ACCEPTANCE_LT) {
                    multipliers_for_prev_rounds[cc] = rand() % UPP_LIM_FOR_MUL + 1;
                }
                sort(multipliers_for_prev_rounds.begin(), multipliers_for_prev_rounds.end());
            }

            long long resultantRatingDelta = 0ll, multSum = 0ll;
            for (int position = 0; position < COUNT_PREV_CONTEST_DELTA; position++) {
                resultantRatingDelta += (multipliers_for_prev_rounds[position] * (long long) prev_rating_deltas[position]);
                multSum += multipliers_for_prev_rounds[position];
            }
            resultantRatingDelta /= multSum;
            predicted_n_ratings[tc] = thisRating + (int)resultantRatingDelta;
        }

        // nearest neighbour like
        sort(predicted_n_ratings.begin(), predicted_n_ratings.end());
        int median1 = testCases/2, median2 = testCases/2+1;
        int group1_mean = predicted_n_ratings[median1], group2_mean = predicted_n_ratings[median2];
        int group1_nos = 1, group2_nos = 1;
        long long sophisticated_sum1 = 0ll, sophisticated_sum2 = 0ll;

        for (int i = 0; i < testCases; i++) {
            if (abs(group1_mean - predicted_n_ratings[i]) < abs(group2_mean - predicted_n_ratings[i])) {
                if (abs(group1_mean - predicted_n_ratings[i]) < MULT_ACCEPTANCE_LT) {
                    group1_mean = (int)((long long)group1_mean + predicted_n_ratings[i]) / 2;
                    sophisticated_sum1 += predicted_n_ratings[i];
                    group1_nos++;
                }
            } else {
                if (abs(group2_mean - predicted_n_ratings[i]) < MULT_ACCEPTANCE_LT) {
                    group2_mean = (int)((long long)group2_mean + predicted_n_ratings[i]) / 2;
                    sophisticated_sum2 += predicted_n_ratings[i];
                    group2_nos++;
                }
            }
        }
        int KNN_like_mean = 
        (int) (
            ((long long)(group1_mean) * group1_nos + (long long) (group2_mean) * group2_nos) 
                    /
             (long long)(group2_nos + group1_nos)
        );
        int sophisticated_like_mean = 
        (int) (((sophisticated_sum1 / group1_nos + sophisticated_sum2 / group2_nos)) / 2
        );

        // cout << ".\n[KNN_like_mean: " << KNN_like_mean << "] & [sophisticated_like_mean: " << sophisticated_like_mean << "]\n";
        if (sophisticated_like_mean > KNN_like_mean) {
            swap(sophisticated_like_mean, KNN_like_mean);
        }
        sort(seeds.begin(), seeds.end());
        int KEN_MULTIPLIER = (rand() % MULT_ACCEPTANCE_LT) + MULTIPLIER;
        int XEN_MULTIPLIER = (rand() % KEN_MULTIPLIER) / 2 + 1;

        predictedRating = 
        (int)(
            ((XEN_MULTIPLIER * seeds[0] * sophisticated_like_mean) + (KEN_MULTIPLIER * seeds[1] * KNN_like_mean))
                /
            (((XEN_MULTIPLIER * seeds[0])) + (KEN_MULTIPLIER * seeds[1]))
        );
    }

    void Analysis() {
        cout 
        << "Your are currently " 
        << title_on_rating[curr_rank] 
        << ".\nA little bit of push and you're gonna make it to next level.\n" 
        << title_on_rating[curr_rank+1]
        << " is not too far from you. WH!GL!\n";
        // cout << curr_rank << ratings_list[curr_rank] << "\n";
    }

    void Challenge() {
        const int maxContestSoFar = 1539;
        string url_to_problem = "https://codeforces.com/problemset/problem/";
        cout << "\nA random challenge for you is to solve the following problem\n";
        cout << url_to_problem << rand() % maxContestSoFar + 1 << "/" << char('A' + (curr_rank + 1) / 2) << "\n";
        cout << "(If this link is invalid, the number you entered was not lucky...)\n";

    }
};

signed main() {
    // random random everywhere.
    srand(unsigned(time(0)));

    #ifdef LUCTIVUD
      // const auto start_time = std::chrono::high_resolution_clock::now();
      freopen("/home/luctivud/CPPractice/Zinput.txt", "r", stdin);
      freopen("/home/luctivud/CPPractice/Zoutput.txt", "w", stdout);
    #endif

    // enter currRating and maxRating
    int maxR, currR;
    cout << "Enter your *max rating* and *current rating* respectively\n";
    cin >> maxR >> currR;

    // validation of Rating
    if (maxR < currR) {
        cout << "Your max rating is less than your current rating.. What have you done, dumbass??\n";
        return 0;
    } else if (maxR > MAX_RATING_SO_FAR) {
        cout << "Unfortunately, No one is interested in your insane shit. :P\n";
    }

    // create rating object
    Rating userRating(maxR, currR);

    // get past ratings
    userRating.Get_PastRatings();

    // predict Ratings:
    userRating.Set_TestCases();
    userRating.Prediction_Process();

    // output ratings;
    cout << "\n.\n..\n...\n";
    cout << "Your *predicted rating* after next few rounds is: " << userRating.predictedRating;// << "\n";
    cout << "\n...\n..\n.\n";

    userRating.Analysis();
    userRating.Challenge();

    return 0;
}