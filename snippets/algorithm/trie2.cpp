#include <iostream>
#include <string>

class TrieNode {
   public:
    // pointer array for child nodes of each node
    TrieNode* children[256];

    // Used for indicating ending of string
    bool isLeaf;

    TrieNode() {
        // initialize the wordEnd variable with false
        isLeaf = false;
        // initialize every index of childNode array with NULL
        for (int i = 0; i < 256; i++) {
            children[i] = nullptr;
        }
    }

    // Method to insert a key into the Trie
    void insert(TrieNode* root, const std::string& key) {
        // Initialize the curr pointer with the root node
        TrieNode* curr = root;

        // Iterate across the length of the string
        for (char c : key) {
            // Convert char to ASCII index
            int index = (int)c;
            // Check if the node exists for the current character in the Trie
            if (curr->children[index] == nullptr) {
                // If node for current character does not exist then make a new node
                curr->children[index] = new TrieNode();
            }
            curr = curr->children[index];
        };
        // Mark the end of the word
        curr->isLeaf = true;
    }

    // Method to search a key in the Trie
    bool search(TrieNode* root, const std::string& key) {
        if (root == nullptr) {
            return false;
        }

        // Initialize the curr pointer with the root node
        TrieNode* curr = root;

        // Iterate across the length of the string
        for (char c : key) {
            int index = (int)c;
            // Check if the node exists for the current character in the Trie
            if (curr->children[index] == nullptr) {
                return false;
            }

            // Move the curr pointer to the already existing node for the current character
            curr = curr->children[index];
        }

        // Return true if the word exists and is marked as ending
        return curr->isLeaf;
    }
};

int main() {
    //
    TrieNode t;
    t.insert(&t, "/home");
    t.insert(&t, "/api/v1/:id");
    t.insert(&t, "/api/v1/:id/users/:userid");

    std::cout << "Search: " << t.search(&t, "/home") << '\n';
    std::cout << "Search: " << t.search(&t, "home") << '\n';
    std::cout << "Search: " << t.search(&t, "/api/v1/:id") << '\n';
    std::cout << "Search: " << t.search(&t, "/api/v1/:idd") << '\n';
    std::cout << "Search: " << t.search(&t, "/api/v1/123/users/:userid") << '\n';
    return 0;
}