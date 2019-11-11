#include <iostream>
#include <stack>
using namespace std;

class syntacticalAnalyzer{
public:
	enum State {Statement, Assign, Expression, Factor, Term_Prime};
	stack <State> cstate;
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
	}

	string parse(string token, string lexeme){
		string s;
		showstack(cstate);
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
		}else{
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
	string r8(string token, string lexeme){
		return "";
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
		if(tokenstack.size()==0 && token=="identifier"){
			tokenstack.push(token);
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
		cstate.push(Expression);
		string s;
		s+= "\t<Expression>-><Term><Expression Prime>\n";
		s+= r26(token, lexeme);
		return s;
	}

	//Expression Prime
	//<Expression Prime> -> + <Term> <Expression> | - <Term> <Expression> | epsilon
	string r25P(string token, string lexeme){
		return "";
	}

	//rule 26
	//Term
	//<Term>-> <Factor> <Term Prime>
	//<Term Prime> -> * <Factor> <Term> | / <Factor> <Term> | epsilon
	string r26(string token, string lexeme){
		string s;
		s+="\t<Term>-> <Factor> <Term Prime>\n";
		s+= r27(token,lexeme);
		cstate.push(Term_Prime);
		return s;
	}

	//Term Prime
	//<Term Prime> -> * <Factor> <Term> | / <Factor> <Term> | epsilon
	string r26P(string token, string lexeme){
		string s;
		s+= "\t<Term Prime>->";
		if(lexeme!="*"&&lexeme!="/"){
			s+="epsilon\n";
		}
		return s;
	}

	//rule 27
	//Factor
	//Factor -> -<Primary> | <Primary>
	string r27(string token, string lexeme){
		if(lexeme=="-"){
			return "";
		}
		cstate.push(Factor);
		string s;
		s+= "\t<Factor>->";
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
