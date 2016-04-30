#ifndef RM_H
#define RM_H

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
#include <arpa/inet.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <unordered_map> 

class RepManager {
public:
	RepManager(const bool p);
	std::string sendMessage(const std::string& message, int port);
	//void log();
	bool getPrimaryVal();
	void setPrimaryVal();
	
private:
	bool primary;
	//int RM1,RM2;
	//std::vector<std::string> messages;
};

void sync();



#endif