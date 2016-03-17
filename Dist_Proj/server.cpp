/*
  From Stevens Unix Network Programming, vol 1.
  Minor modifications by John Sterling
 */

#include <stdio.h>       // perror, snprintf
#include <stdlib.h>      // exit
#include <unistd.h>      // close, write
#include <string.h>      // strlen
#include <strings.h>     // bzero
#include <time.h>        // time, ctime
#include <sys/socket.h>  // socket, AF_INET, SOCK_STREAM,
                         // bind, listen, accept
#include <netinet/in.h>  // servaddr, INADDR_ANY, htons
#include <string>
#include <iostream>
#include <cstring>
#include "appFunctions.h"


#define	MAXLINE		4096	// max text line length
#define	BUFFSIZE	8192    // buffer size for reads and writes
#define  SA struct sockaddr
#define	LISTENQ		1024	// 2nd argument to listen()
#define PORT_NUM    13002


using namespace std;

int main(int argc, char **argv) {
    int			listenfd, connfd;  // Unix file descriptors
    struct sockaddr_in	servaddr;          // Note C use of struct
    char		buff[MAXLINE];
    char        buffer[MAXLINE];
    time_t		ticks;

    // 1. Create the socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Unable to create a socket");
        exit(1);
    }

    // 2. Set up the sockaddr_in

    // zero it.  
    // bzero(&servaddr, sizeof(servaddr)); // Note bzero is "deprecated".  Sigh.
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET; // Specify the family
    // use any network card present
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(PORT_NUM);	// daytime server

    // 3. "Bind" that address object to our listening file descriptor
    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1) {
        perror("Unable to bind port");
        exit(2);
    }

    // 4. Tell the system that we are going to use this sockect for
    //    listening and request a queue length
    if (listen(listenfd, LISTENQ) == -1) {
        perror("Unable to listen");
        exit(3);
    }
    deleteAccount("Franky");

    for ( ; ; ) {
        
    // 5. Block until someone connects.
    //    We could provide a sockaddr if we wanted to know details of whom
    //    we are talking to.
    //    Last arg is where to put the size of the sockaddr if
    //    we asked for one
	fprintf(stderr, "Ready to connect.\n");
    if ((connfd = accept(listenfd, (SA *) NULL, NULL)) == -1) {
        perror("accept failed");
        exit(4);
	}

    fprintf(stderr, "Connected\n");
    memset(buff, 0, sizeof(buff)); //Clear buffer
    
    recv(connfd, buff, sizeof(buff), 0); //Wait and receive something
    string s(buff);
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
        string returnMsg = addFriend(tokens[1], tokens[2], tokens[3]);
        memset(buff, 0, sizeof(buff));
        send(connfd, returnMsg.c_str(), strlen(returnMsg.c_str()), 0);
    }
    else if(tokens[0] == "GETFRIENDS"){
    //Get list of friends
        string returnMsg = getFriends(tokens[1]);
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
        cout << username << ' ' << message << endl;
        addPost(username, message);
    }
    else if (tokens[0] == "GETMESSAGES") {

    }
    else if (tokens[0] == "DELETE") {

    }

    // 6. Close the connection with the current client and go back
    //    for another.
    close(connfd);
    }
}
