#include <string>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

vector<string> states = {"Rat19F", "Opt Function Definitions", "Function Definitions", "Function",
    "Opt Parameter List", "Parameter List", "Parameter", "Qualifier", "Body", "Opt Declaration List",
    "Declaration List", "Declaration", "IDs", "Statement List", "Statement", "Compound", "Assign",
    "If", "Return", "Print", "Scan", "While", "Condition", "Relop", "Expression", "Term", "Factor",
    "Primary", "Empty"};

// current state (stack)
stack<string> cs;

void createstack() {
    cs.push("Null");
}

void printstack() {
    while (!cs.empty()) {
        cout << cs.top() << " ";
        cs.pop();
    }
    cout << endl;
}

string r27(string token, string lexeme) {
    string s;
    s = "<Factor> -> <Identifier>\n";
    return s;

}

string r26P(string token, string lexeme) {
    cs.push("Term Prime");
    return "<Term Prime> -> epsilon\n";
}

string r26(string token, string lexeme) {
    cs.push("Term");
    string s;
    s = "<Term> -> <Factor> <Term Prime>\n";
    cs.push("Factor");
    return s;
}

string r25P(string token, string lexeme) {
    //cs.push("Expression Prime");
    string s;

    if (cs.top() == "Term Prime") {
        s = "<Expression Prime> -> ";
        s += "+ <Term> <Expression Prime>\n";
        cs.push("Expression Prime");
    } else {
        s = "<Expression Prime> -> epsilon\n";
    }
    return s;
}

string r25 (string token, string lexeme) {
    string s;
    s += "<Expression> -> <Term>";
    cs.push("Term");
    s += " ";
    s += "<Expression Prime>\n";
    return s;
}

string r17 (string token, string lexeme) {
    cs.push("Assign");
    string s;
    s += "<Assign>\n";
    if (token == "identifier") {
        s += "<Assign> -> <Identifier> = <Expression>;\n";
        cs.push("Expression");
        return s;
    }
}

string r15(string token, string lexeme) {
		cs.push("Statement");
		string s;
		s += "<Statement>";
		
        if (token == "identifier") {
            s += " -> ";
            s += r17(token, lexeme);
        }
		
		return s;
}

string r13(string token, string lexeme) {
    //cs.push("Identifier");
    return "<IDs> -> <Identifier>\n";
}

string r8(string token, string lexeme) {
    cs.push("Qualifier");
    string s;
    return "<Qualifier> -> " + lexeme + "\n";
}

string syntax_a(string token, string lexeme) {
    string s;

    if (token == "keyword") {
        if (lexeme == "int" || lexeme == "real" || lexeme == "boolean") {
            s = r8(token, lexeme);
        }
        return s;
    }

    if (lexeme == "," && cs.top() == "Identifier") {
        return "";
    }

    if (cs.top() == "Identifier" && token == "identifier") {
        s = r13(token, lexeme);
        return s;
    }

    if (cs.top() == "Qualifier") {
        s = r13(token, lexeme);
        cs.push("Identifier");
        return s;
    }

    if (token == "identifier") {
        // Statement
        s = r15(token, lexeme);
        return s;
    }

    if (cs.top() == "Expression" && token == "identifier") {
        // Expression
        s = r25(token, lexeme);
        // Term
        s += r26(token, lexeme);
        // Factor
        s += r27(token, lexeme);
        return s;
    }

    if (token == "operator" && lexeme == "=") {
        s = "";
    }

    if (token == "operator" && lexeme == "+") {
        s = r26P(token, lexeme);
        cs.push("Term Prime");
        s += r25P(token, lexeme);
        return s;
    }

    if (token == "separator" && lexeme == ";") {
        s = r26P(token, lexeme);
        s += r27(token, lexeme);
    }

    if (token == "identifier" && cs.top() == "Expression Prime") {
        s = r26(token, lexeme);
        s += r27(token, lexeme);
        return s;
    }


    return s;
}