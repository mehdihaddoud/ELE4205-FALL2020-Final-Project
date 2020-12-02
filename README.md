# Projet Ele4205 Automne 2020	{#mainpage}

# Description du Projet

Ce projet est une application client/serveur dans lequel le serveur 
 qui utlise l'Odroid-c2 pour capturer une image et envoie un unint32_t
 au client via une connexion TCP/IP  contenant un message:READY, IDOWN ou PUSHB.
 Si le message envoyé est un READY cela signifie qu'il y a de la lumière et on peut transférer l'image.Le client demande alors l'image
 et choisit la résolution dans laquelle il veut la recevoir en flux continu et peut changer de résolution à tout moment.
 Si le message envoyé est un IDOWN cela signifie qu'il n'y a pas de lumière et pas d'image disponible,le client ne demande pas d'image.
 Si le message envoyé est un PUSHB cela signifie qu'il y a de la lumière,que le boutton pression est enfoncé et une image est disponible,
 le client fait un fork():le parent process affiche l'image comme dans le cas du READY tandis que le child process devra faire de la détection 
 de code QR et sauver l'image.Le client va se connecter sur un nouveau port(41000) pour envoyer 
 le nombre de caractère dans le code QR reçu suivi de la chaine de caractères.
 Le nouveau serveur reçoit cette chaine de caratères et la joue en code morse ITU sur le buzzer.Après avoir terminé de jouer la 
 chaine de caractères,le programme en code morse se termine.

\image html img.png

# Technologies utilisées
 La capture de l'image et la détection de al luminosité se font du côté serveur
 au travers d'un certains nombre d'équipements
  - L'Odroid-c2
  Il s'agit d'un ordinateur à carte unique développé par HardKernel et doté d'un processeur quad core 64 bits ARM Cortex-A53 , 2GHz.
  il sert d'interface entre le serveur et le hardware du projet (camera, photorésistance, buzzer) 
  - Une caméra logitech (720p/30 ips) qui permet de capturer l'image en cas de luminosité
  - un buzzer
  - une photorésistance qui détecte le taux de luminosité
  - un boutton poussoir dont l'état peut être lu par l'odroid via l'ADC et qui lorsqu'il est appuyé parmet au serveur
    sur l'Odroid d'envoyer le message PUSHB au client sur la ligne de commande linux.

### Contribution au Projet
 Nous avons été deux à réaliser ce projet
 -Essono Michel Wilfred
 -Mehdi Haddoud
 Avec l'assistance de :
 Étienne le chargé de laboratoires
 et de :
 Richard Gourdeau le professeur
 
### Références
 
OpenCV Tutorial C++ :
- OpenCV Tutorial C++


Site Beaglebone de Derek Molloy :
- Beaglebone: Video Capture and Image Processing on Embedded Linux
  using OpenCV
- Streaming Video using RTP on the Beaglebone Black

Video for linux 2 (V4L2) : utilitaire de contrôle de parametres de webcam :
- Beaglebone Images, Video and OpenCV
- Video for Linux Two API Specification

TCP/IP :
- TCP/IP Sockets in C (Second Edition) disponible en-ligne `a Polytechnique
- C++ OpenCV image sending through socket
 
  










   
