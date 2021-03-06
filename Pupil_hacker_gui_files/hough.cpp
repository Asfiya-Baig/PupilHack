/*
Faults:
1. SLOW
2. When circle detected on the corners due to errors, roi goes out of the Current Mat. Produces an error.
3. Whenever roimat is black take value from previous frame.
5. Should try using RANSAC ellipse fitting to detect pupil in roi instead of thresholding- more accuracy will be obtained.
*/
#include <typeinfo> 
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <time.h>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace cv;

struct rad_data{
float radiusF;
Mat Cont_Image;

};
char patdat[50], patdat1[50], fname[100];
ifstream infile("../Data/data.csv");		//Takes input from Pupil_Hacker_Gui for reading patient data and stores in the csv file	
int USB = open( "/dev/ttyACM0", O_RDWR| O_NONBLOCK | O_NDELAY );	//Opening communication with the arduino for checking LED status

int framewidth_roimat=72, frameheight_roimat=72;
char key;
int attempts=15, pi=3.14;
vector<Point> approx;
Point2f center_Contour;
float radius_Contour;
//int radius_l, radius_r;
//Point center_l, center_r, point1_l, point1_r, point2_l, point2_r;
double len, area,p;
Mat image_RGB, image,image_left, image_right, grad_x, grad_y, grad, labels, centers, out, IL, IR, roimat_left, roimat_right, Kleft, Kright;
Rect roi;
RotatedRect temp;
Mat drawing_left, drawing_right;
Mat roimat,roimat_x, roimat_y, out_left,out_right,output, hist, histImage, hist_left, hist_right,  drawing, grad_x1, grad_y1, grad1, image4;
Point point1, point2;
int radius;
int histSize = 256;		//no of bins in the histogram
float range[] = { 0, 255 } ;	//range of intensity values
const float* histRange = { range };
int hist_w = 512; int hist_h = 400;	//width and height of window to display the histogram
int bin_w = cvRound( (double) hist_w/histSize );	//bin_w=2



Mat houghC(Mat  image1)
{	

	cvtColor( image1 , image, CV_RGB2GRAY );
	//
	/// Gradient X
 	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
 	Sobel( image.clone(), grad_x, CV_16S, 1, 0, 3);
  	convertScaleAbs( grad_x,grad_x );

  	/// Gradient Y
  	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  	Sobel( image.clone(), grad_y, CV_16S, 0, 1, 3);
  	convertScaleAbs( grad_y, grad_y );

  	/// Total Gradient (approximate)
  	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad );
	

	vector<Vec3f> circles;
	HoughCircles(grad, circles, CV_HOUGH_GRADIENT, 1, 200, 150, 30, 0, 30 );
	roimat = Mat::zeros(Size(72,72),CV_8UC1);
		for( size_t i = 0; i < circles.size(); i++ )
	{
		
			radius = cvRound(circles[i][2]);
   			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			Point point1(cvRound(circles[i][0])-2*radius, cvRound(circles[i][1])-2*radius);
			Point point2(cvRound(circles[i][0])+2*radius, cvRound(circles[i][1])+2*radius);
   			Point point3(cvRound(circles[i][0])-1.5*radius, cvRound(circles[i][1])-1.5*radius);
			//circle( image1, center, 3, Scalar(0,255,0), -1, 8, 0 );
			rectangle(image1, point1, point2, Scalar(0,0,255),2,8,0);
			//Create a bounding box
			//cout<<radius<<endl;
			roi = Rect(point3.x,point3.y,3*radius, 3*radius);	//put 100 in place of 3*radius to observe dilation	
			
			
			roimat = image1(roi);
			
			
			//GaussianBlur(roimat, roimat,Size(9,9),0);	
			cvtColor(roimat, roimat, CV_RGB2GRAY);
			//equalizeHist( roimat, roimat );
			normalize(roimat, roimat, 0, 255, NORM_MINMAX);
			framewidth_roimat= roimat.cols;
			frameheight_roimat = roimat.rows;
			//namedWindow("d",WINDOW_NORMAL);		
       			//imshow("d",image1); 
			//Canny(roimat.clone(), roimat ,200, 195);
			 //Sobel( roimat.clone(), roimat_x, CV_16S, 1, 0, 3);
			 //convertScaleAbs( roimat_x, roimat_x);
	
			 //Sobel( roimat.clone(), roimat_y, CV_16S, 0,1, 3);
			 //convertScaleAbs( roimat_y, roimat_y);

			//addWeighted( roimat_x, 0.5, roimat_y, 0.5, 0, roimat );
			//threshold(roimat.clone(),roimat, 0, 255, THRESH_BINARY_INV|THRESH_OTSU);
			//morphologyEx(roimat, roimat, MORPH_OPEN,3); 	// for closing the contours  
	}

return roimat;	

}

