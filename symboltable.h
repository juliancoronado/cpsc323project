#include <iostream>
#include <map>
#include <string>

using namespace std;

struct values {
    int address;
    string type;
};

class st {
    public:

        void add(string id, int addr, string type) {
            values v;
            v.address = addr;
            v.type = type;
            symtable[id] = v;
        }

    private:
        map<string, values> symtable;
};