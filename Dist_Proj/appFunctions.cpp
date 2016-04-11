#include "appFunctions.h"

using namespace std;
mutex allUsersMutex;

unordered_map<string, Person*> mapping;
//Function to split the string base on white space
vector<string> splitter(const string& line){
    vector<string> tokens;
    istringstream userPass(line);
    copy(istream_iterator<string>(userPass), istream_iterator<string>(), back_inserter(tokens));
    return tokens;
}


Person::Person(const string& userName): name(userName) {}

string logUser(const string& user, const string& pass) {
    //this_thread::sleep_for(chrono::milliseconds(100));    
    string line;
    ifstream listUsers;
    lock_guard<std::mutex> guard(allUsersMutex);
    listUsers.open("listOfUsers.txt");
    while(getline(listUsers, line)){
        //Split line by white space
        vector<string> tokens = splitter(line);
        //If username and password matches
        if(tokens[0] == user && tokens[1] == pass){
            return "LOGIN";
        }
    }
    //Wrong password or username
    return "WPU";
}



string regUser(const string& user, const string& pass) {
    //this_thread::sleep_for(chrono::milliseconds(100));
	string line; 								
	ifstream listUsers;
    lock_guard<std::mutex> guard(allUsersMutex);
	listUsers.open("listOfUsers.txt");
	while (getline(listUsers,line)) {
		vector<string> tokens;
		istringstream userPass(line);
    	copy(istream_iterator<string>(userPass),istream_iterator<string>(),back_inserter(tokens));  //Parse the string based on white space;
    	if (tokens[0] == user) {return "TAKEN";}		
	}
	listUsers.close();
    addToMapping(user,mapping);
	ofstream listUsersW ("listOfUsers.txt", ios_base::app);  //Open up file to be written, ios_base::app makes it able to append
	listUsersW << user + ' ' + pass + " \n";
	const string fileName = user + "Msgs" ".txt";
	const string fileName2 = user + "Friends" ".txt";
	ofstream userFile(fileName.c_str());    //Make the Msgs file for user;
	ofstream userFile2(fileName2.c_str());	//Make the Friends file for user;
    userFile2 << "Home\n";
	listUsersW.close();
    userFile.close();
    userFile2.close();
	return "REGISTERED";		
}

