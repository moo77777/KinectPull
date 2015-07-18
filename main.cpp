#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>
#include <C:\Program Files (x86)\Eigen\Core>
#include <iostream>
using namespace Eigen;


const int width = 640;
const int height = 480;

//Kinect Veriables
HANDLE rgbStream;   //Kinect Camera Identifier
HANDLE depthStream;  
INuiSensor* sensor;
int data[width*height];
int output[width*height];

//Matrix variables
// MatrixXi m(640,480);

//Kinect Initialization
bool initKinect() {
	// Get working kinect sensor
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	//Initialize Sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	sensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_DEPTH,
		NUI_IMAGE_RESOLUTION_640x480,
		0,
		2,
		NULL,
		&depthStream);

	return sensor;

}

void getKinectData(int* data) {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if(sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame) < 0) return;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0) 
	{
		const USHORT* curr = (const USHORT*) LockedRect.pBits;
		const USHORT* dataEnd = curr + (width*height);

		while (curr < dataEnd) {
			//Get Depth in mm
			USHORT depth = NuiDepthPixelToDepth(*curr++);

			//Insert code to build a 640x480 matrix
			for (int i = 0; i= height*width - 1; ++i){
				*data++ = depth;

			}
		}
	}
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);

}


int main(int argc, char* argv[]) {
	if(!initKinect()) return 1;
	getKinectData(output);
	/* 
	MatrixXi image(640,480);
	int counter = 0;
	for (int i = 0; i<height - 1; ++i){
		for (int j = 0; j < width - 1; ++j){
			image(i,j) = output[counter];
			counter++;
		}
	}
	*/
	std::cout << output[10]<< std:: endl;
	std::cout << output[200]<< std:: endl;
	std::cout << output[300]<< std:: endl;
}
