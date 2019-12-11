#include <iostream>
#include <map>
#include <string>

using namespace std;

// struct holds the values going into the symbol table
struct values {
    int address;
    string type;
};

class symboltable {
    public:

        // Add values to the symbol table
        void add(string id) {
            values v;
            v.address = curr_addr;
            v.type = "integer";
            symtable[id] = v;
            curr_addr++;
        }

        // Check if a value exists in the table
        bool exists(string id) {
            if (symtable.count(id) > 0) {
                return true;
            } else {
                return false;
            }
        }

        // Returns address given an identifier
        int getaddr(string id) {
            return symtable[id].address;
        }

        // Getter function that retuns the symbol table
        map<string, values> getTable() {
            return symtable;
        }

    private:
        // Default starting address
        int curr_addr = 5000;
        map<string, values> symtable;
};