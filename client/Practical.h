#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
/**
  \fn DieWithUserMessage(const char *msg, const char *detail)
  \brief Permet de g´erer les erreur en affichant un message fourni par l'utilisateur
  \param const char *msg 
  ref´rence au message `a afficher.
  \param const char *detail
  ref´erence au contenu du message.
**/
void DieWithUserMessage(const char *msg, const char *detail);



/**
  \fn DieWithUserMessage(const char *msg, const char *detail)
  \brief Permet de g´erer les erreur en affichant un message fourni par le syst`eme.
  \param const char *msg 
  ref´rence au message `a afficher.
**/
void DieWithSystemMessage(const char *msg);


/**
   \fn PrintSocketAdress(const struct sockaddr *address, FILE *stream)
   \brief Affiche l'adresse associ´ee au socket et l'envoie au discripteur de flux
   \param const struct sockaddr *address
   \ref´erence `a l'addresse `a afficher
   \ File *stream
**/
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);


/**
  \fn SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2)
  \brief V´erifie si deux addresses de socket sont identiques
  \param const struct sockaddr *addr1
  \ref´erence `a l'addresse du premier socket
  \param const struct sockaddr *addr2
  \ref´erence `a l'addresse du premier socket
  

**/
bool SockAddrsEqual(const struct sockaddr *addr1, const struct sockaddr *addr2);


/**
 \fn SetupTCPServerSocket(const char *service)
 \brief Permet de configurer le socket TCP du Serveur
 \param const char *service
 \Pointeur vers le caract`ere qui repr´esente le num´ero de port
 \retourne (-1) en cas d'´echec ou un socket valide pour le serveur

**/
int SetupTCPServerSocket(const char *service);


/**
  \fn AcceptTCPConnection(int servSock)
  \brief Accepte la connexion TCP du cot´e serveur au socket
  \param int servSock
  \Socket cot´e serveur
  \retourne la valeur du socket cot´e client

**/
int AcceptTCPConnection(int servSock);


/**
 \fn HandleTCPClient(int clntSocket)
 \brief Permet de g´erer ou de configurer la connexion TCP cot´e client
 \param int clntSocket
 \valeur du socket cot´e client

**/
void HandleTCPClient(int clntSocket);


/**
 \fn SetupTCPClientSocket(const char *server, const char *service)
 \brief Permet de configurer le socket TCP du client
 \param const char *server 
 \ref´erence au serveur avec lequel la connexion va etre ´etablie
 \Param const char *service
 \Pointeur vers le caract`ere qui repr´esente le num´ero de port
 \retourne (-1) en cas d'´echec ou un socket valide pour le serveur

**/
int SetupTCPClientSocket(const char *server, const char *service);

enum sizeConstants {
  MAXSTRINGLENGTH = 128,
  BUFSIZE = 512,
};

#endif // PRACTICAL_H_
