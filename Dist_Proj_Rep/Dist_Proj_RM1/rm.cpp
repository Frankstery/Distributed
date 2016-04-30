#include "rm.h"

using namespace std;

RepManager::RepManager(const bool p): primary(p) {}

string RepManager::sendMessage(const string& message, int port) {
	string ipaddr = "127.0.0.1";
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   	if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   	}
	
	// Connect to the remote server
	struct sockaddr_in remoteaddr;
	memset(&remoteaddr, '0', sizeof(remoteaddr)); 
	remoteaddr.sin_family = AF_INET;

	remoteaddr.sin_addr.s_addr = inet_addr(ipaddr.c_str());
	remoteaddr.sin_port = htons(port);
	
	if (connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr)) == -1) {
		cout << "Unable to connect";
		return "Unable to connect";
	}
		//cout << "Couldn't connect RM1" << endl;
		//string message1 = "Hello\n";
	//int n = write(sockfd,message.c_str(),sizeof(message));
	send(sockfd, message.c_str(), strlen(message.c_str()), 0);
	close(sockfd);


	//send(RM2, message.c_str(), strlen(message.c_str()), 0);
	return "Success";
}

bool RepManager::getPrimaryVal() {return primary;}

void RepManager::setPrimaryVal() {primary = 1;}



void synchronize() {} //Will send a message out to other RMs saying SYNC. 

