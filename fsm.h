#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

const vector<string> STATE_NAMES = {"keyword", "identifier", "separator", "operator", "real", "integer", "error state"};
const vector<string> KEYWORD_LIST = { "int", "real", "boolean", "if", "else", "then","get", "put", "while", "for", "return", "fi"};
const vector<string> SEPARATOR_LIST = { "'", "(", ")", "[", "]", "{", "}", ",", ".", ":", ";", "!" };
const vector<string> OPERATOR_LIST = { "*", "+", "-", "==", "/=", ">", "<", "=>", "<=", "="};

// fsm - identifier
bool fsmId(string val) {
    if (isalpha(val.at(0))) {
        for (int i = 0; i < val.length(); i++) {
            if (!isalpha(val.at(i)) && !isdigit(val.at(i)) && (int)val.at(i) != 95) {
                // char is not letter, digit, or number
                return false;
            }
        }
        return true;
    }
}

// fsm - integer
bool fsmInt(string val) {
    for (int i = 0; i < val.length(); i++) {
        if (!isdigit(val.at(i))) {
            // contains char that is not a digit
            return false;
        }
    }
    return true;
}

// fsm - real
bool fsmReal(string val) {
    bool period = false;
    int pcount = 0;
    if (isdigit(val.at(0))) {
        for (int i = 0; i < val.length(); i++) {
            // check if char is a period && not last char in lexeme
            if ((int)val.at(i) == 46 && i != (val.length() - 1)) {
                period = true;
                pcount++;
            } else if (!isdigit(val.at(i))) {
                return false;
            }
        }

        if (pcount == 1 && period) {
            return true;
        }
    }
}

// lexer function (utilizing fsm states)
int lexer (string t) {
    // initialize to error state
    int tstate = 6;

    if (find(KEYWORD_LIST.begin(), KEYWORD_LIST.end(), t) != KEYWORD_LIST.end()) {
        tstate = 0;
    } else if (fsmId(t)) {
        tstate = 1;
    } else if (find(SEPARATOR_LIST.begin(), SEPARATOR_LIST.end(), t) != SEPARATOR_LIST.end()) {
        tstate = 2;
    } else if (find(OPERATOR_LIST.begin(), OPERATOR_LIST.end(), t) != OPERATOR_LIST.end()) {
		tstate = 3;
    } else if (fsmReal(t)) {
        tstate = 4;
    } else if (fsmInt(t)) {
        tstate = 5;
    }

    return tstate;
}