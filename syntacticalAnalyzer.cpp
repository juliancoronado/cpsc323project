#include <iostream>
#include <stack>
using namespace std;

class syntacticalAnalyzer{
public:
	enum State {Statement, Assign, Expression, Factor, Term, Term_Prime, Expression_Prime,
		Scan, IDs, Print, While, Condition, Relop, Empty, If, Return };
	//track current state of parser
	stack <State> cstate;
	//track of all tokens that have been passed to syntax analyzer
	stack <string> tokenstack;

	//for debugging
	void showstack(stack <State> s) { 
	    while (!s.empty()) { 
	        cout << '\t' << s.top(); 
	        s.pop(); 
	    } 
	    cout << '\n'; 
	} 

	//default constructor set to Statement
	syntacticalAnalyzer(){
		cstate.push(Statement);
		tokenstack.push("null");
	}

	string parse(string token, string lexeme){
		string s;

		if (cstate.top() == Statement) {
			s = r15(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == Expression) {
			s = r25(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == Assign) {
		 	s = r17(token, lexeme);
		 	tokenstack.push(token);
		 	return s;
		}
		if (cstate.top() == Term_Prime) {
			s = r26P(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == Term) {
			s = r26(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == Expression_Prime) {
			s = r25P(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == Scan) {
			s = r21(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if (cstate.top() == IDs){
			s = r13(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		else{
			return "";
		}
	}

private:
	//rule 1
	string r1(string token, string lexeme){
		return "";
	}

	//rule 2
	string r2(string token, string lexeme){
		return "";
	}

	//rule 3
	string r3(string token, string lexeme){
		return "";
	}

	//rule 4
	string r4(string token, string lexeme){
		return "";
	}

	//rule 5
	string r5(string token, string lexeme){
		return "";
	}

	//rule 6
	string r6(string token, string lexeme){
		return "";
	}

	//rule 7
	string r7(string token, string lexeme){
		return "";
	}

	//rule 8
	//<Qualifier> -> int | boolean | real
	string r8(string token, string lexeme) {
		string s;
		s += "\t<Qualifier> ->";
		if (lexeme == "int") {
			s += "int\n";
		}
		else if (lexeme == "bool") {
			s += "boolean\n";
		}
		else {
		s += "real\n";
		}
		return s;
	}

	//rule 9
	string r9(string token, string lexeme){
		return "";
	}

	//rule 10
	string r10(string token, string lexeme){
		return "";
	}

	//rule 11
	string r11(string token, string lexeme){
		return "";
	}

	//rule 12
	string r12(string token, string lexeme){
		return "";
	}

	//rule 13
	//IDs
	//IDs-> <Identifier> | <Identifier>, <IDs>
	string r13(string token, string lexeme){
		string s;
		if(cstate.top()!=IDs){
			cstate.push(IDs);
			s+= "\t<IDs>->";
		}
		if(token=="identifier"){
			cstate.pop();
			s+="<Identifier>\n";
		}if(token=="separator"&&lexeme==","){
			cstate.pop();
			s+=",\n";
		}
		return s;
	}

	//rule 14
	string r14(string token, string lexeme){
		return "";
	}

	//rule 15
	//Statement
	//<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
	string r15(string token, string lexeme){
		string s;
		s+="\t<Statement>";
		//<Scan>
		if(token=="keyword"&&lexeme=="get"){
			s += "->";
			s += r21(token, lexeme);
		}else if(token=="identifier"){
			s+="->";
			s+=r17(token, lexeme);

		}  else if (token == "keyword" && lexeme == "if") {
			// Rule 18 (if)
			s += r18(token, lexeme);
		} else if (token == "keyword" && lexeme == "return") {
			// Rule 19 (return)
			s += r19(token, lexeme);
		} else if (token == "") {
			// to do
		}
		else{
			s+="\n";
		}
		return s;
	}

	//rule 16
	string r16(string token, string lexeme){
		return "";
	}

	//rule 17
	//Assign
	//<Assign> -> <Identifier> = <Expression>
	string r17(string token, string lexeme){
		if(cstate.top()!=Assign){
			cstate.push(Assign);
		}
		if(tokenstack.top()!="operator" && token=="identifier"){
			string s;
			s+="<Assign>";
			s+="\n\t<Assign>-> <Identifier> = <Expression>\n";
			return s; 
		}
		if(token=="operator"&&lexeme=="="){
			return "";
		}if(tokenstack.top()=="operator"){
			return r25(token,lexeme);
		}else if(lexeme==";"){
			cstate.pop();
			return "";
		}else{
			return "";
		}
		
	}

	//rule 18 *Julian*
	// If
	// <If> -> if ( <Condition> ) <Statement> fi |
	//	if ( <Condition> ) <Statement> otherwise <Statement> fi
	string r18(string token, string lexeme){
		cstate.push(If);
		string s;
		// TODO needs some sort of check here bc idk how to check
		// for which one to add to s
		if (token == "keyword" && lexeme =="if") {
			s += "if (" + r23(token, lexeme) + ") " + r15(token, lexeme) + " fi";
		} else {
			s += "if (" + r23(token, lexeme) + ") " + r15(token, lexeme);
			s += " otherwise " + r15(token, lexeme) + " fi";
		}
		return s;
	}

	//rule 19 *Julian*
	// Return
	// <Return> -> return; | return <Expression>;
	string r19(string token, string lexeme){
		cstate.push(Return);
		string s;
		s += "<Return>-> ";
		// idk if this is right, but it checks if the return string
		// has more chars than just "return;" indicating that an expression
		// follows the return statement, or could we just put <Expression>?
		if (lexeme.size() > 7) {
			s += "return" + r25(token, lexeme) + ";";
		} else {
			s+= "return;";
		}
		return s;
	}

	//rule 20 *Julian*
	// Print
	// <Print> -> put( <Expression> );
	string r20(string token, string lexeme){
		cstate.push(Print);
		string s;
		s += "<Print> -> put(" + r25(token, lexeme) + ");";
		return s;
	}


	//rule 21
	//<Scan> -> get ( <IDs> );
	string r21(string token, string lexeme){
		string s;
		if(cstate.top()!=Scan){
			cstate.push(Scan);
			s+= "<Scan>\n";
			s+= "\t<Scan>->get(<IDs>);\n";
		}
		if(token=="keyword" && lexeme=="get"){
			s+="";
		}
		if(token=="separator"&&lexeme=="("){
			s+="";
		}
		if(token=="identifier"||token=="separator"&&lexeme==","){
			s+= r13(token,lexeme);
		}
		if(token=="separator"&&lexeme==")"){
			s+="";
		}
		if(token=="separator"&&lexeme==";"){
			s+="\n";
			cstate.pop();
		}
		return s;
	}

	//rule 22 *Julian*
	// While
	// <While> -> while ( <Condition> ) <Statement>
	string r22(string token, string lexeme){
		cstate.push(While);
		string s;
		s += "<While>->(" + r23(token, lexeme) + ")<Statement>";
		return s;
	}


	//rule 23 *Julian*
	// Condition
	// <Condition> -> <Expression> <Relop> <Expression>
	string r23(string token, string lexeme){
		cstate.push(Condition);
		string s;
		s += "<Condition>->" + r25(token, lexeme) + "<Relop>" + r25(token,lexeme);
		return s;
	}

	//rule 24 *Julian*
	// Relop
	// <Relop> -> == | /= | > | < | => | <=
	string r24(string token, string lexeme){
		cstate.push(Relop);
		string s;
		// ----- maybe these can all be return statements??? ------
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
			// idk what goes in the else part
		}
		return s;
	}


	//rule 25
	//Expression
	//<Expression> -> <Term> <Expression Prime>
	//<Expression Prime> -> + <Term> <Expression> | - <Term> <Expression> | epsilon
	string r25(string token, string lexeme){
		string s;
		if(cstate.top()!=Expression){
			cstate.push(Expression);
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

	//Expression Prime
	//<Expression Prime> -> + <Term> <Expression> | - <Term> <Expression> | epsilon
	string r25P(string token, string lexeme){
		string s;
		if(cstate.top()!=Expression_Prime){
			cstate.push(Expression_Prime);
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

	//rule 26
	//Term
	//<Term>-> <Factor> <Term Prime>
	//<Term Prime> -> * <Factor> <Term> | / <Factor> <Term> | epsilon
	string r26(string token, string lexeme){
		if(cstate.top()!=Term){
			cstate.push(Term);
		}if(token!="operator"&&token!="separator"){
			string s;
			s+="\t<Term>-> <Factor> <Term Prime>\n";
			s+= r27(token,lexeme);
			return s;
		}else{
			cstate.pop();
			return r26P(token, lexeme);
		}
	}

	//Term Prime
	//<Term Prime> -> * <Factor> <Term> | / <Factor> <Term> | epsilon
	string r26P(string token, string lexeme){

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

	//rule 27
	//Factor
	//Factor -> -<Primary> | <Primary>
	string r27(string token, string lexeme){
		if(cstate.top()!=Factor){
			cstate.push(Factor);
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

	//rule 28
	//Primary
	//Primary -> <Identifier> | <Integer> | <Identifier> (<IDs>)|(<Expression>)|<Real>|true|false
	string r28(string token, string lexeme){
		if(token=="identifier"){
			return "<Identifier>\n";
		}if(token=="integer"){
			return "<Integer>\n";
		}
	}

	//rule 29 *Julian*
	// Empty
	// <Empty> -> ε
	string r29(string token, string lexeme){
		cstate.push(Empty);
		return "ε";
	}
	
};
