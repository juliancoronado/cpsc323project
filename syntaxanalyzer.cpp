#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class syntaxanalyzer {
public:
	// contains all the states listed in the syntax rules (assignment 1)
	enum State {Null, Statement, Assign, Expression, Factor, Term, Term_Prime, Expression_Prime, Opt_Declaration_List, Declaration, Declaration_List,
		Scan, IDs, Print, While, Condition, Relop, Empty, If, Return, Statement_List, Compound, Qualifier,
		Body, Parameter, Parameter_List, Opt_Parameter_List, Function, Function_Definitions, Opt_Function_Definitions, Rat19F};
	// tracks current state of parser
	stack <string> cstate;
	// tracks of all tokens that have been passed to syntax analyzer
	stack <string> tokenstack;

	void printcstates() {
		cout << "Length of cstates: " << cstate.size() << endl;
		while (!cstate.empty()) {
			cout << cstate.top() << endl;
			cstate.pop();
		}
		cout << '\n';
	}

	//default constructor set to Statement
	syntaxanalyzer() {
		cstate.push(enumVect[Statement]);
		tokenstack.push("null");
	}

    // begins to parse the procedure and declars which state its in
	string parse(string token, string lexeme){
		string s;

		if (cstate.top() == enumVect[Statement]) {
			s = r15(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == enumVect[Expression]) {
			s = r25(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == enumVect[Assign]) {
		 	s = r17(token, lexeme);
		 	tokenstack.push(token);
		 	return s;
		}
		if (cstate.top() == enumVect[Term_Prime]) {
			s = r26P(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == enumVect[Term]) {
			s = r26(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == enumVect[Expression_Prime]) {
			s = r25P(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == enumVect[Scan]) {
			s = r21(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == enumVect[IDs]){
			s = r13(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		else{
			return "";
		}
	}

    // array with strings for each enum State
	string enumVect[31] = {"Null", "Statement", "Assign", "Expression", "Factor", "Term", "Term_Prime", "Expression_Prime", "Opt_Declaration_List", "Declaration", "Declaration_List",
		"Scan", "IDs", "Print", "While", "Condition", "Relop", "Empty", "If", "Return", "Statement_List", "Compound", "Qualifier",
		"Body", "Parameter", "Parameter_List", "Opt_Parameter_List", "Function", "Function_Definitions", "Opt_Function_Definitions", "Rat19F"};

private:

    // Production Rules are contained as private functions of this class

	// Rule 1 *Julian*
	// Rat19F
	// <Rat19F> -> <Opt Function Definitions> %% <Opt Declaration List> <Statement List> %%
	string r1(string token, string lexeme) {
		cstate.push(enumVect[Rat19F]);
		string s;
		s += "<Rat19F> -> ";
		s += r2(token, lexeme); // opt func defs
		s += " %% ";
		s += r10(token, lexeme); // opt dec list
		s += r14(token, lexeme);
		s += " %% ";
		return s;
	}

	// Rule 2 *Julian*
	// Opt Function Definitions
	// <Opt Function Definitions> -> <Function Definitions> | <Empty>
	string r2(string token, string lexeme) {
		cstate.push(enumVect[Opt_Function_Definitions]);
		string s;
		s += "<Opt Function Definitions> ->";
		if (!lexeme.empty()) {
            // Rule 3
			s += r3(token, lexeme);
		} else {
            // Empty
			s += r29(token, lexeme);
		}
		return s;
	}

	// Rule 3 *Julian*
	// Function Definitions
	// <Function Definitions> -> <Function> | <Function> <Function Definitions>
	string r3(string token, string lexeme) {
		cstate.push(enumVect[Function_Definitions]);
		string s;
		s += "<Function Definitions>";
		s += "-> ";
		s += r4(token, lexeme); // todo (the OR part)
		return s;
	}

	// Rule 4 *Julian*
	// Function
	// <Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
	string r4(string token, string lexeme) {
		cstate.push(enumVect[Function]);
		string s;
		s += "<Function> -> ";
		s += "function <Identifier> ( ";
		s += r5(token, lexeme);
		s += ") " + r10(token, lexeme);
		s += r9(token, lexeme);
		return s;
	}

	// Rule 5 *Julian*
	// Opt Parameter List
	// <Opt Parameter List> -> <Parameter List> | <Empty>
	string r5(string token, string lexeme){
		cstate.push(enumVect[Opt_Parameter_List]);
		string s;
		s += "<Opt Parameter List> ->";
		if (!lexeme.empty()) {
			s += r6(token, lexeme);
		} else {
            // Empty
			s += r29(token, lexeme);
		}
		return s;
	}

	// Rule 6 *Julian*
	// Parameter List
	// <Parameter List> -> <Parameter> | <Parameter>, <Parameter List>
	string r6(string token, string lexeme){
		cstate.push(enumVect[Parameter_List]);
		string s;
		s += "<Parameter List>";
		s += "->";
		s += r7(token, lexeme); // todo (the OR part)
		return s;
	}

	// Rule 7 *Julian*
	// Parameter
	// <Parameter> -> <IDs> <Qualifier>
	string r7(string token, string lexeme) {
		cstate.push(enumVect[Parameter]);
		string s;
		s += "<Parameter> ->";
		s += r13(token, lexeme);
		s += r8(token, lexeme);
		return s;
	}

	// Rule 8
	// Qualifier
	// <Qualifier> -> int | boolean | real
	string r8(string token, string lexeme) {
		cstate.push(enumVect[Qualifier]);
		string s;
		s += "\t<Qualifier> ->";
		if (lexeme == "int") {
			s += "int\n";
		}else if (lexeme == "bool") {
			s += "boolean\n";
		}
		else {
			s += "real\n";
		}
		return s;
	}

	// Rule 9 *Julian*
	// Body
	// <Body> -> { <Statement List> }
	string r9(string token, string lexeme) {
		cstate.push(enumVect[Body]);
		return "<Body> -> { " + r14(token, lexeme) + " }";
	}

	// Rule 10 *Julian*
	// Opt Declaration List
	// <Opt Declaration List> -> <Declaration List> | <Empty>;
	string r10(string token, string lexeme) {
		cstate.push(enumVect[Opt_Declaration_List]);
		string s;

		s += "<Opt Declaration List>";
		s += "->";
		
		if (!lexeme.empty()) {
			s += r11(token, lexeme);
		} else {
			// empty function
			s += r29(token, lexeme);
		}

		return s;
	}

	// Rule 11 *Julian*
	// Declaration List
	// <Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>
	string r11(string token, string lexeme) {
		cstate.push(enumVect[Declaration_List]);
		string s;
		s += "<Declaration List>";
		s += "->";
		if (token == "identifier") {
			s += r12(token, lexeme);
		} else {
			s += "\t"; // todo (the OR part)
		}
		return s;
	}

	// Rule 12 *Julian*
	// Declaration
	// <Declaration> -> <Qualifier> <IDs>
	string r12(string token, string lexeme){
		cstate.push(enumVect[Declaration]);
		string s;
		s += "<Declaration>";
		s += "->";
		if (lexeme == "int" || lexeme == "boolean" || lexeme == "real") {
			s += r8(token, lexeme);
		}
		if (token == "identifier") {
			s += r13(token, lexeme);
		}
		return s;
	}

	// Rule 13
	// IDs
	// IDs-> <Identifier> | <Identifier>, <IDs>
	string r13(string token, string lexeme) {
		string s;
		if (cstate.top() != enumVect[IDs]) {
			cstate.push(enumVect[IDs]);
			s += "\t<IDs>->";
		}
		if (token == "identifier") {
			cstate.pop();
			s += "<Identifier>\n";
		}
		if (token == "separator" && lexeme == ","){
			cstate.pop();
			s += ",\n";
		}
		return s;
	}

	// Rule 14 *Julian*
	// Statement List
	// <Statement List> -> <Statement> | <Statement> <Statement List>
	string r14(string token, string lexeme) {
		cstate.push(enumVect[Statement_List]);
		string s;
		s += "<Statement List>";
		s += "->";
		s += r15(token, lexeme); // todo (the OR part)
		return s;
	}

	// Rule 15 *Julian*
	// Statement
	// <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
	string r15(string token, string lexeme){
		string s;
		s += "\t<Statement>";
		// compound
		if (token == "keyword" && lexeme == "compound") {
			s += "->";
			s += r21(token, lexeme);
		} else if (token == "operator" && lexeme == "=") {
			s += "-> <Assign>\n";
			// assign
			s+= r17(token, lexeme);
		}  else if (token == "keyword" && lexeme == "if") {
			// Rule 18 (if)
			s += r18(token, lexeme);
		} else if (token == "keyword" && lexeme == "return") {
			// Rule 19 (return)
			s += r19(token, lexeme);
		} else if (token == "keyword" && lexeme == "print") {
			// Print
			s += r20(token, lexeme);
		} else if (token == "keyword" && lexeme == "while") {
            // While
			s += r22(token, lexeme);
		} else {
			s += "\n";
		}
		return s;
	}

	// Rule 16 *Julian*
	// Compound
	// <Compound> -> { <Statement List> }
	string r16(string token, string lexeme){
		cstate.push(enumVect[Compound]);
		string s;
		s += "<Compound>";
		s += "->";
		s += "{ ";
		s += r14(token, lexeme);
		s += "}";
		return s;
	}

	// Rule 17
	// Assign
	// <Assign> -> <Identifier> = <Expression>
	string r17(string token, string lexeme){
		cstate.push(enumVect[Assign]);

		if(tokenstack.top() != "operator" && token == "identifier"){
			string s;
			s+="<Assign>";
			s+="\n\t<Assign>-> <Identifier> = <Expression>\n";
			return s; 
		} 
		if(token=="operator" && lexeme == "="){
			return "";
		}if(tokenstack.top() == "operator"){
			return r25(token,lexeme);
		}else if(lexeme == ";"){
			cstate.pop();
			return "";
		}else{
			return "";
		}
		
	}

	// Rule 18 *Julian*
	// If
	// <If> -> if ( <Condition> ) <Statement> fi |
	//	if ( <Condition> ) <Statement> otherwise <Statement> fi
	string r18(string token, string lexeme){
		cstate.push(enumVect[If]);
		string s;
		if (token == "keyword" && lexeme =="if") {
			s += "if (" + r23(token, lexeme) + ") " + r15(token, lexeme) + " fi";
		} else {
			s += "if (" + r23(token, lexeme) + ") " + r15(token, lexeme);
			s += " otherwise " + r15(token, lexeme) + " fi";
		}
		return s;
	}

	// Rule 19 *Julian*
	// Return
	// <Return> -> return; | return <Expression>;
	string r19(string token, string lexeme){
		cstate.push(enumVect[Return]);
		string s;
		s += "<Return>-> ";
		if (lexeme.size() > 7) {
			s += "return" + r25(token, lexeme) + ";";
		} else {
			s+= "return;";
		}
		return s;
	}

	// Rule 20 *Julian*
	// Print
	// <Print> -> put( <Expression> );
	string r20(string token, string lexeme){
		cstate.push(enumVect[Print]);
		string s;
		s += "<Print> -> put(" + r25(token, lexeme) + ");";
		return s;
	}


	// Rule 21
	// <Scan> -> get ( <IDs> );
	string r21(string token, string lexeme) {
		string s;
		if(cstate.top() != enumVect[Scan]) {
			cstate.push(enumVect[Scan]);
			s += "<Scan>\n";
			s += "\t<Scan>->get(<IDs>);\n";
		}
		if(token == "keyword" && lexeme=="get") {
			s +="";
		}
		if(token == "separator" && lexeme == "(") {
			s +="";
		}
		if(token=="identifier" || token == "separator" && lexeme == ",") {
			s += r13(token,lexeme);
		}
		if(token == "separator" && lexeme == ")") {
			s +="";
		}
		if(token == "separator" && lexeme == ";"){
			s +="\n";
			cstate.pop();
		}
		return s;
	}

	// Rule 22 *Julian*
	// While
	// <While> -> while ( <Condition> ) <Statement>
	string r22(string token, string lexeme){
		cstate.push(enumVect[While]);
		string s;
		s += "<While>->(" + r23(token, lexeme) + ")<Statement>";
		return s;
	}


	// Rule 23 *Julian*
	// Condition
	// <Condition> -> <Expression> <Relop> <Expression>
	string r23(string token, string lexeme){
		cstate.push(enumVect[Condition]);
		string s;
		s += "<Condition>->" + r25(token, lexeme) + "<Relop>" + r25(token,lexeme);
		return s;
	}

	// Rule 24 *Julian*
	// Relop
	// <Relop> -> == | /= | > | < | => | <=
	string r24(string token, string lexeme){
		cstate.push(enumVect[Relop]);
		string s;
		if (lexeme == "==") {
			s += "==";
		} else if (lexeme == "/=") {
			s += "/=";
		} else if (lexeme == ">") {
			s += ">";
		} else if (lexeme == "<") {
			s += "<";
		} else if (lexeme == "=>") {
			s += "=>";
		} else if (lexeme == "<=") {
			s += "<=";
		} else {
			//
		}
		return s;
	}

	// Rule 25
	// Expression
	// <Expression> -> <Term> <Expression Prime>
	// <Expression Prime> -> + <Term> <Expression> | - <Term> <Expression> | epsilon
	string r25(string token, string lexeme){
		string s;
		if(cstate.top()!= enumVect[Expression]){
			cstate.push(enumVect[Expression]);
		}if(token!="operator"&&token!="separator"){
			s+= "\t<Expression>-><Term><Expression Prime>\n";
			s+= r26(token, lexeme);
			return s;
		}else{
			cstate.pop();
			s+= r25P(token,lexeme);
			return s;
		}
	}

	// Rule 25.2 (stops left recursion)
	// Expression Prime
	// <Expression Prime> -> + <Term> <Expression> | - <Term> <Expression> | epsilon
	string r25P(string token, string lexeme){
		string s;
		if(cstate.top()!= enumVect[Expression_Prime]){
			cstate.push(enumVect[Expression_Prime]);
			s+= "\t<Expression Prime>->";
		}
		if(lexeme=="+"){
			s+="+<Term><Expression_Prime>\n";
		}if(lexeme=="-"){
			s+="-<Term><Expression_Prime>\n";
		}if(token=="identifier"){
			s+= r26(token, lexeme);
		}if(token=="separator"){
			cstate.pop();
			s+="epsilon\n";
			s+= r17(token,lexeme);
		}
		return s;
	}

	// Rule 26
	// Term
	// <Term>-> <Factor> <Term Prime>
	// <Term Prime> -> * <Factor> <Term> | / <Factor> <Term> | epsilon
	string r26(string token, string lexeme){
		if(cstate.top()!= enumVect[Term]){
			cstate.push(enumVect[Term]);
		}if(token != "operator" && token != "separator"){
			string s;
			s+="\t<Term>-> <Factor> <Term Prime>\n";
			s+= r27(token,lexeme);
			return s;
		}else{
			cstate.pop();
			return r26P(token, lexeme);
		}
	}

	// Rule 26.2 (stops left recursion)
	//Term Prime
	//<Term Prime> -> * <Factor> <Term> | / <Factor> <Term> | epsilon
	string r26P(string token, string lexeme) {
		string s;
		s+= "\t<Term Prime>->";
		if(lexeme!="*"&&lexeme!="/"){
			cstate.pop();
			s+="epsilon\n";
			s+= r25(token, lexeme);
		}else if(lexeme=="*"){
			s+="*<Factor><Term>\n";
		}else if(lexeme=="/"){
			s+="/<Factor><Term>\n";
		}else if(token=="identifier"){
			s+= r26(token, lexeme);
		}
		return s;
	}

	// Rule 27
	// Factor
	// Factor -> -<Primary> | <Primary>
	string r27(string token, string lexeme){
		if(cstate.top()!= enumVect[Factor]){
			cstate.push(enumVect[Factor]);
		}
		if(lexeme=="-"){
			return "";
		}
		string s;
		s+= "\t<Factor>->";
		cstate.pop();
		s+= r28(token, lexeme);
		return s;
	}

	// Rule 28
	// Primary
	// Primary -> <Identifier> | <Integer> | <Identifier> (<IDs>)|(<Expression>)|<Real>|true|false
	string r28(string token, string lexeme){
		if(token=="identifier"){
			return "<Identifier>\n";
		}if(token=="integer"){
			return "<Integer>\n";
		}
	}

	// Rule 29 *Julian*
	// Empty
	// <Empty> -> ε
	string r29(string token, string lexeme){
		cstate.push(enumVect[Empty]);
		return "ε";
	}
	
};