string Person::addFriend(const string& user, const string& userFriend, const string& fileName){
    //If trying to add self, return "SELF"
    lock_guard<std::mutex> guard(fileFriends);
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

string Person::getFriends(const string& user){
    //this_thread::sleep_for(chrono::milliseconds(100));   
    string allNames = "";
    string fileName = user + "Friends.txt";
    string name;
    ifstream listUsers;
    lock_guard<std::mutex> guard(fileFriends);
    listUsers.open(fileName.c_str());
    while(listUsers >> name){
        allNames = allNames + name + " ";
    }
    return allNames;
}

string Person::addPost(const string& user, const string& msg) {
	string s = user + "Msgs.txt";
    lock_guard<std::mutex> guard(filePosts);
	ofstream userMsgs (s.c_str(), ios_base::app);
	userMsgs << msg << " \n" << "\n";
	time_t ticks = time(NULL);
	string str(ctime(&ticks));
	userMsgs << '-' << str << " \n";
	userMsgs << "$a$b$c$1$2$3\n";
	userMsgs.close();
	return "SUCCESS";
}

vector<string> Person::getPosts(const string& user){
    //this_thread::sleep_for(chrono::milliseconds(100));
    lock_guard<std::mutex> guard(filePosts);
    vector<string> allMessages;
   
    //Creating the separator to show different messages
    string a = "$a$b$c$1$2$3";
   
    string s = user + "Msgs.txt";
    ifstream userMsgs(s.c_str());
    string aMessage = "";
    string line;
    while(getline(userMsgs, line)){
        //If line isn't separator append to messages
        if(line != a){
            aMessage = aMessage + line + "\n";
        }
        //If line is separator push the message to vector and reset it
        else{
            allMessages.push_back(aMessage);
            aMessage = "";
        }
    }
    userMsgs.close();
    return allMessages;
}

void removeLineFromFile(const string& file, const string& user) {
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


string Person::deleteAccount(const string& user) {
    lock_guard<std::mutex> guard(allUsersMutex);
    lock_guard<std::mutex> guard2(fileFriends);
	removeLineFromFile("listOfUsers.txt", user);
    int other;
	DIR *dir = NULL;
	struct dirent *ent;
	if ((dir = opendir (".")) != NULL) { 		//Open current directory
	  while ((ent = readdir (dir))) {   		//While there are files left
	    string s = ent->d_name;
	    string userFriends = user + "Friends.txt";
	    string userMsgs = user + "Msgs.txt";
	    if (s == userFriends || s == userMsgs) {
            remove(s.c_str()); //If it is the users Friends or Msgs files, remove them
        }
	    else if ((other = s.find("Friends.txt")) != string::npos) {			  //Delete the user from every other persons' friends list
            string sub = s.substr(0, other);
            cout << sub << endl;
            cout << mapping.size() << endl;
            unordered_map<string,Person*>::const_iterator got = mapping.find(sub);
            if (got == mapping.end()) {
                cout << "End of map";
                return "No work";
            }
            got->second->fileFriends.lock();
	    	removeLineFromFile(s,user);
            got->second->fileFriends.unlock();
	    }
	  }
	  closedir (dir);
	} else {
	  return "COULD NOT OPEN DIRECTORY";
	}
	return "SUCCESS";
}

string Person::getName() {return name;}

void addToMapping(const std::string& user, unordered_map<std::string, Person*>& mapping) {
    Person* person = new Person(user);
    mapping.insert({user,person});
}

void initialize() {
    ifstream listUsers;
    listUsers.open("listOfUsers.txt");
    string line;
    while(getline(listUsers, line)){
        vector<string> tokens = splitter(line);
        //If adding is on list of users
        //cout << tokens[0] << endl;
        //cout << mapping.size();
        addToMapping(tokens[0],mapping);
        //cout << mapping.size();
        for (auto& x: mapping) {
            cout << x.second->getName() << endl;
        }
    }    
}

void processRequest(int connfd) {
    char    buff[4096];
    memset(buff, 0, sizeof(buff)); //Clear buffer
    recv(connfd, buff, sizeof(buff), 0); //Wait and receive something
    string s(buff);
    cout << s << endl;
    istringstream recvMsg(s);
    vector<string> tokens;
    copy(istream_iterator<string>(recvMsg),istream_iterator<string>(),back_inserter(tokens)); //Parse the message
    if (tokens[0] == "REGISTER") {
        string returnMsg = regUser(tokens[1], tokens[2]);                          //Register user if message is to register 
        //cout << returnMsg.c_str() << endl;
        memset(buff, 0, sizeof(buff));                                             
        strcpy(buff, returnMsg.c_str());                                           //Put string to buffer to be sent
        send(connfd, buff, strlen(returnMsg.c_str()), 0);                          //Send to client
    }
    else if (tokens[0] == "LOGIN") {
        string returnMsg = logUser(tokens[1], tokens[2]);                          //Register user if message is to register 
        //cout << returnMsg.c_str() << endl;
        memset(buff, 0, sizeof(buff));                                             
        strcpy(buff, returnMsg.c_str());                                           //Put string to buffer to be sent
        send(connfd, buff, strlen(returnMsg.c_str()), 0);    
    }
    else if(tokens[0] == "ADDFRIEND"){
    //Add friend to list of friends
        unordered_map<string,Person*>::const_iterator got = mapping.find(tokens[1]);
        string returnMsg = got->second->addFriend(tokens[1], tokens[2], tokens[3]);
        cout << returnMsg << endl;
        memset(buff, 0, sizeof(buff));
        send(connfd, returnMsg.c_str(), strlen(returnMsg.c_str()), 0);
    }
    else if(tokens[0] == "GETFRIENDS"){
    //Get list of friends
        unordered_map<string,Person*>::const_iterator got = mapping.find(tokens[1]);
        string returnMsg = got->second->getFriends(tokens[1]);
        send(connfd, returnMsg.c_str(), strlen(returnMsg.c_str()), 0);
    }
    else if (tokens[0] == "ADDMSG") {
        string username, message;
        username = tokens[1];
        int pos = s.find(' ');
        s.erase(0, pos + 1);
        pos = s.find(' ');
        s.erase(0, pos + 1);
        message = s;
        unordered_map<string,Person*>::const_iterator got = mapping.find(tokens[1]);
        got->second->addPost(username, message);
    }
    else if(tokens[0] == "GETPOSTS"){
    //Make a list of messages
        unordered_map<string,Person*>::const_iterator got = mapping.find(tokens[1]);
        vector<string> allMessages = got->second->getPosts(tokens[1]);
        string amess;
        for(int i = 0; i < allMessages.size(); i++){   //Send one message at a time
            amess = allMessages[i];
            send(connfd, amess.c_str(), strlen(amess.c_str()), 0);
            while(1){          
                memset(buff, 0, sizeof(buff));
                recv(connfd, buff, sizeof(buff), 0);
                string t(buff);
                if(t == "OKMSGS")
                    break;
            }
        }
       
        string last = "End of Messages";            //To indicate the end of all messages
        send(connfd, last.c_str(), strlen(last.c_str()), 0);
 
    }
    else if (tokens[0] == "DELETE") {
        unordered_map<string,Person*>::const_iterator got = mapping.find(tokens[1]);
        string returnMsg = got->second->deleteAccount(tokens[1]);
        send(connfd, returnMsg.c_str(), strlen(returnMsg.c_str()), 0);
    }
    close(connfd);
}

