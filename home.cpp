#include <iostream>
#include <string>
#include <vector>
#include "parse.h"

using namespace std;

int main() {
    vector <string> parsed;
    string fname;

    cout << "Enter file name: ";
    cin >> fname;

    string inputfile = readFile(fname);

    parsed = parse(inputfile);

    output(parsed);

    return 0;
}