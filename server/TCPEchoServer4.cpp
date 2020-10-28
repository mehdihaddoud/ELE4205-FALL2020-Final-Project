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
#include "Practical.h"
#include "DieWithMessage.cpp"
#include "AddressUtility.cpp"
#include "captureImage.cpp"

//#include "TCPServerUtility.cpp"

using namespace std;
using namespace cv;

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[]) {

  if (argc != 2) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  in_port_t servPort = atoi(argv[1]); // First arg:  local port

  // Create socket for incoming connections
  int servSock; // Socket descriptor for server
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
char* recv_buffer;
  for (;;) { // Run forever
    struct sockaddr_in clntAddr; // Client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

    // clntSock is connected to a client!

    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    else
      puts("Unable to get client address");
	VideoCapture cap(0);
	if(!cap.isOpened()){
		cout << "ERROR: Could not open camera" <<endl;
	}
    Mat image;
	
  while(true){
	
	cap >> image;
	/* bool isSuccess = imwrite("/home/root/wesh.jpg",image);
	if (isSuccess == false){
		cout << "Failed to save the image" << endl;
	}
	else{
		cout << "Image is succesfully saved to a file" << endl;
	} */
	char* tosend = (char*)&(image.rows);
	int remaining = sizeof(int);
	int result = 0;
	int sent = 0;
	while(remaining > 0){
		result = send(clntSock, tosend+sent,remaining,0);
		if (result > 0){
		remaining -= result;
		sent += remaining;
		}
	
	else if(result < 0){
		cout << "Error!" <<endl;
		break;
	}
	}
	tosend = (char*)&(image.cols);
	remaining = sizeof(int);
	sent = 0;
	result = 0;
	while(remaining > 0){
	result = send(clntSock, tosend+sent,remaining,0);
	if (result > 0){
	remaining -= result;
	sent += remaining;
	}
	
	else if(result < 0){
	cout << "Error!" <<endl;
	break;
	}
	}
	int bytes;
	image = (image.reshape(0,1));
	int imgSize = image.total() * image.elemSize();
	bytes = send(clntSock,image.data,imgSize,0);
    if (bytes < 0){ 
         cout<<"ERROR writing to socket";
}
	//HandleTCPClient(clntSock);
	//cap.release();
	//destroyAllWindows();
	
int controle=0;
recv_buffer = (char*)&controle;
remaining = sizeof(int);
int received =0;
result = 0;
while(remaining > 0){
	result = recv(clntSock, recv_buffer+received, remaining,0);
	if(result > 0){
		remaining -= result;
		received += result;
	}
	else if (result ==0){
		//cout << "Remote side closed his end of the connection before all data was received." <<endl;
		//break;
	}
	else if (result < 0){
		cout << "ERROR!" << endl;
		break;
	}
	
}

if (controle == 0){
	  close(clntSock);

	break;
}
  }
  
  break;

  }
  exit(0);
  // NOT REACHED
}

/* Mat frame;
frame = (frame.reshape(0,1)); // to make it continuous

int  imgSize = frame.total()*frame.elemSize();

// Send data here
bytes = send(clientSock, frame.data, imgSize, 0))
 */
