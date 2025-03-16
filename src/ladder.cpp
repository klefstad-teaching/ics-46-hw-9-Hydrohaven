#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// Prints an error message regarding the two words.
void error(string word1, string word2, string msg) {
    cerr << "Error between \"" << word1 << "\" and \"" << word2 << "\": " << msg << endl;
}

// Computes the edit distance between str1 and str2 and returns true if the distance is within d.
bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int n = str1.size(), m = str2.size();
    // Quick check: if the length difference is greater than d, they can't be within distance d.
    if (abs(n - m) > d) return false;
    
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    
    // Initialize DP table.
    for (int i = 0; i <= n; i++)
        dp[i][0] = i;
    for (int j = 0; j <= m; j++)
        dp[0][j] = j;
    
    // Compute edit distance.
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
        }
    }
    return dp[n][m] <= d;
}

// Determines if two words are adjacent (i.e. differ by exactly one letter).
bool is_adjacent(const string& word1, const string& word2) {
    if (word1.length() != word2.length())
        return false;
    
    int diffCount = 0;
    for (size_t i = 0; i < word1.length(); i++) {
        if (word1[i] != word2[i]) {
            diffCount++;
            if (diffCount > 1)
                return false;
        }
    }
    return diffCount == 1;
}

// Generates a word ladder (shortest transformation sequence) from begin_word to end_word.
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "End word not in word list");
        return {};
    }

    queue<string> q;
    map<string, string> predecessor;
    set<string> visited;

    q.push(begin_word);
    visited.insert(begin_word);

    bool found = false;
    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == end_word) {
            found = true;
            break;
        }

        // Try every candidate from the word list.
        for (const auto& candidate : word_list) {
            if (visited.find(candidate) == visited.end() && is_adjacent(current, candidate)) {
                visited.insert(candidate);
                predecessor[candidate] = current;
                q.push(candidate);
            }
        }
    }

    vector<string> ladder;
    if (!found) {
        error(begin_word, end_word, "No ladder found");
        return ladder;
    }
    
    // Reconstruct the ladder by backtracking from end_word to begin_word.
    string word = end_word;
    while (word != begin_word) {
        ladder.push_back(word);
        word = predecessor[word];
    }
    ladder.push_back(begin_word);
    reverse(ladder.begin(), ladder.end());
    return ladder;
}

// Loads words from a file into a set. Assumes one word per line.
void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Cannot open file " << file_name << endl;
        return;
    }
    
    string word;
    while (in >> word) {
        word_list.insert(word);
    }
    in.close();
}

// Prints the word ladder sequence.
void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i != ladder.size() - 1)
            cout << " -> ";
    }
    cout << endl;
}

// Runs a simple verification test for the word ladder functions.
void verify_word_ladder() {
    // Example word list for testing.
    set<string> word_list = {"cat", "bat", "bet", "bed", "at", "ad", "ed"};
    string begin_word = "cat";
    string end_word = "bed";
    
    cout << "Verifying word ladder from \"" << begin_word << "\" to \"" << end_word << "\":" << endl;
    vector<string> ladder = generate_word_ladder(begin_word, end_word, word_list);
    print_word_ladder(ladder);
}
