#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <cctype>
using namespace std;

// Helper function: convert a string to lowercase.
string to_lower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Print an error message with context.
void error(string word1, string word2, string msg) {
    cerr << "Error between \"" << word1 << "\" and \"" << word2 << "\": " << msg << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    // For thresholds > 1, fall back to full DP (unlikely to be used).
    if(d > 1) {
        int n = str1.size(), m = str2.size();
        if (abs(n - m) > d)
            return false;
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i <= n; i++)
            dp[i][0] = i;
        for (int j = 0; j <= m; j++)
            dp[0][j] = j;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (tolower(str1[i - 1]) == tolower(str2[j - 1]))
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = 1 + min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
        return dp[n][m] <= d;
    }
    
    // Now handle the d==1 case optimally.
    int len1 = str1.size();
    int len2 = str2.size();
    if (abs(len1 - len2) > 1)
        return false;
    
    // If the strings are equal (ignoring case), edit distance is 0.
    if (to_lower(str1) == to_lower(str2))
        return true;
    
    // If lengths are equal, check for at most one substitution.
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (tolower(str1[i]) != tolower(str2[i]))
                diffCount++;
            if (diffCount > 1)
                return false;
        }
        return true;
    } else {
        // Otherwise, lengths differ by exactly 1.
        // Let shorter and longer be defined.
        const string &shorter = (len1 < len2) ? str1 : str2;
        const string &longer  = (len1 < len2) ? str2 : str1;
        int i = 0, j = 0;
        bool foundDifference = false;
        while (i < shorter.size() && j < longer.size()) {
            if (tolower(shorter[i]) == tolower(longer[j])) {
                i++;
                j++;
            } else {
                if (foundDifference)
                    return false;
                foundDifference = true;
                j++; // skip one letter in the longer string
            }
        }
        return true;
    }
}

// Determines if two words are adjacent by allowing an edit distance of <= 1.
// (Thus, is_adjacent("apple", "apple") returns true.)
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    string start = to_lower(begin_word);
    string end = to_lower(end_word);
    
    // The end word must be in the dictionary.
    if (word_list.find(end) == word_list.end()) {
        error(begin_word, end_word, "End word not in dictionary");
        return vector<string>();
    }
    
    queue<vector<string>> ladder_queue;
    vector<string> initial = { start };
    ladder_queue.push(initial);
    
    // Track words that have been used in any ladder.
    set<string> visited;
    visited.insert(start);
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        
        // Iterate only over dictionary words whose length difference with last_word is <= 1.
        for (const string& candidate : word_list) {
            if (abs(static_cast<int>(candidate.size()) - static_cast<int>(last_word.size())) > 1)
                continue;
            if (visited.find(candidate) == visited.end() && is_adjacent(last_word, candidate)) {
                visited.insert(candidate);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(candidate);
                if (candidate == end)
                    return new_ladder;
                ladder_queue.push(new_ladder);
            }
        }
    }
    error(begin_word, end_word, "No ladder found");
    return vector<string>();
}

// Loads words from a file (one word per line) into the provided set.
// All words are converted to lowercase.
void load_words(set<string>& word_list, const string& file_name) {
    ifstream infile(file_name);
    if (!infile) {
        cerr << "Cannot open file: " << file_name << endl;
        return;
    }
    string word;
    while (getline(infile, word)) {
        if (!word.empty())
            word_list.insert(to_lower(word));
    }
    infile.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << "\n";
        return;
    }
    cout << "Word ladder found: ";
    for (const auto& word : ladder) {
        cout << word << " ";
    }
    cout << "\n";
}

// Runs a series of tests to verify that generate_word_ladder() works as expected.
void verify_word_ladder() {
    #define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}
