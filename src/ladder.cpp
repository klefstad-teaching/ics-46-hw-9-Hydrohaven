#include "ladder.h"
#include <cctype>

using namespace std;

// Helper function: convert a string to lowercase.
string to_lower(const string& s) {
    string result;
    for (char ch : s) {
        result.push_back(tolower(ch));
    }
    return result;
}

// Prints an error message regarding the two words.
void error(const string& word1, const string& word2, const string& msg) {
    cerr << "Error between \"" << word1 << "\" and \"" << word2 << "\": " << msg << endl;
}

// Computes the edit distance between str1 and str2 and returns true if the distance is within d.
bool edit_distance_within(const string& str1, const string& str2, int d) {
    int n = str1.size(), m = str2.size();
    if (abs(n - m) > d) return false;
    
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;
    
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

// Determines if two words are adjacent (i.e. their edit distance is exactly 1).
bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

// Generates a word ladder (shortest transformation sequence) from begin_word to end_word using BFS.
vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // The end word must be in the dictionary.
    if (word_list.find(to_lower(end_word)) == word_list.end()) {
        error(begin_word, end_word, "End word not in dictionary");
        return {};
    }
    queue<vector<string>> ladder_queue;
    vector<string> initial = { to_lower(begin_word) };
    ladder_queue.push(initial);
    
    set<string> visited;
    visited.insert(to_lower(begin_word));
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();
        
        // Explore every candidate word in the dictionary.
        for (const string& word : word_list) {
            if (visited.find(word) == visited.end() && is_adjacent(last_word, word)) {
                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);
                visited.insert(word);
                if (word == to_lower(end_word))
                    return new_ladder;
                ladder_queue.push(new_ladder);
            }
        }
    }
    error(begin_word, end_word, "No ladder found");
    return {};
}

// Loads words from a file into a set (one word per line) and converts them to lowercase.
void load_words(set<string>& word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Cannot open file " << file_name << endl;
        return;
    }
    string word;
    while(getline(in, word)) {
        if(!word.empty())
            word_list.insert(to_lower(word));
    }
    in.close();
}

// Prints the word ladder sequence.
void print_word_ladder(const vector<string>& ladder) {
    if(ladder.empty()){
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

#define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }

// Runs verification tests for the word ladder function.
void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}
