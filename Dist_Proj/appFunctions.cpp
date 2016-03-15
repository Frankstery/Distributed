#include "appFunctions.h"

using namespace std;

namespace appFunctions {

string login() {
	string s = "LOGIN";
	return s;
}

bool register(const string& user, const string& pass) {
	string line; 
	vector<string> tokens;

	ifstream listUsers;
	listUsers.open("listOfUsers.txt");
	while (getline(listUsers,line)) {
		copy(istream_iterator<string>(iss), istream_iterator<string>(),back_inserter(tokens));
	}
	cout << tokens.size() << tokens[0] << tokens[1] << endl;
	return 1;
}




}