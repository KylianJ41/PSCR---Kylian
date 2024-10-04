#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>
#include "Hash_table.h"

// func that print the val if exist in the hash_table, and if get returns a nullptr will print 0
void print_apperances(pr::Hash_table<std::string, int> &words, const std::string &word)
{
    int *count = words.get(word);
    if (count)
        std::cout << "Found " << word << " " << *count << " times." << std::endl;
    else
        std::cout << "Word '" << word << "' not found." << std::endl;
}

int main()
{
    using namespace std;
    using namespace std::chrono;

    ifstream input = ifstream("WarAndPeace.txt");

    auto start = steady_clock::now();
    cout << "Parsing War and Peace" << endl;

    // faire une hash_table avec mots en clé en int en val
    pr::Hash_table<string, int> words;

    size_t nombre_lu = 0;
    // prochain mot lu
    string word;
    // une regex qui reconnait les caractères anormaux (négation des lettres)
    regex re(R"([^a-zA-Z])");
    while (input >> word)
    {
        // élimine la ponctuation et les caractères spéciaux
        word = regex_replace(word, re, "");
        // passe en lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        int *count = words.get(word);
        if (count)
            (*count)++;
        else
            words.put(word, 1);

        // word est maintenant "tout propre"
        if (nombre_lu % 100 == 0)
            // on affiche un mot "propre" sur 100
            cout << nombre_lu << ": " << word << endl;
        nombre_lu++;
    }
    input.close();

    cout << "Finished Parsing War and Peace" << endl;

    auto end = steady_clock::now();
    cout << "Parsing took "
         << duration_cast<milliseconds>(end - start).count()
         << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    print_apperances(words, "war");
    print_apperances(words, "peace");
    print_apperances(words, "toto");

    size_t unique_count = pr::count(words.begin(), words.end());
    size_t size_from_hash = words.size();

    cout << "Unique count via count: " << unique_count << endl;
    cout << "Unique count via size(): " << size_from_hash << endl;

    if (unique_count == size_from_hash)
    {
        cout << "Counts are consistent." << endl;
    }
    else
    {
        cout << "Counts are NOT consistent!" << endl;
    }

    string target_word = "war";
    size_t war_count = pr::count_if_equal(words.begin(), words.end(), target_word);
    cout << "Found " << war_count << " instances of " << target_word << endl;

    // Q5
    vector<pair<string, int>> entries;
    entries.reserve(words.size());

    for (auto it = words.begin(); it != words.end(); ++it)
        entries.emplace_back(it->key, it->value);

    cout << "Entries copy de Iterator size: " << entries.size() << endl;

    sort(entries.begin(), entries.end(),
         [](const pair<string, int> &a, const pair<string, int> &b) -> bool
         {
             return a.second > b.second;
         });

    cout << "Top 10 words:" << endl;
    size_t top_n = 10;
    for (size_t i = 0; i < top_n; ++i)
        cout << entries[i].first << ": " << entries[i].second << endl;

    return 0;
}
