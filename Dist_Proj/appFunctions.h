#ifndef APPFUNCTION_H
#define APPFUNCTION_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>



namespace appFunctions {

std::string login();
bool register(const std::string& user, const std::string& pass);

}

#endif