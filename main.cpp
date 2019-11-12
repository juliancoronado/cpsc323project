#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "syntacticalAnalyzer.cpp"

using namespace std;

// global constants
const vector<string> STATE_NAMES = { "keyword", "identifier", "separator", "operator", "real", "integer", "Error state" };
const vector<string> KEYWORD_LIST = { "int", "float", "bool", "if", "else", "then","get", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or" , "function" };
const vector<string> SEPARATOR_LIST = { "'", "(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "!" };
const vector<string> OPERATOR_LIST = { "*", "+", "-", "=", "/", ">", "<", "%" };
syntacticalAnalyzer sa;

// function protype
vector<string> parse(string);
string readFile(string);
bool fsmIdentifier(string);
bool fsmInteger(string);
bool fsmReal(string);
void output(vector<string>);
int lexer(string);

int main() {
	// will contain the parsed tokens once file is read
	vector<string> parsedList;
	string fileName;

	cout << "Enter the file name: ";
	cin >> fileName;

	string inputfile = readFile(fileName);

	// calls parse function on text to separate it into individual items
	parsedList = parse(inputfile);
	
	output(parsedList);

	return 0;
}

void output(vector<string> tList) {
	ofstream outfile;
	stringstream ss;
	outfile.open("output.txt");

	ss << "Token \t\t\t Lexemes\n";
	ss << "-----------------------------------------\n";

	for (int i = 0; i < tList.size(); i++) {
		ss << STATE_NAMES[lexer(tList[i])] << setw(24) << tList[i] << '\n';
		//pass token and lexeme to syntactical analyzer
		ss<<sa.parse(STATE_NAMES[lexer(tList[i])], tList[i]);
	}

	cout << ss.str();
	outfile << ss.str();
	outfile.close();

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
	if (find(KEYWORD_LIST.begin(), KEYWORD_LIST.end(), token) != KEYWORD_LIST.end()) {
		token_state = 0;
	}else if (fsmIdentifier(token)) {
		token_state = 1;
	}else if (find(SEPARATOR_LIST.begin(), SEPARATOR_LIST.end(), token) != SEPARATOR_LIST.end()) {
		token_state = 2;
	}else if (find(OPERATOR_LIST.begin(), OPERATOR_LIST.end(), token) != OPERATOR_LIST.end()) {
		token_state = 3;
	}else if (fsmReal(token)) {
		token_state = 4;
	}else if (fsmInteger(token)){
		token_state = 5;
	}
	return token_state;
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

		//invalid character
		if(atBegin.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890'()[]{ },.:;!*+-=/><%	 ") !=string::npos){
			parseList.push_back(s.substr(0, 1));
			s.erase(0, 1);
			atBegin.assign(s, 0 , 1);
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

	if (infile.is_open()) {
		
		// runs while index is not at the end of the file

		while (getline(infile, temp)) {
			// this will remove all the newline characters and put text into one line

			temp += " ";
			text += temp;
			// cout << count(text.begin(), text.end(), ' ') << endl;
		}

		infile.close();
	} else {
		// Displays error if file is unable to open
		cout << "Error: Unable to open file" << endl;
	}

	return text;

}
