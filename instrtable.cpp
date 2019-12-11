#include <iostream>
#include <map>
#include <string>

using namespace std;

// struct holds the operation and operand
struct values {
    string op;
    string operand;
};

class instrtable {
    public:

    private:
        // default address index starts at 1
        int curr_addr = 1;
        map<int, values> instr_table;
};