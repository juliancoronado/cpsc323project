#pragma once
#include <iostream>

using namespace std;

string save;

void A() {
  if (token_state == 1) {
    save = token;
    lexer();
    if (token == "=") {
      lexer();
      E();
      //get_instr(POPM, get_address(save));
    }
    else cout << "ERROR: = expected" << endl;
  }
  else {cout << "ERROR: id expected" << endl;}
};

void E() {
  T();
  E_prime();
};

void E_prime() {
  if (token == "+") {
    lexer();
    T();
    //gen_instr(ADD, nil);
    E_prime();
  }
};

void T() {
  F();
  T_prime();
};

void T_prime() {
  if (token == "*") {
    lexer();
    F();
    //gen_instr(MUL,nil);
    T_prime();
  }
};

void F() {
  if (token_state == 1) {
    //gen_instr(PUSHM, get_address(token));
    lexer();
  } else {
    cout << "ERROR: id expected" << endl;
  }
};

void gen_instr(op, oprnd) {
  instr_table[instr_address].address = instr_address;
  instr_table[instr_address].op = op;
  instr_table[instr_address].oprnd = oprnd;
  instr_address++;
};
