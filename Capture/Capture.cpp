/* Capture.cpp
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.derekmolloy.ie
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that source code redistributions retain this notice.
 *
 * This software is provided AS IS and it comes with no warranties of any type.
 */
#include"Capture.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>



using namespace std;
using namespace cv;

Capture::Capture(){}

Capture::~Capture(){}

int Capture::get_ResX(){return ResX;}

int Capture::get_ResY(){return ResY;}

double Capture::get_fps(){return fps;}

void Capture::set_ResX(int resX){ResX=resX;}

void Capture::set_ResY(int resY){ResY=resY;}

void Capture::set_fps(double FPS){ fps=FPS;}


double Capture::cal_tps(int ResX,int ResY)
{


 VideoCapture cap(0);
 
cap.set(CV_CAP_PROP_FRAME_WIDTH, ResX);
 cap.set(CV_CAP_PROP_FRAME_HEIGHT, ResY);
if (!cap.isOpened()) {
	 cout << "Failed to connect to the camera." << endl;
   }
 
 Mat frame,autre;
 struct timespec start, end;
int frames =2;
for (int b=0; b<frames; b++) 
{
 cap >> autre;
}

 clock_gettime(CLOCK_REALTIME, &start);

  frames = 10;
 for (int a = 0; a < frames; a++) {
	 cap >> frame;
	 if (frame.empty()) {
		 cout << "Failed to capture an image" << endl;
		 return -1;
	 }
	 
 }

 clock_gettime(CLOCK_REALTIME, &end);
 double difference = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1000000000;
 //double FPS = (frames/difference);

return difference;
}








