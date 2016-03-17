#include "appFunctions.h"

using namespace std;

vector<string> splitter(const string& line){
    vector<string> tokens;
    istringstream userPass(line);
    copy(istream_iterator<string>(userPass), istream_iterator<string>(), back_inserter(tokens));
    return tokens;
}

string logUser(const string& user, const string& pass) {
    string line;
    ifstream listUsers;
    listUsers.open("listOfUsers.txt");
    while(getline(listUsers, line)){
        //Split line by white space
        vector<string> tokens;
        istringstream userPass(line);
        copy(istream_iterator<string>(userPass), istream_iterator<string>(), back_inserter(tokens));
        //If username and password matches
        if(tokens[0] == user && tokens[1] == pass){
            return "LOGIN";
        }
    }
    //Wrong password or username
    return "WPU";
}



string regUser(const string& user, const string& pass) {
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

string addFriend(const string& user, const string& userFriend, const string& fileName){
    //If trying to add self, return "SELF"
    if(user == userFriend)
        return "SELF";
    ifstream listUsers;
    listUsers.open("listOfUsers.txt");
    string line;
    while(getline(listUsers, line)){
        vector<string> tokens = splitter(line);
        //If adding is on list of users
        if(tokens[0] == userFriend){
            ifstream addedList;
            addedList.open(fileName.c_str());
            string user1;
            while(addedList >> user1){
                //If adding is on list of user and on list of friends
                if(user1 == userFriend){
                    return "ALREADYADDED";
                }
            }
            //Add to list since on list of user, but not on list of friends
            addedList.close();
            ofstream fList(fileName.c_str(), ios_base::app);
            fList << userFriend + "\n";
            fList.close();
            return "ADD";
        }
    }
    //adding is not on list of Users
    listUsers.close();
    return "NOUSER";
}

string getFriends(const string& user){
    string allNames = "";
    string fileName = user + "Friends.txt";
    string name;
    ifstream listUsers;
    listUsers.open(fileName.c_str());
    while(listUsers >> name){
        allNames = allNames + name + " ";
    }
    return allNames;
}

string addPost(const string& user, const string& msg) {
	string s = user + "Msgs.txt";
	ofstream userMsgs (s.c_str(), ios_base::app);
	userMsgs << msg << " \n" << "\n";
	time_t ticks = time(NULL);
	string str(ctime(&ticks));
	userMsgs << str << " \n";
	userMsgs << char(187) << char(187) << char(187) << " \n";
	userMsgs.close();
	return "SUCCESS";
}

void removeLineFromFile(const string& file, const string& user) {
    cout << "File to remove line: " << file << endl;
	string lineFromFile;
    ifstream listUsers;
    listUsers.open(file.c_str());
    vector<string> linesToAddBack;			//Lines that do not contain the user to be deleted
    while(getline(listUsers, lineFromFile)){ 
    	vector<string> lineBack = splitter(lineFromFile);
 		if (lineBack[0] != user) {			//If the line is not the user to be deleted, put into a vector
 			linesToAddBack.push_back(lineFromFile);
     	}
    }
    listUsers.close();    
    remove(file.c_str());   			//Remove the file
    ofstream userFriends(file.c_str(), ios_base::app); //Create file with same name
    for (int i = 0; i < linesToAddBack.size(); ++i) {  //Add back the vector of people that do not contain the user
    	userFriends << linesToAddBack[i] << " \n";
    }
   	userFriends.close();
}


string deleteAccount(const string& user) {
	removeLineFromFile("listOfUsers.txt", user);
	DIR *dir = NULL;
	struct dirent *ent;
	if ((dir = opendir (".")) != NULL) { 		//Open current directory
	  while ((ent = readdir (dir))) {   		//While there are files left
	    string s = ent->d_name;
	    string userFriends = user + "Friends.txt";
	    string userMsgs = user + "Msgs.txt";
	    if (s == userFriends || s == userMsgs) {
            cout << "File to be removed: " << s << endl;
            remove(s.c_str()); //If it is the users Friends or Msgs files, remove them
        }
	    else if (s.find("Friends.txt") != string::npos) {			  //Delete the user from every other persons' friends list
	    	removeLineFromFile(s,user);
	    }
	  }
	  closedir (dir);
	} else {
	  return "COULD NOT OPEN DIRECTORY";
	}
	return "SUCCESS";
}


