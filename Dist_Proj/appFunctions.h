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
#include <time.h>


std::string logUser(const std::string& user, const std::string& pass);
std::string regUser(const std::string& user, const std::string& pass);
std::string addFriend(const std::string& user, const std::string& userFriend, const std::string& fileName);
std::string getFriends(const std::string& user);
std::string addPost(const std::string& user, const std::string& msg);
std::vector<std::string> getPosts(const std::string& user);
std::string deleteAccount(const std::string& user);



#endif