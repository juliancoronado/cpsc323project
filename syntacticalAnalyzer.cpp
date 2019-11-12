#include <iostream>
#include <stack>
using namespace std;

class syntacticalAnalyzer{
public:
	enum State {Statement, Assign, Expression, Factor,Term,Term_Prime, Expression_Prime};
	//track current state of parser
	stack <State> cstate;
	//track of all tokens that have been passed to syntax analyzer
	stack <string> tokenstack;


	//for debugging
	void showstack(stack <State> s) 
	{ 
	    while (!s.empty()) 
	    { 
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
		if(cstate.top()==Statement){
			s = r15(token, lexeme);
			tokenstack.push(token);
			return s;
		}if(cstate.top()==Expression){
			s = r25(token, lexeme);
			tokenstack.push(token);
			return s;
		}
		if(cstate.top()==Assign){
		 	s = r17(token, lexeme);
		 	tokenstack.push(token);
		 	return s;
		}
		if(cstate.top()==Term_Prime){
			s = r26P(token, lexeme);
			tokenstack.push(token);
			return s;
		}if(cstate.top()==Term){
			s = r26(token, lexeme);
			tokenstack.push(token);
			return s;
		}if(cstate.top()==Expression_Prime){
			s = r25P(token, lexeme);
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
	string r13(string token, string lexeme){
		return "";
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
		if(token=="identifier"){
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

	//rule 18
	string r18(string token, string lexeme){
		return "";
	}

	//rule 19
	string r19(string token, string lexeme){
		return "";
	}

	//rule 20
	string r20(string token, string lexeme){
		return "";
	}

	//rule 21
	string r21(string token, string lexeme){
		return "";
	}

	//rule 22
	string r22(string token, string lexeme){
		return "";
	}

	//rule 23
	string r23(string token, string lexeme){
		return "";
	}

	//rule 24
	string r24(string token, string lexeme){
		return "";
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

	//rule 29
	string r29(string token, string lexeme){
		return "";
	}
	
};
