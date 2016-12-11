#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>
#include <ctime>

#define START "\\\\start state\\\\"
#define END   "\r\n"

typedef std::map < std::string, std::map< std::string, int > > state_map;

std::vector <std::string> split(std::string s, char delimit);
void mapping_word_from_input(std::string word, std::string next_word, state_map &state);
std::string find_next_word(state_map &state, std::string current_word);
void sentence_generator(state_map &state, std::string word);

int main()
{
	std::string filename;
	std::fstream fin;
	std::string strInput;
	std::vector <std::string> vector_word;

	// get filename
	std::cout << "Open a file: ";
	std::cin >> filename;
	fin.open(filename, std::fstream::in);
	if (!fin.is_open())
	{
		std::cout << "File open failed.";
		return -1;
	}
	
	state_map state;
	while(getline(fin, strInput))
	{
		vector_word = split(strInput, ' ');
		
		// check if empty or sentence too short (4 is for US, Iraqi constitution purpose)
		if (vector_word.size() <= 4)
			continue;

		// add line front and end
		vector_word.insert(vector_word.begin(), START);
		vector_word.push_back(END);

		// map word and next word with probability
		for (auto &i : vector_word)
		{
			if (i == END)
				continue;

			mapping_word_from_input(i, *(&i + 1), state);
		}
	}

	// sentence generate
	sentence_generator(state, START);

	fin.close();
	return 0;
}

std::vector <std::string> split(std::string s, char delimit)
{
	std::vector <std::string> vector_word;
	std::stringstream ss(s);
	std::string word;
	while(std::getline(ss, word, delimit))
		if (!word.empty())
			vector_word.push_back(word);

	return vector_word;
}

void mapping_word_from_input(std::string word, std::string next_word, state_map &state)
{
	std::map <std::string, int> second_map_feed;
	auto it_word = state.find(word);
	if (it_word == state.end())
	{
		second_map_feed.insert(std::make_pair(next_word, 1));
		state.insert(std::make_pair(word, second_map_feed));
	}
	else
	{
		auto it_second_map = it_word->second.find(next_word);
		if (it_second_map == it_word->second.end())
			it_word->second.insert(std::make_pair(next_word, 1));
		else
			it_second_map->second ++;
	}
}

std::string find_next_word(state_map &state, std::string current_word)
{
	auto it_word = state.find(current_word);
	// state empty
	if (it_word == state.end())
		return END;

	int total_next_count = 0, rand_count = 0, accumulate = 0;
	// count total child
	for (auto it : it_word->second)
		total_next_count += it.second;
	
	// rand
	rand_count = rand() % total_next_count + 1;
	// find the random chosen one
	for (auto it : it_word->second)
	{
		accumulate += it.second;
		if (accumulate >= rand_count)
		// found
			return it.first;
	}

	// ERROR
	return END;
}

void sentence_generator(state_map &state, std::string word)
{
	if (word != START)
		std::cout << word;

	// random number
	srand(time(NULL));

	while (word != END)
	{
		word = find_next_word(state, word);
		std::cout << word << " ";
	}
}