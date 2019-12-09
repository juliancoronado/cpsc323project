#include <iostream>
#include <map>
#include <string>

using namespace std;

struct values {
    string op;
    string operand;
};

class instrtable {
    public:

    private:
        int curr_addr = 1;
        map<int, values> instr_table;
};