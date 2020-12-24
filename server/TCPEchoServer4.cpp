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

/**
 \var  const int MAXPENDING
 \ variable statique qui specifie la longueur de la file pour les connexions en attente

**/
static const int MAXPENDING = 5; // Maximum outstanding connection requests


/**
 \fn sendInt(int controle, char* tosend, int sock)
 \brief Permet d envoyer des donnees au client au travers du socket
 \param int controle
 \ represente la quantite de donnee envoyees  a chaque envoie
 \param char* tosend
 \pointeur vers le premier caractere de la chaine de donnees a envoyer
 \int sock
 \ valeur du socket cote serveur
**/
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

/**
 \fn receiveInt(char* recv_buffer, int sock)
 \brief Permet de recevoir les donn´ees envoy´ees par le client
 \param char* recv_buffer
 \Pointeur vers le premier caractere de la chaine de caracteres envoyees par le client
 \int sock
 \ numero de socket pour la connexion au client
 \retourne le pointeur vers la prochaine chaine de caractere  a recevoir
**/
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

int getValue(string path){

	ifstream file;

	int line;

	file.open(path.c_str());

	if(file.is_open()){

		file >> line;

		return line;

		file.close();

	}

	else

	cout << "Unable to open file";

} 

/**
 \fn connectClient(int port_number)
 \brief Permet de creer un socket du cot´e client et le lie `a un port en attente de connexions avec un serveur
 \param int port_number
 \represente le numero de prot au travers duquel la connexion sera etablie
 \retourne le socket connect´e au client
**/
int connectClient(int port_number){
	  in_port_t servPort = port_number;

  // Create socket for incoming connections

/**
 \var int servSock
 \Descripteur du socket cot´e serveur
**/
  int servSock; // Socket descriptor for server
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithSystemMessage("socket() failed");


/**
 \struct  sockaddr_in servAddr
 \brief Structure contenant les attributs de l'addresse du serveur local
**/
  // Construct local address structure
  struct sockaddr_in servAddr;                  // Local address
  memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
  servAddr.sin_family = AF_INET;                // IPv4 address family
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
  servAddr.sin_port = htons(servPort);          // Local port


/**
 \liaison su socket a l'adresse locale
**/
  // Bind to the local address
  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("bind() failed");


/**
  \Socket du serveur accepte les connexions
**/
  
  if (listen(servSock, MAXPENDING) < 0)
    DieWithSystemMessage("listen() failed");

    struct sockaddr_in clntAddr; // Client address
    // Set length of client address structure (in-out parameter)

/**
 \Modifie la taille du socket contenant l'addresse du client
**/
    socklen_t clntAddrLen = sizeof(clntAddr);


/**
 \Attend la connexion du client
**/
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

   


/**
 \Var char clntName[INET_ADDRSTRLEN]
 \chaine de caract`ere qui contient l'addresse du client
**/
    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    else
      puts("Unable to get client address");
  
  return clntSock;
}



int main(int argc, char *argv[]) {
	
	int port_number = 4099;
	int clntSock = connectClient(port_number);
  
	VideoCapture cap(0);
	if(!cap.isOpened()){
		cout << "ERROR: Could not open camera" <<endl;
	}
    Mat image;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 176); //fastest resolution
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,144); //fastest resolution
	char* tosend;
	char* recv_buffer;
	string ADC_path = "/sys/class/saradc/ch0";
	string button_path = "/sys/class/gpio/gpio228/value";
	int lightIntensity,button_value;
	const int min_intensity = 950;
	const int max_intensity = 200;
	bool reset = 0;
	bool reset_button = 0;
	bool button_state;
	time_t start;
	int n_secondes = 5;
	start=time(0);
	int controle;
	//ofstream Myfile("analyse.txt");
  while(true){
	lightIntensity = getValue(ADC_path);
	button_value = getValue(button_path);
	if (button_value == 0){
		button_state = true;
	}
	else{
		button_state = false;
		reset_button = true;
	}
	//Myfile << lightIntensity << endl;
	if (time(0)-start > 0){
		start = start + n_secondes;
		cout << "Light intensity : " << lightIntensity << endl;
	} 
	if (lightIntensity < min_intensity && lightIntensity > max_intensity){
	reset = 0;
	if (button_state == true && reset_button == true){
	controle = PUSHB;
	reset_button = false;
	}
	else
		controle = READY;
	
	sendInt(controle, tosend, clntSock);
	
	
	cap >> image;

	sendInt(image.rows, tosend,clntSock);


	sendInt(image.cols, tosend, clntSock);

	int bytes;
	image = (image.reshape(0,1));
	int imgSize = image.total() * image.elemSize();
	bytes = send(clntSock,image.data,imgSize,0);
    if (bytes < 0){ 
         cout<<"ERROR writing to socket";
}
	
	if (controle == PUSHB){
		
		int id = fork();
		
		if (id == 0) {
			char *args[] = {"./Server2",NULL};
			execvp(args[0],args);
		}  	
	}	
	
controle=receiveInt(recv_buffer,clntSock);


if (controle == QUIT){
	  close(clntSock);

	break;
}

  if (controle == OK){
	  continue;
  }

if (controle == RES01){
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
}
if (controle == RES02){
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 176);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,144);

}

if (controle == RES03){
	cap.set(CV_CAP_PROP_FRAME_WIDTH,960);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,544);

}

if (controle == RES04){
	cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);

}
  }
  else{
	  if (reset == 0){
		cout << "Light intensity too low!" << endl;
		reset = 1;
	  }
	  controle = IDOWN;
	  sendInt(controle, tosend, clntSock);
  }
  }
  
exit(0);
	  }


