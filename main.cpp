/*
 * main.c
 *
 *  Created on: Jul 17, 2015
 *      Author: 4205_2
 */

#include"Capture.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>

using namespace std;
using namespace cv;

#define nbr_res 13

 int Choix_resolution() 
 {
        int i=0;
	cout << "Choisissez un numero de resolution dans le tableau" << endl;
        cin >> i;
	   
    return i;

 }


int main()
{

int Tab[13][2]
	{ 
          {176,144},
          {160,120},
          {320,176},
          {320,240},
          {352,288},
          {432,240},
          {800,600},
          {864,480},
          {960,544},
          {960,720},
          {1184,656}, 
          {1280,720},
          {1280,960}
};

Capture Cam[nbr_res];

int frames =10;

    for(int i=0; i<nbr_res; i++)
   {
       Cam[i].set_ResX(Tab[i][0]);
       Cam[i].set_ResY(Tab[i][1]);
       Cam[i].cal_tps(Cam[i].get_ResX(),Cam[i].get_ResY());
       double diff=Cam[i].cal_tps(Cam[i].get_ResX(),Cam[i].get_ResY());
       Cam[i].set_fps(frames/diff) ;
       cout << "Res" << i+1 << ":" << Tab[i][0] <<"X"<< Tab[i][1]<< endl;
       cout << "------------------"<< endl;
       cout << "It took " << diff << " seconds to process " << frames << " frames" << endl;
       cout << "Capturing and processing " << frames/diff << " frames per second " << endl;
       cout << "------------------------------------------------------"<< endl;
       cout << "------------------------------------------------------"<< endl;
    }
   
   
   int index = Choix_resolution() ;
   
   int val = index-1;

    //Open the default video camera
    VideoCapture cap(0);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video camera" << endl;
        //cin.get(); //wait for any key press
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,Cam[val].get_ResX());
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,Cam[val].get_ResY());

   // int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
  //  int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    
    double frame_per_second = Cam[val].get_fps();

    //Create and initialize the VideoWriter object 
   //Cam[val].videoWrite(frame_width,frame_height,frame_per_second);

VideoWriter oVideoWriter("/home/root/capture-liv1.avi", CV_FOURCC('M', 'J', 'P', 'G'),frame_per_second, Size(Cam[val].get_ResX(),Cam[val].get_ResY()));
    int count =0;
    int nb_sec = 5;
  //VideoCapture cap(0);
    while (count<(nb_sec*frame_per_second))
    {
        Mat frame;
        cap >> frame;
       // bool isSuccess = cap.read(frame); // read a new frame from the video camera
       
        //Breaking the while loop if frames cannot be read from the camera
        if (frame.empty())
        {
            //cout << "Video camera is disconnected" << endl;
            //cin.get(); //Wait for any key press
            break;
        }
       
        oVideoWriter.write(frame);
        
   
        char c = (char)waitKey(1);
        if (c == 27)
        {
            cout << "Esc key is pressed by the user. Stopping the video" << endl;
            break;
         
        }
       count++;
    }

    //Flush and close the video file
    cap.release();
    oVideoWriter.release();

    destroyAllWindows();
	return 0;
}
