#include<iostream>
#include<opencv2/opencv.hpp>
#include<time.h>


using namespace std;
using namespace cv;

Mat captureImage(){	

VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "ERROR: Could not open camera" << endl;
        //return false;
    }


    // this will contain the image from the webcam
    Mat frame;
        
    // capture the next frame from the webcam
    cap >> frame;

    bool isSuccess = imwrite("/home/root/wesh.jpg", frame);

     if (isSuccess == false)
 {
  cout << "Failed to save the image" << endl;
  cin.get(); //wait for a key press
  //return false;
 }

    cout << "Image is succusfully saved to a file" << endl;
    // display the frame until you press a key
    cap.release();
    destroyAllWindows();
	return frame;
}