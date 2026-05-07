#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TrieNode {
   public:
    TrieNode* children[256];
    bool isEndOfWord;
    bool isParameter;      // Flag to indicate this node represents a parameter
    string parameterName;  // Name of the parameter

    TrieNode() {
        isEndOfWord = false;
        isParameter = false;
        parameterName = "";
        for (int i = 0; i < 256; i++) {
            children[i] = nullptr;
        }
    }
};

class PathTrie {
   private:
    TrieNode* root;

   public:
    PathTrie() { root = new TrieNode(); }

    // Insert a path into the trie
    void insert(string path) {
        TrieNode* curr = root;

        for (int i = 0; i < path.length(); i++) {
            char c = path[i];
            int index = (int)c;

            if (curr->children[index] == nullptr) {
                curr->children[index] = new TrieNode();
            }
            curr = curr->children[index];
        }
        curr->isEndOfWord = true;
    }

    // Search for a path
    bool search(string path) {
        TrieNode* curr = root;

        for (int i = 0; i < path.length(); i++) {
            char c = path[i];
            int index = (int)c;

            if (curr->children[index] == nullptr) {
                return false;
            }
            curr = curr->children[index];
        }
        return curr->isEndOfWord;
    }

    // Check if a path exists (prefix matching)
    bool startsWith(string prefix) {
        TrieNode* curr = root;

        for (int i = 0; i < prefix.length(); i++) {
            char c = prefix[i];
            int index = (int)c;

            if (curr->children[index] == nullptr) {
                return false;
            }
            curr = curr->children[index];
        }
        return true;
    }

    // Match a path and extract parameters
    bool matchPath(string path, vector<string>& parameters) {
        TrieNode* curr = root;
        parameters.clear();

        for (int i = 0; i < path.length(); i++) {
            char c = path[i];
            int index = (int)c;

            if (curr->children[index] == nullptr) {
                return false;
            }
            curr = curr->children[index];
        }
        return curr->isEndOfWord;
    }
};

// Simple test to demonstrate the functionality
int main() {
    PathTrie trie;

    // Insert paths with parameters
    trie.insert("/home/:id");
    trie.insert("/home/:user/profile");
    trie.insert("/api/v1/users/:user_id/posts/:post_id");
    trie.insert("/api/v1/users");

    // Test searches
    cout << trie.search("/home/123") << endl;                    // 1 (true) - matches pattern
    cout << trie.search("/home/:id") << endl;                    // 1 (true) - matches pattern
    cout << trie.search("/api/v1/users/456/posts/789") << endl;  // 1 (true) - matches pattern

    cout << trie.startsWith("/home/") << endl;         // 1 (true)
    cout << trie.startsWith("/api/v1/users") << endl;  // 1 (true)
    cout << trie.startsWith("/nonexistent") << endl;   // 0 (false)

    return 0;
}