#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// global constants
const vector<string> STATE_NAMES = { "keyword", "identifier", "separator", "operator", "real", "integer", "Error state" };
const vector<string> KEYWORD_LIST = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or" , "function" };
const vector<string> SEPARATOR_LIST = { "'", "(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "!" };
const vector<string> OPERATOR_LIST = { "*", "+", "-", "=", "/", ">", "<", "%" };

// function protype
vector<string> parse(string);
string readFile(string);
bool fsmIdentifier(string);
bool fsmInteger(string);
bool fsmReal(string);


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

//fsm to check for identifier
bool fsmIdentifier(string value){
	if(isalpha(value.at(0))){
		for(int i=0; i<value.length(); i++){
			if(!isalpha(value.at(i)) && !isdigit(value.at(i)) && (int)value.at(i) != 95){
				//character is not a letter, digit, or number
				return false;
			}
		}
		return true;
	}
}

//fsm to check for integer
bool fsmInteger(string value){
	for(int i=0; i<value.length(); i++){
		if(!isdigit(value.at(i))){
			//contains a character that is not a digit
			return false;
		}
	}
	return true;
}

//fsm to check for real
bool fsmReal(string value){
	bool period=false;
	int pcount=0;
	if(isdigit(value.at(0))){
		for(int i=0; i<value.length(); i++){
			//check if character is a period and is not the last character in the lexeme
			if((int)value.at(i) == 46 && i!=(value.length()-1)){
				period=true;
				pcount++;
			}
			//else if: check if the character is a valid digit
			else if(!isdigit(value.at(i))){
				return false;
			}
		}
		//check if the lexeme has a period and that period only occurs once
		if(pcount=1 && period){
			return true;
		}
	}
}

//lexer function
int lexer(string token){
	//initialize to error state
	int token_state=6;
	if (std::find(KEYWORD_LIST.begin(), KEYWORD_LIST.end(), token) != KEYWORD_LIST.end()) {
		currentState = 0;
	}else if (isValidID(token)) {
		currentState = 1;
	}else if (std::find(SEPARATOR_LIST.begin(), SEPARATOR_LIST.end(), token) != SEPARATOR_LIST.end()) {
		currentState = 2;
	}else if (std::find(OPERATOR_LIST.begin(), OPERATOR_LIST.end(), token) != OPERATOR_LIST.end()) {
		currentState = 3;
	}else if (isValidReal(token)) {
		currentState = 4;
	}else if (isValidInteger(token)){
		currentState = 5;
	}
	return currentState;
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
