#include <stdio.h>

#include <stdlib.h>

#include <string>

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

#define OK 1

#define QUIT 0

#define RES01 2
#define RES02 3
#define RES03 4
#define RES04 5
#define READY 6
#define IDOWN 7
#define PUSHB 8

using namespace std;

using namespace cv;

void sendInt(int controle, char* tosend, int sock){

  	tosend = (char*)&(controle);

	 int remaining = sizeof(int);

	 int result = 0;

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

Mat makeImage(int height, int length, int sock){
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

return img;
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

	cout << "Bonjour!" << endl;

	cout << "En appuyant sur les touches suivantes, vous changez la configuration:"<<endl;

	cout << "a : 176 x 144" << endl;

	cout << "b : 960 x 544" << endl;

cout << "c : 800 x 600" <<endl;

cout << "d : 320 * 240" <<endl; 	

int controle, height, length;

char* recv_buffer;
int touche;	
string window_name = "My Camera Feed";
int nb_capture = 0; 
//string path = "/home/ele4205/Desktop/debug_client/my_image_";
	 while(true){
		 
	controle = receiveInt(recv_buffer, sock);
	
	if (controle == READY || controle == PUSHB){

height = receiveInt(recv_buffer,sock);

length = receiveInt(recv_buffer,sock);



Mat img =makeImage(height, length, sock);

if (controle == READY){
imshow(window_name, img); 

 touche = waitKey(30); 

  if(touche == 27){

	controle = QUIT;

	sendInt(controle, tosend, sock);
	
	  close(sock);

  exit(0);

	break;

  } else if(touche == 97){

	 controle = RES01;

	 sendInt(controle, tosend, sock);

  }

else if(touche == 98){

	controle = RES02;

	sendInt(controle, tosend, sock);

}

else if(touche == 99){

	controle = RES03;

	sendInt(controle, tosend, sock);

	

	

}  

  else if (touche == 100){

	 controle = RES04;

	sendInt(controle, tosend, sock);

	  

  }

  else{

	controle = OK;

	sendInt(controle, tosend, sock);

	 }
}
 else if (controle == PUSHB){
	 
	 int id = fork();
	
	if (id == 0){
			ostringstream stream;
	stream << "/home/ele4205/Desktop/debug_client/my_image_";
	char prev = stream.fill('0');
	stream.width(1);
	stream <<  nb_capture;
	stream.fill(prev);
	stream << ".png";	
	bool isSuccess = imwrite(cv::String(stream.str()),img); //write the image to a file as JPEG 
 if (isSuccess == false)
 {
  cout << "Failed to save the image" << endl;
 }
else{
 cout << "Image is succusfully saved to a file" << endl;
}
		break;
		
	}
	else {
		

	imshow(window_name, img); 

		controle = OK;
		
		sendInt(controle, tosend, sock);
		nb_capture++;

		
	}
	 
 }	 

 } else if (controle == IDOWN)
	 continue;

	 }


}
