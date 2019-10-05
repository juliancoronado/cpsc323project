#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// global constants
const vector<string> STATE_NAMES = { "initial state", "keyword", "identifier", "separator", "operator", "real", "Error state" };
const vector<string> KEYWORD_LIST = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or" , "function" };
const vector<string> SEPARATOR_LIST = { "'", "(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "!" };
const vector<string> OPERATOR_LIST = { "*", "+", "-", "=", "/", ">", "<", "%" };

// protypes
vector<string> parse(string);
string readFile(string);

int main() {
	
	// will contain the parsed tokens once file is read
	vector<string> parsedList;

	string fileName = "sample1";
	string inputfile = readFile(fileName);

	// calls parse function on text to separate it into individual items
	parsedList = parse(inputfile);

	// Sample output of what the parsedList contains
	for (vector<string>::const_iterator i = parsedList.begin(); i != parsedList.end(); ++i) {
		cout << *i << '\n';
	}

	return 0;
}

// parse function
vector<string> parse(string s) {

	vector<string> parseList;
	string atBegin;
	atBegin.assign(s, 0 , 1);

	do {
		// LETTER
		if (isalpha(s[0])) {
			for (int i = 0; i < s.length(); i++) {
				// if the next char is a letter, number, or underscore, keep going
				if (isalpha(s[i+1]) || isdigit(s[i+1]) || s[i+1] == '_') {
					continue;
				// else push that token to the vector
				} else {
					parseList.push_back(s.substr(0, i+1));
					s.erase(0, i+1);
					break;
				}
			} 
			atBegin.assign(s, 0, 1);
		}

		// NUMBER
		if (isdigit(s[0])) {
			// keeps track of periods
			int periodCount = 0;
			for (int i = 0; i < s.length(); i++) {
				if (isdigit(s[i+1])) {
					continue;
				} else if (s[i+1] == '.') {
					periodCount++;
					// if there is more than one period, there will be an error
					if (periodCount > 1) {cout << "ERROR" << endl;}
				} else {
					parseList.push_back(s.substr(0, i+1));
					s.erase(0, i+1);
					break;
				}
			}
			atBegin.assign(s, 0, 1);
		}

		// WHITESPACE
		if (isblank(s[0])) {
			s.erase(0, 1);
			atBegin.assign(s, 0, 1);
		}

		// SEPARATOR - compares to separator list
		for (int i = 0; i < SEPARATOR_LIST.size(); i++) {
			if (atBegin == SEPARATOR_LIST[i]) {
				parseList.push_back(s.substr(0, 1));
				s.erase(0, 1);
				atBegin.assign(s, 0, 1);
			}
		}

		// OPERATOR - compares to operator list
		for (int i = 0; i < OPERATOR_LIST.size(); i++) {
			if (atBegin == OPERATOR_LIST[i]) {
				parseList.push_back(s.substr(0, 1));
				s.erase(0, 1);
				atBegin.assign(s, 0 , 1);
			}
		}

	} while (!s.empty());
	// runs while s is NOT empty, will stop once s IS empty

	// returns parseList
	return parseList;
}

// read input from file function
string readFile(string filename) {
	string text, temp;
	ifstream infile(filename);

	if (infile.is_open())
	{
		// runs while index is not at the end of the file
		while (!infile.eof())
		{
			// this will remove all the newline characters and put text into one line
			getline(infile, temp);
			temp += " ";
			text += temp;
		}
		infile.close();
	} else {
		// Displays error if file is unable to open
		cout << "Error: Unable to open file" << endl;
	}

	return text;

}
