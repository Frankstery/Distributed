#include "appFunctions.h"

using namespace std;



string login() {
	return "Hello";
}



string regUser(string& user, string& pass) {
	string line; 								
	ifstream listUsers;
	listUsers.open("listOfUsers.txt");
	while (getline(listUsers,line)) {
		vector<string> tokens;
		istringstream userPass(line);
    	copy(istream_iterator<string>(userPass),istream_iterator<string>(),back_inserter(tokens));  //Parse the string based on white space;
    	if (tokens[0] == user) {return "TAKEN";}		
	}
	listUsers.close();
	ofstream listUsersW ("listOfUsers.txt", ios_base::app);  //Open up file to be written, ios_base::app makes it able to append
	listUsersW << user + ' ' + pass + " \n";
	const string fileName = user + "Msgs" ".txt";
	const string fileName2 = user + "Friends" ".txt";
	ofstream userFile(fileName.c_str());    //Make the Msgs file for user;
	ofstream userFile2(fileName2.c_str());	//Make the Friends file for user;
	//listUsersW.close();
	return "REGISTERED";		
}