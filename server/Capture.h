/*
 * Capture.h
 *
 *  Created on: Jul 17, 2015
 *      Author: 4205_2
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#include <stdint.h>
#include <stdlib.h>
/**
\class capture 
\brief permet de creer des objets ayant pour attributs une largeur, une longueur et un nombre de \capture par seconde.
**/
class Capture
{
private:
/**
\var ResX
\brief definit la largeur de l image ou de la video enregistree.
**/
int ResX;
/**
\var ResY
\brief definit la longueur de l image ou de la video enregistree.
**/
int ResY;
/**
\brief definit le nombre de frames par secondes de la video
**/
double fps;



public:
/**
 \brief Constructeur de la classe Capture.Permet de creer un objet de la classe capture avec des parametres par defaut
**/
Capture();
/**
  \brief Destructeur de la classe Capture.Libere automatiquement les donnees allouees par les conteneurs de la STL
**/
~Capture();
/**
  \fn get_ResX()
  \brief Permet d'acceder au contenu de la variable privee ResX d'un objet capture
  \retourne la valeur de ResX
**/
int get_ResX();
/**
 \fn get_ResY()
  \brief Permet d'acceder au contenu de la variable privee ResX d'un objet capture
  \retourne la valeur de ResY
**/
int get_ResY();
/**
 \fn get_fps()
 \brief permet d'acceder au nombre de frame par seconde qui est une variable privee
 \retourne le nombre frame par seconde
**/
double get_fps();
/**
 \fn set_fps(double FPS)
 \brief Permet de modifier le nombre de frame par seconde
 \param double FPS
 \nouvelle valeur de nombre de frame par seconde
**/
void set_fps(double FPS);
/**
 \fn set_ResX(int resX)
 \brief Permet de modifier la variable ResX
 \param int resX
 \Nouvelle valeur de ResX
**/
void set_ResX(int resX);
/**
 \fn set_ResY(int resY)
 \brief Permet de modifier la variable ResY
 \param int resY
 \Nouvelle valeur de ResY
**/
void set_ResY(int resY);
/**
 \fn cal_tps(int resX,int resY)
 \brief Permet de calculer la duree de capture d'une video
 \param int resX
 \resolution suivant l'axe x de la video (longueur)
 \param int resY
 \resolution suivant l'axe y de la video (largeur)
**/
double cal_tps(int resX,int resY);


};

#endif /* CAPTURE_H_ */
