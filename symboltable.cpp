#include <iostream>
#include <map>
#include <string>

using namespace std;

struct values {
    int address;
    string type;
};

class symboltable {
    public:

        void add(string id) {
            values v;
            v.address = curr_addr;
            v.type = "integer";
            symtable[id] = v;
            curr_addr++;
        }

        bool exists(string id) {
            if (symtable.count(id) > 0) {
                return true;
            } else {
                return false;
            }
        }

        void displaymap() {
            map<string, values>::iterator it;
            for (it = symtable.begin(); it != symtable.end(); it++) {
                cout << "Key: " << it->first << endl;
                cout << "Address: " << it->second.address << endl;
                cout << "Type: " << it->second.type << endl << endl;
            }
        }

    private:
        int curr_addr = 5000;
        map<string, values> symtable;
};