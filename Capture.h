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
\permet de creer des objets ayant pour attributs une largeur, une longueur et un nombre de \capture par seconde.
**/
class Capture
{
private:
/**
\var ResX
\definit la largeur de l image ou de la video enregistree.
**/
int ResX;
/**
\var ResY
\definit la longueur de l image ou de la video enregistree.
**/
int ResY;
double fps;



public:
Capture();
~Capture();
int get_ResX();
int get_ResY();
double get_fps();

void set_fps(double FPS);
void set_ResX(int resX);
void set_ResY(int resY);
double cal_tps(int resX,int resY);


};

#endif /* CAPTURE_H_ */
