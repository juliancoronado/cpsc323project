#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "fsm.h"
#include "sa.h"
using namespace std;

const string validchars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890'()[]{ },.:;!*+-=/><%     ";

void output(vector<string> tlist) {
    ofstream outfile;
    stringstream ss;

    createstack();

    outfile.open("output.txt");

    for (int i = 0; i < tlist.size(); i++) {
        string lexeme = tlist[i];
        string token = STATE_NAMES[lexer(lexeme)];
        ss << "Token: " << token << "\t\tLexeme: " << lexeme << "\n";
        ss << syntax_a(token, lexeme);
    }

    //printstack();

    cout << ss.str();
    outfile << ss.str();
    outfile.close();
}

string readFile(string filename) {
    string text, temp;
    ifstream infile(filename);

    if (infile.is_open()) {
        // runs while index is not at the end of the file
        while (getline(infile, temp)) {
            // this removes all newline chars
            // puts text into one long line
            temp += " ";
            text += temp;
        }
        infile.close();
    } else {
        cout << "Error: Unable to open file.\n";
    }
    return text;

}

vector<string> parse(string s) {
    vector <string> plist;
    string begin;
    begin.assign(s, 0, 1); 

    while (!s.empty()) {
        // Identifier check
        if (isalpha(s[0])) {
            for (int i = 0; i < s.length(); i++) {
                if (isalpha(s[i+1]) || isdigit(s[i+1]) || s[i+1] == '_') {
                    continue;
                } else {
                    plist.push_back(s.substr(0, i + 1));
                    s.erase(0, i + 1);
                    break;
                }
            }
            begin.assign(s, 0, 1);
        }

        // Number check
        if (isdigit(s[0])) {
            int pcount = 0;
            for (int i = 0; i < s.length(); i++) {
                if (isdigit(s[i + 1])) {
                    continue;
                } else if (s[i+1] == '.') {
                    pcount++;
                    if (pcount > 1) {cout << "Error.\n";}
                } else {
                    plist.push_back(s.substr(0, i+1));
                    s.erase(0, i+1);
                    break;
                }
            }
            begin.assign(s, 0, 1);
        }

        // Whitespace check
        if (isblank(s[0])) {
            s.erase(0, 1);
            begin.assign(s, 0, 1);
        }

        // Separator check
        for (int i = 0; i < SEPARATOR_LIST.size(); i++) {
            if (begin == SEPARATOR_LIST[i]) {
                plist.push_back(s.substr(0, 1));
                s.erase(0, 1);
                begin.assign(s, 0, 1);
            }
        }

        // Operator check
        for (int i = 0; i < OPERATOR_LIST.size(); i++) {
            if (begin == OPERATOR_LIST[i]) {
                plist.push_back(s.substr(0, 1));
                s.erase(0, 1);
                begin.assign(s, 0, 1);
            }
        }

        // Invalid Character check
        if (begin.find_first_not_of(validchars) != string::npos) {
            plist.push_back(s.substr(0, 1));
            s.erase(0, 1);
            begin.assign(s, 0, 1);
        }

    }

    return plist;
}