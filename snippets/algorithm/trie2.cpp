// C++ Program to implement trie
#include <iostream>
#include <string>

using namespace std;

const int ALPHABET_SIZE = 26;

// class for a node of the trie
class TrieNode {
   public:
    bool endofWord;
    TrieNode* children[ALPHABET_SIZE];

    // Constructore to initialize a trie node
    TrieNode() {
        endofWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

// class for the Trie implementation
class Trie {
   private:
    TrieNode* root;

   public:
    Trie() { root = new TrieNode(); }

    // Function to insert a word into the trie
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (!node->children[index]) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->endofWord = true;
    }

    // Function to search a word in the trie
    bool search(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (!node->children[index]) {
                return false;
            }
            node = node->children[index];
        }
        return node->endofWord;
    }

    // Function to check if there is any word in the trie
    // that starts with the given prefix
    bool startsWith(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int index = c - 'a';
            if (!node->children[index]) {
                return false;
            }
            node = node->children[index];
        }
        return true;
    }

    // Function to delete a word from the trie
    void deleteWord(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int index = c - 'a';
            if (!node->children[index]) {
                return;
            }
            node = node->children[index];
        }
        if (node->endofWord == true) {
            node->endofWord = false;
        }
    }

    // Function to print the trie
    void print(TrieNode* node, string prefix) const {
        if (node->endofWord) {
            cout << prefix << endl;
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                print(node->children[i], prefix + char('a' + i));
            }
        }
    }

    // Function to start printing from the root
    void print() const { print(root, ""); }
};

int main() {
    // Create a Trie
    Trie trie;

    // Insert words into the trie
    trie.insert("geek/geek");
    // trie.insert("geeks");
    // trie.insert("code");
    // trie.insert("coder");
    // trie.insert("coding");

    // Print the trie
    cout << "Trie contents:" << endl;
    trie.print();

    // Search for words in the trie
    cout << "\nSearch results:" << endl;
    auto node = trie.search("geek/geek");
    cout << "geek/geek: " << node << '\n';

    // cout << "geeks: " << trie.search("geeks") << endl;
    // cout << "code: " << trie.search("code") << endl;
    // cout << "coder: " << trie.search("coder") << endl;
    // cout << "coding: " << trie.search("coding") << endl;
    // cout << "codex: " << trie.search("codex") << endl;

    // // Check if prefixes exist in the trie
    // cout << "\nPrefix results:" << endl;
    // cout << "ge: " << trie.startsWith("ge") << endl;
    // cout << "cod: " << trie.startsWith("cod") << endl;
    // cout << "coz: " << trie.startsWith("coz") << endl;

    // // Delete words from the trie
    // trie.deleteWord("coding");
    // trie.deleteWord("geek");

    // // Print the trie after deletions
    // cout << "\nTrie contents after deletions:" << endl;
    // trie.print();

    // // Search for words in the trie after deletions
    // cout << "\nSearch results after deletions:" << endl;
    // cout << "coding: " << trie.search("coding") << endl;
    // cout << "geek: " << trie.search("geek") << endl;

    return 0;
}