#include <iostream>
#include "ladder.h"
#include <set>
#include <vector>
#include <string>
using namespace std;

int main() {
    // Load the dictionary from src/words.txt.
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    
    string start, end;
    cout << "Enter start word: ";
    cin >> start;
    cout << "Enter end word: ";
    cin >> end;
    
    // Check for trivial input.
    if (to_lower(start) == to_lower(end)) {
        cout << "Error: Start and end words must be different." << endl;
        return 1;
    }
    
    // Generate and print the word ladder.
    vector<string> ladder = generate_word_ladder(start, end, word_list);
    if (ladder.empty()) {
        cout << "No word ladder found from " << start << " to " << end << "." << endl;
    } else {
        cout << "Word ladder from " << start << " to " << end << ":\n";
        print_word_ladder(ladder);
    }
    
    return 0;
}
