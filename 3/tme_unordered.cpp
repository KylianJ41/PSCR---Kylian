#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <forward_list>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

int main()
{
    using namespace std;
    using namespace std::chrono;

    ifstream input("WarAndPeace.txt");

    auto start = steady_clock::now();

    unordered_map<string, int> words;

    size_t nombre_lu = 0;
    string word;
    regex re(R"([^a-zA-Z])");

    while (input >> word)
    {
        word = regex_replace(word, re, "");
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        auto it = words.find(word);
        if (it != words.end())
            it->second++;
        else
            words.insert({word, 1});

        if (nombre_lu % 100 == 0)
            cout << nombre_lu << ": " << word << endl;
        nombre_lu++;
    }
    input.close();

    cout << "Finished Parsing War and Peace" << endl;

    auto end = steady_clock::now();
    cout << "Parsing took "
         << duration_cast<milliseconds>(end - start).count()
         << "ms. \n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

    size_t unique_count = words.size();
    cout << "unique count via size(): " << unique_count << endl;

    string target_word = "war";
    size_t war_count = (words.find(target_word) != words.end()) ? 1 : 0;
    cout << "Found " << war_count << "instances of " << target_word << endl;

    vector<pair<string, int>> entries;
    entries.reserve(words.size());

    for (const auto &entry : words)
        entries.emplace_back(entry.first, entry.second);

    sort(entries.begin(), entries.end(),
         [](const pair<string, int> &a, const pair<string, int> &b) -> bool
         {
             return a.second > b.second;
         });

    cout << "Top 10 words:" << endl;
    size_t top_n = 10;
    for (size_t i = 0; i < top_n; ++i)
        cout << entries[i].first << " : " << entries[i].second << endl;

    unordered_map<int, forward_list<string>> freq_map;

    for (const auto &entry : words)
        freq_map[entry.second].push_front(entry.first);

    vector<int> N_val = {10, 20};

    for (const auto &N : N_val)
    {
        cout << "\nWords with " << N << " occurences:" << endl;
        auto it = freq_map.find(N);
        if (it == freq_map.end())
        {
            cout << "No words found with " << N << " occurences." << endl;
        }
        else
        {
            for (const auto &w : it->second)
                cout << w << " ";
            cout << endl;
        }
    }

    // Q9
    /*
        On utilise une unorderep_map pour créer des associations entre
        la val (clé) et une collection d'objets (Personne).
        On peut ensuite parcourur le vector de Personnes et inserer chaque
        personne dans un groupe duquel il fait partie.

        Ces unordered_map seront alors similaires au résultat que renvoie
        une requête GROUP BY de SQL.
    */
    return 0;
}
