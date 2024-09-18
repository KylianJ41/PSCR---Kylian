#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>

bool is_in_vector(const std::vector<std::pair<std::string, int>> &words, const std::string &word)
{
	for (const auto &w : words)
	{
		if (w.first == word)
			return true;
	}
	return false;
}

bool compare_pairs(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
{
	return a.first < b.first;
}

// func that add a word to the vector or increment the count if already in it
void add_word(std::vector<std::pair<std::string, int>> &words, const std::string &word)
{
	auto it = std::lower_bound(words.begin(), words.end(), std::make_pair(word, 0), compare_pairs);
	if (it != words.end() && it->first == word)
		it->second++;
	else
		words.insert(it, std::pair<std::string, int>(word, 1));
}

bool is_in_sorted_vector(const std::vector<std::pair<std::string, int>> &words, const std::string &word)
{
	// binary search
	auto it = std::lower_bound(words.begin(), words.end(), std::make_pair(word, 0), compare_pairs);

	return (it != words.end() && it->first == word);
}

// func that return a pair of string and int if in vector
std::pair<std::string, int> get_pair(const std::vector<std::pair<std::string, int>> &words, const std::string &word)
{
	auto it = std::lower_bound(words.begin(), words.end(), std::make_pair(word, 0), compare_pairs);
	if (it != words.end() && it->first == word)
		return *it;
	return std::pair<std::string, int>("", 0);
}

// func that print the word and nb of occurences of found, and a message if not found
void print_apperances(const std::vector<std::pair<std::string, int>> &words, const std::string &word)
{
	auto p = get_pair(words, word);
	if (p.first != "")
		std::cout << "Found " << p.first << " " << p.second << " times." << std::endl;
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

	// vecteur dans lequel on metra les mots rencontrés
	vector<std::pair<std::string, int>> words;

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
		// word est maintenant "tout propre"

		// if (is_in_vector(words, word))
		//	continue;

		add_word(words, word);

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

	// On trie notre vecteur de pairs pour pouvoir faire une binary_search
	// plutot que de parse tout le vecteur. Ca nous permet d'avoir une
	// complexité en O(log n) plutot qu'en O(n)
	std::sort(words.begin(), words.end(), compare_pairs);

	print_apperances(words, "war");
	print_apperances(words, "peace");
	print_apperances(words, "toto");

	return 0;
}