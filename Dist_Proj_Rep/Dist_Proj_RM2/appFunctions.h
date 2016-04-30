#ifndef APPFUNCTION_H
#define APPFUNCTION_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <dirent.h>

#include <stdio.h>       // perror, snprintf
#include <stdlib.h>      // exit
#include <unistd.h>      // close, write
#include <string.h>      // strlen
#include <strings.h>     // bzero
#include <time.h>        // time, ctime
#include <sys/socket.h>  // socket, AF_INET, SOCK_STREAM,
                         // bind, listen, accept
#include <netinet/in.h>  // servaddr, INADDR_ANY, htons
#include <cstring>
#include <thread>
#include <mutex>
#include <unordered_map> 
#include "rm.h"

void processRequest(int connfd, RepManager* pr);
std::string regUser(const std::string& user, const std::string& pass);
std::string logUser(const std::string& user, const std::string& pass);

class Person {
public:
	Person(const std::string& userName);
	std::string getName();
	std::string addFriend(const std::string& user, const std::string& userFriend, const std::string& fileName);
	std::string getFriends(const std::string& user);
	std::string addPost(const std::string& user, const std::string& msg);
	std::vector<std::string> getPosts(const std::string& user);
	std::string deleteAccount(const std::string& user);
	
private:
	std::string name;
	std::mutex fileFriends;
	std::mutex filePosts;
};

void addToMapping(const std::string& user, std::unordered_map<std::string, Person*>& mapping);
void initialize();

#endif