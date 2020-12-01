#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <iostream>

#include <opencv2/opencv.hpp>

#include <time.h>

#include <fstream>

#include "Practical.h"

#include "DieWithMessage.cpp"

#include "AddressUtility.cpp"


using namespace std;
/**
 \var  const int MAXPENDING
 \ variable statique qui specifie la longueur de la file pour les connexions en attente

**/
static const int MAXPENDING = 5; 


/**
 \fn connectClient(int port_number)
 \brief Permet de creer un socket du cot´e client et le lie `a un port en attente de connexions avec un serveur
 \param int port_number
 \represente le numero de prot au travers duquel la connexion sera etablie
 \retourne le socket connect´e au client
**/
int connectClient(int port_number){

	  in_port_t servPort = port_number;


/**
 \var int servSock
 \Descripteur du socket cot´e serveur
**/
  int servSock; 

  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)

    DieWithSystemMessage("socket() failed");



  // Construct local address structure

  struct sockaddr_in servAddr;                  // Local address

  memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure

  servAddr.sin_family = AF_INET;                // IPv4 address family

  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface

  servAddr.sin_port = htons(servPort);          // Local port



  // Bind to the local address

  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)

    DieWithSystemMessage("bind() failed");



  // Mark the socket so it will listen for incoming connections

  if (listen(servSock, MAXPENDING) < 0)

    DieWithSystemMessage("listen() failed");



    struct sockaddr_in clntAddr; // Client address

    // Set length of client address structure (in-out parameter)

    socklen_t clntAddrLen = sizeof(clntAddr);



    // Wait for a client to connect

    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);

    if (clntSock < 0)

      DieWithSystemMessage("accept() failed");



    // clntSock is connected to a client!



    char clntName[INET_ADDRSTRLEN]; // String to contain client address

/*     if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,

        sizeof(clntName)) != NULL)

      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));

    else

      puts("Unable to get client address");
 */
  

  return clntSock;

}

int receiveInt(char* recv_buffer, int sock){ 

int number;	

recv_buffer = (char*)&number;

int remaining = sizeof(int);

int received =0;

int result = 0;

while(remaining > 0){

	result = recv(sock, recv_buffer+received, remaining,0);

	if(result > 0){

		remaining -= result;

		received += result;

	}

	else if (result ==0){

	}

	else if (result < 0){

		cout << "ERROR!" << endl;

		break;

	}

	

}

	return number;

}

string morseEncode(char x) { 
  
  // refer to the Morse table 
  // image attached in the article 
  switch (x) { 
  case 'a': 
    return ".-";
	
case 'A': 
    return ".-";
	
  case 'b': 
    return "-...";
  case 'B': 
    return "-..."; 	
  case 'c': 
    return "-.-.";
  case 'C': 
    return "-.-.";	
  case 'd': 
    return "-..";
  case 'D': 
    return "-..";	
  case 'e': 
    return ".";
  case 'E': 
    return ".";	
  case 'f': 
    return "..-.";
  case 'F': 
    return "..-.";	
  case 'g': 
    return "--.";
  case 'G': 
    return "--.";	
  case 'h': 
    return "....";
  case 'H': 
    return "....";	
  case 'i': 
    return "..";
  case 'I': 
    return "..";	
  case 'j': 
    return ".---";
  case 'J': 
    return ".---"; 	
  case 'k': 
    return "-.-";
  case 'K': 
    return "-.-";	
  case 'l': 
    return ".-..";
  case 'L': 
    return ".-..";	
  case 'm': 
    return "--";
  case 'M': 
    return "--";	
  case 'n': 
    return "-.";
  case 'N': 
    return "-.";	
  case 'o': 
    return "---";
  case 'O': 
    return "---";	
  case 'p': 
    return ".--.";
  case 'P': 
    return ".--.";	
  case 'q': 
    return "--.-";
  case 'Q': 
    return "--.-";	
  case 'r': 
    return ".-.";
  case 'R': 
    return ".-.";	
  case 's': 
    return "...";
  case 'S': 
    return "...";	
  case 't': 
    return "-";
  case 'T': 
    return "-";	
  case 'u': 
    return "..-";
  case 'U': 
    return "..-";	
  case 'v': 
    return "...-";
  case 'V': 
    return "...-";	
  case 'w': 
    return ".--";
  case 'W': 
    return ".--";	
  case 'x': 
    return "-..-";
  case 'X': 
    return "-..-";	
  case 'y': 
    return "-.--";
  case 'Y': 
    return "-.--";	
  // for space 
  case 'z': 
    return "--..";
  case 'Z': 
    return "--..";	
  } 
}



int main(int argc, char *argv[]) {
	
	int dot = 60000;
	int dot_3 = 180000;
	
	string enablePath = "/sys/devices/pwm-ctrl.42/enable0";

	
	char* toreceive;
	
	int port_number = 4100;

	int clntSock = connectClient(port_number);
	
	int message_size = receiveInt(toreceive, clntSock);
	
 char buffer[BUFSIZE]; // Buffer for echo string

  // Receive message from client
  ssize_t numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
    DieWithSystemMessage("recv() failed");

ofstream myfile;
string code_char;

cout << "La chaîne de caractères est : ";

for (int i = 0 ; i < message_size ; i++){
	
	code_char = morseEncode(buffer[i]);
	
	cout << buffer[i];
	
	for(int j = 0 ; j < code_char.length() ; j++){
		if (code_char[j] == '.'){
			myfile.open(enablePath.c_str());
			myfile << 1;
			myfile.close();
			usleep(dot);
			myfile.open(enablePath.c_str());
			myfile << 0;
			myfile.close();
			usleep(dot);			
		}
		if (code_char[j] == '-'){
			myfile.open(enablePath.c_str());
			myfile << 1;
			myfile.close();
			usleep(dot_3);
			myfile.open(enablePath.c_str());
			myfile << 0;
			myfile.close();
			usleep(dot);
		}
	}
	usleep(2*dot);
	
	
}

cout << endl;

close(clntSock);
exit(0);
	  }





