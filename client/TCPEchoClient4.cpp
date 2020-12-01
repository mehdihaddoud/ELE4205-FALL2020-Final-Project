#include <stdio.h>

#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <zbar.h>
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

using namespace zbar;

typedef struct
{
  string type;
  string data;
  vector <Point> location;
}decodedObject;
/**
 \
**/
// Find and decode barcodes and QR codes
void decode(Mat &im, vector<decodedObject>&decodedObjects)
{
	//Reference:https://www.learnopencv.com/opencv-qr-code-scanner-c-and-python/


  // Create zbar scanner
  ImageScanner scanner;

  // Configure scanner
  
    // disable all
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);

    // enable qr
    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
  // Convert image to grayscale
  Mat imGray;
  cvtColor(im, imGray,CV_BGR2GRAY);

  // Wrap image data in a zbar image
  Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);

  // Print results
  for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
    decodedObject obj;

    obj.type = symbol->get_type_name();
    obj.data = symbol->get_data();

    // Print type and data
    cout << "Type : " << obj.type << endl;
    cout << "Data : " << obj.data << endl << endl;
    decodedObjects.push_back(obj);
  }
}

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

int connectServer(int serv_port){
	char *servIP = NULL;
servIP = (char *)malloc(sizeof(char) * (12));
sprintf(servIP,"192.168.7.2");


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

  servAddr.sin_port = htons(serv_port);    // Server port



  // Establish the connection to the echo server


  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	DieWithSystemMessage("connect() failed");

	return sock;
}



int main(int argc, char *argv[]) {

int serv_port = 4099;

int sock = connectServer(serv_port);

	cout << "Bonjour!" << endl;

	cout << "En appuyant sur les touches suivantes, vous changez la configuration:"<<endl;

	cout << "a : 176 x 144" << endl;

	cout << "b : 960 x 544" << endl;

cout << "c : 800 x 600" <<endl;

cout << "d : 320 * 240" <<endl; 	

int controle, height, length;
 char* tosend;

char* recv_buffer;
int touche;	
string window_name = "My Camera Feed";
int nb_capture = 0; 
vector<decodedObject> decodedObjects;

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
 
   // Find and decode barcodes and QR codes
   decode(img, decodedObjects);
	int serv_port2 = 4100;

	int sock2 = connectServer(serv_port2);
	
	//string message = decodedObjects.data;
	
	int n =  decodedObjects.back().data.length();

	sendInt(n, tosend, sock2);

	char char_array[n+1];

	strcpy(char_array,decodedObjects.back().data.c_str());

	size_t echoStringLen = strlen(char_array); // Determine input length

  // Send the string to the server
	ssize_t numBytes = send(sock2, char_array, echoStringLen, 0);
	if (numBytes < 0)
		DieWithSystemMessage("send() failed");
	else if (numBytes != echoStringLen)
		DieWithUserMessage("send()", "sent unexpected number of bytes");
	
	close(sock2);
	exit(0);
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