rad_data postP(Mat image3, int a)
{

rad_data object;
//Lets try using hough circles again
//double m1, var1;
//meanStdDev(image3,m1,var1,Mat());			//Taking mean and std deviation of the left eye region values	doubt3
//cout<<"m1 is: "<<m1-var1;
cvtColor(image3.clone(), drawing, CV_GRAY2BGR);

int thresh= image3.at<uchar>(image3.cols/2, image3.rows/2);
//cout<<"threshold value is: "<<thresh<<endl;
threshold(image3, out, thresh+a, 255, THRESH_BINARY_INV);
vector<vector<Point> > contours;
findContours(out.clone(), contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
for( int i = 0; i< contours.size(); i++ )
{
len = arcLength(contours[i], true); 
area=contourArea(contours[i], false);	
if( area> 200 )
{
approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
if(approx.size()>=8 && approx.size()< 15)
//min enclosing circle
minEnclosingCircle(contours[i],  center_Contour ,  radius_Contour);

circle( drawing, center_Contour, radius_Contour, Scalar(0,0,255), 1, 8, 0 );
object.radiusF = radius_Contour ;

}
}
object.Cont_Image = drawing;

return object;
}




int main()
{

  char flag[1];
	int n;
  /* Error handling in case Arduino does not respond to the serial communication request*/
  if ( USB < 0 ){
    cout << "Error " << errno << " opening " << "/dev/ttyACM0" << ": " << strerror (errno) << endl;
  }

  /*Setting the baud rate and other parameters for serial communication with arduino*/
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  cfsetospeed (&tty, B9600);
  cfsetispeed (&tty, B9600);
  memset (&flag, '\0', sizeof flag);

  /* Setting other Port Stuff for Arduino */
  tty.c_cflag     &=  ~PARENB;        	// Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;
  tty.c_cflag     &=  ~CRTSCTS;       	// no flow control
  tty.c_lflag     =   0;          	// no signaling chars, no echo, no canonical processing
  tty.c_oflag     =   0;                  // no remapping, no delays
  tty.c_cc[VMIN]      =   0;              // read doesn't block
  tty.c_cc[VTIME]     =   5;              // 0.5 seconds read timeout

  tty.c_cflag     |=  CREAD | CLOCAL;     	// turn on READ & ignore ctrl lines
  tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);	// turn off s/w flow ctrl
  tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
  tty.c_oflag     &=  ~OPOST;              	// make raw

  /* Flush Port, then applies attributes */
  tcflush( USB, TCIFLUSH );
  if ( tcsetattr ( USB, TCSANOW, &tty ) != 0){
    cout << "Error " << errno << " from tcsetattr" << endl;
  }  

rad_data object_left;
rad_data object_right;
infile>>patdat;			//reading patients first name
infile>>patdat1;		//reading patients last name

char fname[100];
strcpy(fname, "../Videos/Raw/");	//declaring path for storing video
strcat(fname, patdat);		//appending patients first name on the video
strcat(fname, " ");		//appending patients name on the video
strcat(fname, patdat1);		//appending patients last name on the video
strcat(fname,"_hough");
strcat(fname,".avi");	
//video_rec(fname);

VideoCapture capture(0);	//Opens the camera of the device connected  "../Videos/Raw/m n_raw.avi"
capture.set(CV_CAP_PROP_FPS,7);
capture>>image_RGB;			//Extract a frame and store in image matrix. 
int framewidth=image_RGB.cols; 		
int frameheight=image_RGB.rows;
char fname1[100];
strcpy(fname1, "../Videos/Processed/");	//declaring path for storing video
strcat(fname1, patdat);		//appending patients first name on the video
strcat(fname1, " ");		//appending patients name on the video
strcat(fname1, patdat1);		//appending patients last name on the video
strcat(fname1,"_hough");
strcat(fname1,".avi");	
int fps=10;
/*Define VideoiWriter object for storing the video*/
  VideoWriter video1(fname1,CV_FOURCC('M','J','P','G'),fps,cvSize(framewidth, frameheight));  //CV_FOURCC('M','J','P','G') is a motion-jpeg codec
boost::posix_time::ptime t1,t0;	//Time Variable for recording keeping track of time passed since execution of program


char fname2[100];
strcpy(fname2, "../Data/");	//declaring path for storing video
strcat(fname2, patdat);		//appending patients first name on the video
strcat(fname2, " ");		//appending patients name on the video
strcat(fname2, patdat1);		//appending patients last name on the video
strcat(fname2,"_radtime");
strcat(fname2,".csv");	

ofstream image_file(fname2); 	//csv file initialization. Data obtained from the image file written onto the csv file 

char fname3[100];
strcpy(fname3, "../Data/radtime.csv");	//declaring path for storing video
ofstream image_file_copy(fname3); 	//csv file initialization. Data obtained from the image file written onto the csv file 


//image_file<<"\t"<<"right eye"<<"\t"<<"left eye"<<"\n";
 n = write( USB,"s", sizeof (char) );	//Sends a serial variable 's' while hints the arduino program to start the blink code. This was "					done to sync with the arduino and giving the control of the arduino program in the cpp file here.
cout << "Returning Value " << n << endl;
t0 = boost::posix_time::microsec_clock::local_time();	//Getting absolute time at this point and storing in t0
while(1)
{
	//double t = (double)getTickCount();
	// t0 = boost::posix_time::microsec_clock::local_time();	//Getting absolute time at this point and storing in t0
	

	capture>>image_RGB;		//Extract a frame and store in image matrix. 
	
	Mat mask = Mat::zeros(image_RGB.size(),CV_8UC3);	//Makes a black mask
	ellipse(mask, Point( 0, 285 ), Size(220,120), 0, 0, 360, Scalar( 255, 255,255), -1, 8 );//mask_LEFT- ellipse on the left
	ellipse(mask, Point( 638,300), Size( 280,120 ), 0, 0, 360, Scalar( 255, 255, 255), -1, 8 );//mask_RIGHT- ellipse on the right
	Mat res= Mat::zeros(image_RGB.size(),CV_8UC3);   
	bitwise_and(image_RGB,mask,res);  /*Black with image gives black. White with image gives image. Only elliptical regions of the 								image are visible.*/
	//res=image_RGB.clone();
	//
	//circle_data obj;
	image_left=res(Rect(0,130,220,280));	//crop left eye 
	image_right=res(Rect(370,150,250,250));	//crop right eye
	Mat L = image_left.clone();
	Mat R = image_right.clone();
	//Calling function to detect hough circles
	//Mat roimat_left=Mat::zeros(roimat.size(),roimat.type());
	//roimat_left = Mat::zeros(Size(11,11),CV_8UC1);	
	//roimat_right = Mat::zeros(Size(11,11),CV_8UC1);	
	roimat_left = houghC(image_left);
	roimat_right = houghC(image_right);

	//Calculating the histogram
	//hist_left = Histo(roimat_left);
	//hist_right = Histo(roimat_right);

	//circles detected and bounding boxes drawn
	IL=roimat_left.clone();
	IR=roimat_right.clone();
	//Calling function to perform k means clustering.
	//Kleft = KMeans_function(IL,3);		
	//Kright = KMeans_function(IR,4);
	
	//Calling function to process the segmented image
	object_left  = postP(IL, 10);
	object_right = postP(IR, 10);
	//cout<<object_left.radiusF/5.9<<"     "<<object_right.radiusF/5.9<<endl;
	//image_file<<object_left.radiusF/5.9<<"\t"<<object_right.radiusF/5.9<<"\n";
	
	//int Frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
	//cout<<Frames<<endl;
	namedWindow("Video",WINDOW_NORMAL);		
        imshow("Video",res);
	moveWindow("Video", 500,0);
	namedWindow("Right eye",WINDOW_NORMAL);		
      imshow("Right eye",object_left.Cont_Image);
	moveWindow("Right eye", 0,0);
	namedWindow("Left eye",WINDOW_NORMAL);		
        imshow("Left eye",object_right.Cont_Image);
	moveWindow("Left eye", 1000,0);

	video1<<res;
	 t1=boost::posix_time::microsec_clock::local_time();	//Gives absolute time at this point and storing in t1
    boost::posix_time::time_duration diff = t1 - t0;	//Taking time difference between the time the code started and this point
	//video_left<<out_left;
	//video_right<<out_right;
	 n = read( USB,flag, sizeof flag );	//Reads the LED status to know which LED is ON using serial port
	cout << flag[0]<<"\n";			//Outputs the LED state on the terminal for debuggin
	/*insert data in CSV file depending upon the character sent in flag by arduino serially (depending on which LED is on) */
	if(object_left.radiusF>10 &&	object_right.radiusF>10 && object_left.radiusF<30 &&	object_right.radiusF<30) {


    if(flag[0]=='0'){	//No LEDs are blinking
  	  //insert the time and radius of pupil in the csv file- 
   	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<0<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='1'){ //Stimulus 1
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='2'){	//Stimulus 2
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
     }
     else if(flag[0]=='3'){ //Stimulus 3
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='4'){	//Stimulus 4
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
     }
     else if(flag[0]=='5'){	//Stimulus 5
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n";
       image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n";  
     }
     else if(flag[0]=='6'){	//Stimulus 6
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
     }
     else if(flag[0]=='7'){	//Stimulus 7
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n";
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<5<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='8'){	//Stimulus 8
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<5<<"\n"; 
     }
     else{			//No LEDs are ON
	image_file<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<0<<"\n"; 
	image_file_copy<<diff.total_milliseconds()<<"\t"<<object_right.radiusF/5.9<<"\t"<<object_left.radiusF/5.9<<"\t"<<0<<"\t"<<0<<"\n"; 
     }

	}
	//namedWindow("window right",WINDOW_NORMAL);		
        //imshow("window right",roimat2); 
	key = waitKey(100); 	//Capture Keyboard stroke
    	if (char(key) == 27)
	{
	        break; 		//If you hit ESC key loop will break and code will terminate

	}
}

image_file.close();
image_file_copy.close();

system("gnuplot ../plot.ps");	//run the plot.ps postscript for generating the plot from the csv
system("sh ../report.sh");	//run the report.sh shell script which forms the report using the data generated above







}
