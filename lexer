#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*struct record
{
	string token;
	string lexeme;
};


string lexer()
{





}*/


int main()
{
	//Read file character by character
	char c;
	ifstream infile("sample1.txt");
	if (infile.is_open())
	{
		char c;
		while (infile.get(c))
		{
			cout << c;
		}
		cout << endl;
		infile.close();
	}

	//Display error message if file cannot be accessed
	else cout << "Error: Unable to open file" << endl;

	//Now write results to file
	ofstream outfile;
	outfile.open("output1.txt");
	outfile << "Output:\n Token		Lexeme" << endl;
	//Below line is going to replaced by loop outputting
	//tokens and lexemes stored in struct1
	outfile << "keyword				while" << endl;
	outfile.close();


	system("pause");
	return 0;
}
