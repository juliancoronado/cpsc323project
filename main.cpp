#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include "syntaxanalyzer.cpp"
#include "symboltable.cpp"

using namespace std;

// struct for intruction table
struct inst {
    int address;
    string op;
    int oprnd;
};

// global constants
const vector<string> STATE_NAMES = { "keyword", "identifier", "separator", "operator", "real", "integer", "Error state" };
const vector<string> KEYWORD_LIST = { "int", "float", "boolean", "if", "else", "then","get", "put", "while", "for", "return", "fi"};
const vector<string> SEPARATOR_LIST = { "'", "(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "!" };
const vector<string> OPERATOR_LIST = { "*", "+", "-", "==", "/=", ">", "<", "=>", "<=", "="};
syntacticalAnalyzer sa;
symboltable st;
int instr_address = 1;
map<int, inst> instr_table;
string temp;
string global_token;
string global_tstate;
string global_lexeme;
int global_index;

// function protype
void A();
void E();
void T();
void E_prime();
void T_prime();
void F();
void gen_instr(string, int);
void displayTable();
vector<string> parse(string);
string readFile(string);
bool fsmIdentifier(string);
bool fsmInteger(string);
bool fsmReal(string);
void output(vector<string>);
int lexer(string);

vector<string> parsedList;

int main() {
	// will contain the parsed tokens once file is read
	// vector<string> parsedList;
	string fileName;

	cout << "Enter the file name (input is our sample): ";
	cin >> fileName;
	string inputfile = readFile(fileName);

	// calls parse function on text to separate it into individual items
	parsedList = parse(inputfile);
	
	output(parsedList);
    cout << "Completed program - check output.txt.\n";
	return 0;
}

void output(vector<string> tList) {
	ofstream outfile;
	stringstream ss;
	outfile.open("output.txt");

	ss << "Token \t\t\t\t Lexemes\n";
	ss << "-----------------------------------------\n";

	for (int i = 0; i < tList.size(); i++) {
		string lexeme = tList[i];
		string token = STATE_NAMES[lexer(lexeme)];
        global_token = token;
        global_lexeme = lexeme;
        global_tstate = lexer(lexeme);
        int tstate = lexer(lexeme);
        global_index = i;
        // // // // // // // //
        if (token == "identifier") {
            A();
        }
		ss << "Token: " << token << setw(24) << "Lexeme: " << lexeme << '\n';
		//pass token and lexeme to syntactical analyzer
		ss << sa.parse(token, lexeme);
		if (token == "identifier" && !st.exists(lexeme)) {
			st.add(lexeme);
		}
	}
    // adds tokens, lexemes, and production rules to the output file
	outfile << ss.str();
    // adds instruction table to the output file
    outfile << "\nINSTRUCTION TABLE (without nil oprnds)" << endl;
    map<int, inst>::iterator it;
    for (it = instr_table.begin(); it != instr_table.end(); it++) {
        if (it->second.oprnd == 0) {
            continue;
        } else {
            outfile << "Address: " << it->second.address << endl;
            outfile << "Op: " << it->second.op << endl;
            outfile << "Operand: " << it->second.oprnd << endl;
        }
    }
    // adds symbol table to the output file
    outfile << "\nSYMBOL TABLE" << endl;
    map<string, values> tempMap = st.getTable();
    map<string, values>::iterator itr;
    for (itr = tempMap.begin(); itr != tempMap.end(); itr++) {
        if (itr->first == "identifier") {
            continue;
        } else {
                outfile << "Key: " << itr->first << endl;
                outfile << "Address: " << itr->second.address << endl;
                outfile << "Type: " << itr->second.type << endl;
        }
    }
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
	bool period = false;
	int pcount = 0;
	if(isdigit(value.at(0))){
		for(int i = 0; i < value.length(); i++){
			//check if character is a period and is not the last character in the lexeme
			if((int)value.at(i) == 46 && i!=(value.length()-1)){
				period = true;
				pcount++;
			}
			//else if: check if the character is a valid digit
			else if(!isdigit(value.at(i))){
				return false;
			}
		}
		//check if the lexeme has a period and that period only occurs once
		if(pcount == 1 && period){
			return true;
		}
	}
}

//lexer function
int lexer(string token){
	//initialize to error state
	int token_state = 6;
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void A() {
    if (global_token == "identifier") {
        // same = token;
        temp = global_lexeme;
        // lexer();
        string next_lex = parsedList[global_index+1];
        global_index++;
        global_lexeme = next_lex;
        global_token = STATE_NAMES[lexer(next_lex)];
        if (global_lexeme == "=") {
            // lexer();
            next_lex = parsedList[global_index+1];
            global_index++;
            global_lexeme = next_lex;
            global_token = STATE_NAMES[lexer(next_lex)];
            E();
            // cout << "CHECKPOINT\n";
            gen_instr("POPM", st.getaddr(temp));
         } // else {
    //         cout << "A - ERROR: = expected" << endl;
    // }
  //} else {cout << "A - ERROR: id expected" << endl;
  }
};

void E() {
    T();
    E_prime();
};

void E_prime() {
    if (global_lexeme == "+") {
        // lexer();
        string next_lex = parsedList[global_index+1];
        global_lexeme = next_lex;
        global_index++;
        global_token = STATE_NAMES[lexer(next_lex)];
        T();
        gen_instr("ADD", 0);
        E_prime();
  }
};

void T() {
    F();
    T_prime();
};

void T_prime() {
    if (global_lexeme == "*") {
        // lexer();
        string next_lex = parsedList[global_index+1];
        global_index++;
        global_lexeme = next_lex;
        global_token = STATE_NAMES[lexer(next_lex)];
        F();
        // cout << "T_Prime\n";
        gen_instr("MUL", 0);
        T_prime();
    }
};

void F() {
    if (global_token == "identifier") {
        // lexer();
        gen_instr("PUSHM", st.getaddr(global_token));
        string next_lex = parsedList[global_index+1];
        global_lexeme = next_lex;
        global_index++;
        global_token = STATE_NAMES[lexer(next_lex)];
    } // else {
    //     cout << "F - ERROR: id expected" << endl;
    // }
};

void gen_instr(string op, int oprnd) {
    instr_table[instr_address].address = instr_address;
    instr_table[instr_address].op = op;
    instr_table[instr_address].oprnd = oprnd;
    instr_address++;
};

void displayTable() {
    map<int, inst>::iterator it;
    for (it = instr_table.begin(); it != instr_table.end(); it++) {
        cout << "Address: " << it->second.address << endl;
        cout << "Op: " << it->second.op << endl;
        cout << "Operand: " << it->second.oprnd << endl;
    }
}