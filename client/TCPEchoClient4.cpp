#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include "Practical.h"
#include "AddressUtility.cpp"
#include "DieWithMessage.cpp"
//#include "TCPClientUtility.c"
using namespace std;
using namespace cv;
int main(int argc, char *argv[]) {

  if (argc < 3 || argc > 4) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",
        "<Server Address> <Echo Word> [<Server Port>]");

  char *servIP = argv[1];     // First arg: server IP address (dotted quad)
  char *echoString = argv[2]; // Second arg: string to echo

  // Third arg (optional): server port (numeric).  7 is well-known echo port
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;

  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(4099);    // Server port

  // Establish the connection to the echo server
 char* tosend;
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	  
    DieWithSystemMessage("connect() failed");
	 while(true){
int height = 0;
int length = 0;
char* recv_buffer = (char*)&height;
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
		cout << "Remote side closed his end of the connection before all data was received." <<endl;
		break;
	}
	else if (result < 0){
		cout << "ERROR!" << endl;
		break;
	}
	
}

recv_buffer = (char*)&length;
remaining = sizeof(int);
received = 0;
result = 0;
while(remaining > 0){
	result = recv(sock, recv_buffer+received, remaining,0);
	if(result > 0){
		remaining -= result;
		received += result;
	}
	else if (result ==0){
		cout << "Remote side closed his end of the connection before all data was received." <<endl;
		break;
	}
	else if (result < 0){
		cout << "ERROR!" << endl;
		break;
	}
	
}

Mat img = Mat::zeros(height, length, CV_8UC3);
int imgSize = img.total()*img.elemSize();
uchar sockData[imgSize];
int bytes;
if ((bytes = recv(sock, sockData, imgSize, 0)) == -1)  {
        cout << "Receive failed" << endl;
    }
for (int i = bytes; i < imgSize; i += bytes) {
    if ((bytes = recv(sock, sockData +i, imgSize  - i, 0)) == -1)  {
        cout << "Receive failed" << endl;
		break;
    }
}
// Assign pixel value to img
int ptr=0;
for (int i = 0;  i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {                                     
       img.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr+ 0],sockData[ptr+1],sockData[ptr+2]);
       ptr=ptr+3;
    }
}
	string window_name = "My Camera Feed";
  imshow(window_name, img); 
  
  if(waitKey(30) != -1){
	int controle = 0;
  	tosend = (char*)&(controle);
	 remaining = sizeof(int);
	 result = 0;
	 int sent = 0;
	while(remaining > 0){
		result = send(sock, tosend+sent,remaining,0);
		if (result > 0){
		remaining -= result;
		sent += remaining;
		}
	
	else if(result < 0){
		cout << "Error!" <<endl;
		break;
	}
	}
	break;
  }
	int controle = 1;
  	tosend = (char*)&(controle);
	 remaining = sizeof(int);
	 result = 0;
	 int sent = 0;
	while(remaining > 0){
		result = send(sock, tosend+sent,remaining,0);
		if (result > 0){
		remaining -= result;
		sent += remaining;
		}
	
	else if(result < 0){
		cout << "Error!" <<endl;
		break;
	}
	}

  
}
  close(sock);
  exit(0);
  
}
/* Mat  img = Mat::zeros( height,width, CV_8UC3);
int  imgSize = img.total()*img.elemSize();
uchar sockData[imgSize];

//Receive data here

for (int i = 0; i < imgSize; i += bytes) {
    if ((bytes = recv(connectSock, sockData +i, imgSize  - i, 0)) == -1)  {
        quit("recv failed", 1);
    }
}

// Assign pixel value to img
int ptr=0;
for (int i = 0;  i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {                                     
       img.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr+ 0],sockData[ptr+1],sockData[ptr+2]);
       ptr=ptr+3;
    }
} */
