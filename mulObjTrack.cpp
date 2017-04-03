
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Fruit.h"


//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//Now I introduce a time eval function to reduce unwanted zooming ops
int time_eval = 5;
int zoom_eval = 5;
int sess_eval = 50;
int draw_eval = 5;
//And here I check the zooming value so that I can adjust the radius of the brush accordingly
int brush_rad = 1;
//Here is the last point variable to make the drawing appear more continous
Point Smoother(-1,-1);
Point mid, temp;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

Mat img(480, 640, CV_8UC3, Scalar(255,255,255));
//the buffer for zooming image

static double distPts(const cv::Point a, const cv::Point b)
{
    double xDiff = a.x - b.x;
    double yDiff = a.y - b.y;

    return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed





}

static void onPoint(int event, int x, int y, int, void* imgptr){
	static int i=0, j=0 , k=0;
	float bias = 0.1;
	int phld;
	//static Point prev;
    if ( event != 1 ) return;// only draw on lmouse down

    Mat & img = (*(Mat*)imgptr); // first cast, then deref
    Point pt1 = Point(x, y);
	if(x > 520 && x < 610 && y >100 && y < 150){
		j = 255;i=0; k=0;
	}
	else if(x > 30 && x < 120 && y >100 && y < 150){
		j = 0;i=0; k=255;
	}
	else if(x > 280 && x < 370 && y >100 && y < 150){
		j = 0;i=255; k=0;
	}
	else if(x > 0 && x < 100 && y >400 && y < 480){
		i=0;j=0;k=0;
		Mat temp(480, 640, CV_8UC3, Scalar(255,255,255));
		img = temp;
		Smoother.x = -1;
		Smoother.y = -1;
	}

	// It is now time to improve the drawing function by making it seem
	// that the patches between two consecutive dots are filled!
    circle(img, pt1, brush_rad, Scalar(i, j, k), 15, 8, 0);
	
	//This should draw short lines between patchy areas making it seem
	// more continous
	//if(draw_eval != 0){
	
	if (Smoother.x != -1 && Smoother.y != -1){
		// it first checks if the distance between current
		// and last point is not greater than 50 else the curve will look more
		// line an inelegant line
		if (distPts(pt1, Smoother)<50){
			line(img, pt1 , Smoother ,Scalar(i, j, k), 15, 8, 0);
		}
		//however, if the distance between two points is grater than that,
		// and we still wanna show a smooth curve, a straight line won't work for this
		// So, I have written a dynamic point generation algorithm that will create random points in between the long distance to make it seem
		// as it was drawn by hand
		else{
			phld = int(sqrt((distPts(pt1, Smoother))));
			cout << phld ;
			cout << "\n";
			temp = pt1;
			for(int x = 1; x <= phld; x++){
				//mid = (Smoother + temp)*(float(x/phld));
				mid.x = pt1.x + float(x/phld)*(Smoother.x - pt1.x);
				mid.y = pt1.y + float(x/phld)*(Smoother.y - pt1.y);
				//adding randomness biases
				mid.x += rand()%10 +1;
				mid.y += rand()%10 +1;
				line(img, temp , mid ,Scalar(i, j, k), 15, 8, 0);
				temp = mid;
			}
		}
		
	}
	if(draw_eval > 0){
	Smoother = pt1;
	draw_eval --;
	
	}
	else{
		draw_eval = 3;
	Smoother.x = -1;
	Smoother.y = -1;
	}
	//cout << "\nDval: ";
	//cout<<draw_eval;
	
    //imshow("Display window", img);
    //waitKey(1);*/
}

string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}

void createTrackbars(){
	//create window for trackbars


	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}

void drawObject(vector<Fruit> theFruits,Mat &frame){

	for(int i =0; i<theFruits.size(); i++){

	cv::circle(frame,cv::Point(theFruits.at(i).getXPos(),theFruits.at(i).getYPos()),10,cv::Scalar(0,0,255));
	//cv::putText(frame,intToString(theFruits.at(i).getXPos())+ " , " + intToString(theFruits.at(i).getYPos()),cv::Point(theFruits.at(i).getXPos(),theFruits.at(i).getYPos()+20),1,1,Scalar(0,255,0));
	cv::putText(frame,"+",cv::Point(theFruits.at(i).getXPos(),theFruits.at(i).getYPos()-30),1,2,theFruits.at(i).getColour());
	}
}

void drawPointer(vector<Fruit> theFruits,Mat &frame){

	for(int i =0; i<theFruits.size(); i++){

	cv::circle(frame,cv::Point(theFruits.at(i).getXPos(),theFruits.at(i).getYPos()),10,cv::Scalar(0,0,255));
	//cv::putText(frame,intToString(theFruits.at(i).getXPos())+ " , " + intToString(theFruits.at(i).getYPos()),cv::Point(theFruits.at(i).getXPos(),theFruits.at(i).getYPos()+20),1,1,Scalar(0,255,0));
	cv::putText(frame,"X",cv::Point(theFruits.at(i).getXPos(),theFruits.at(i).getYPos()-30),1,2,theFruits.at(i).getColour());
	onPoint(1,theFruits.at(i).getXPos(),theFruits.at(i).getYPos(),1,&img);
	//draw_eval --;
	}
}

void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);

	//applied extra dialation for distinctly recognizing an image
	dilate(thresh,thresh,dilateElement);



}

void trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed){

	
	vector <Fruit> apples;

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){

					Fruit apple;
					
					apple.setXPos(moment.m10/area);
					apple.setYPos(moment.m01/area);
					

					apples.push_back(apple);

					objectFound = true;

				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				//draw object location on screen
				drawObject(apples,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

int roundUp(int to)
{
  return (to % 2 == 0) ? to : (to + 1);
}

void trackFilteredObject(Fruit theFruit,Mat threshold,Mat HSV, Mat &cameraFeed){

	
	vector <Fruit> apples;

	Mat temp;
	static Mat original, zooming; //original stores the actual canvas
	// for zooming in and zooming out
	static int curr, last;
	Point pt0(0,0);
	Point pt1(0,0);

	threshold.copyTo(temp);
	img.copyTo(original);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){

					Fruit apple;
					
					apple.setXPos(moment.m10/area);
					apple.setYPos(moment.m01/area);
					apple.setType(theFruit.getType());
					apple.setColour(theFruit.getColour());

					apples.push_back(apple);

					objectFound = true;

				}else objectFound = false;


			}
			
			//let user know you found an object
			//last = 0;
			if(objectFound ==true){
				
				//cv::putText(cameraFeed,"no of pens: "+intToString(numObjects),Point(100, 30),1,2,Scalar(0,255,0));
				if(numObjects >= 2){
					drawObject(apples,cameraFeed);
					if(time_eval  != 0){
						//Let's see how this works out
						time_eval--;
					}
					else if(zoom_eval != 0 ){
						time_eval = 5;
					pt0.x = apples.at(0).getXPos();
					pt1.x = apples.at(1).getXPos();
					pt0.y = apples.at(0).getYPos();
					pt1.y = apples.at(1).getYPos();
					// Now the curr value is constantly changing because of errors in my webcam
					//So, I introduced a tolerance so that curr doesn't vary too greatly!
					curr = distPts(pt0, pt1);

					//sexyy! Now this has an amazing tolerance! This way, There won't be a lot of 
					//errors
					curr = roundUp(int(curr/30))*30;

					cv::putText(cameraFeed,intToString(curr),pt0,1,2,Scalar(0,255,0));
					cv::putText(cameraFeed,intToString(last),pt1,1,2,Scalar(0,255,0));
					if (curr > last){
						last = curr;
						pyrUp(original, zooming, Size( original.cols*2, original.rows*2 ) );
						// I've added this functionality called as zoom_eval
						// Which waits for a few seconds after zooming in 
						//or zooming out. This is so that we can reduce human errors while
						//taking away the second pointer after zooming

						zoom_eval = 5;
						//brush_rad *= 2;

					}
					
					else if (curr < last){

						last = curr;
						pyrDown(original, zooming, Size( original.cols/2, original.rows/2 ) );
						// I've added this functionality called as zoom_eval
						// Which waits for a few seconds after zooming in 
						//or zooming out. This is so that we can reduce human errors while
						//taking away the second pointer after zooming
						zoom_eval = 5;
						if(brush_rad != 1){
						//brush_rad /= 2;
						}
					}

					//Here I am creating a temporary zoom in/out window to view the image
					// and I am destroying it after 2 seconds so that user cannot cause loss in resolution.

					//Finally I decided not to destroy the temporary window coz its nyc.
					imshow("zoom", zooming);
					sess_eval = 50;
					//waitKey(2000);
					//destroyWindow("zoom");
					
					//one probelm with this is that the original image is downsampled and up sampled. This results in loss of resolution
					//^Solved this by adding a temporary window.
					original = zooming;
					}
					else{
					zoom_eval --;
					}
					
				}
				else if(numObjects == 1){
					//last = 0;
					time_eval = 5;
				drawPointer(apples,cameraFeed);
				}
				
				// makes the window dissappear when not in use
				// allows the user to manipulate the zoom window while there are 2 pointers present in the system
				// and when not in use, removes the window
				if(sess_eval == 0){
						sess_eval = 50;
						destroyWindow("zoom");
					}
					else{
						sess_eval --;
					}
				//^amazing piece of code right here
				     }
			/*
			else{
				Smoother.x = -1;
				Smoother.y = -1;
				draw_eval = 0;
				cout << "\nthisDval: ";
				cout<< draw_eval;
				
			}*/

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

int main(int argc, char* argv[])
{
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = true;
	
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;
	cv::Mat flip, canFlip;  //coz i want mirror img
	if(calibrationMode){
		//create slider bars for HSV filtering
		createTrackbars();
	}
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

		if(calibrationMode==true){
		//if in calibration mode,  track objecwets based on the HSV slider values.
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		morphOps(threshold);
		imshow(windowName2,threshold);
		trackFilteredObject(threshold,HSV,cameraFeed);
		}else{
		//create some temp fruit objects so that
		//we can use their member functions/information
		Fruit apple("Green");// banana("banana"), cherry("cherry";

		
		//first find apples

		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,apple.getHSVmin(),apple.getHSVmax(),threshold);
		morphOps(threshold);
		trackFilteredObject(apple,threshold,HSV,cameraFeed);
		//then bananas
		/*
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,banana.getHSVmin(),banana.getHSVmax(),threshold);
		morphOps(threshold);
		trackFilteredObject(banana,threshold,HSV,cameraFeed);
		//then cherries
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,cherry.getHSVmin(),cherry.getHSVmax(),threshold);
		morphOps(threshold);
		trackFilteredObject(cherry,threshold,HSV,cameraFeed);*/

		}

		//show frames 
		//imshow(windowName2,threshold);
		cv::flip(img, canFlip, 1);
		imshow("Canvas", canFlip);
		//color pallete
		//Red
		cv::rectangle(cameraFeed, cv::Point(30, 100), cv::Point(120, 150), cv::Scalar(0, 0, 255), -1);
		//Blue
		cv::rectangle(cameraFeed, cv::Point(280, 100), cv::Point(370, 150), cv::Scalar(255, 0, 0), -1);
		//Green
		cv::rectangle(cameraFeed, cv::Point(520, 100), cv::Point(610, 150), cv::Scalar(0, 255, 0), -1);

		cv::rectangle(cameraFeed, cv::Point(520, 420), cv::Point(610, 480), cv::Scalar(255, 255, 255), -1);
		//This was my old approach (i.e. label using texts) but now I use solid rectangles as a pallette
		//cv::putText(img,"Red",cv::Point(10, 150),1,2,Scalar(0,0,255));
		//cv::putText(img,"Blue",cv::Point(320, 150),1,2,Scalar(255,0,0));
		//cv::putText(img,"Green",cv::Point(520, 150),1,2,Scalar(0,255,0));
		cv::flip(cameraFeed, flip, 1);  
		cv::putText(flip,"Reset",cv::Point(530, 420),1,2,Scalar(0,255,0));
		imshow(windowName,flip);
		//imshow(windowName1,HSV);


		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(1);
	}


	return 0;
}