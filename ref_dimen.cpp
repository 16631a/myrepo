#include <opencv2/aruco.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;

int pixel_cm_ratio,largest_area=0,largest_contour_index;;

Mat src,src_gray,edge,dil,thresh;

/*********************fun to find pixels per/cm ratio****************************/
bool findPixelPerCm(Mat image){
cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
cv::Mat cameraMatrix, distCoeffs;
cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_100);
    cv::Mat imageCopy;
    image.copyTo(imageCopy);
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners;
    cv::aruco::detectMarkers(image, dictionary, corners, ids);
    vector<vector<Point>> contours1;
    if (ids.size()==0) {
       return false;
    }
    int len;
    if(corners[0].size() == 4){
      len=arcLength(corners[0], true);
      pixel_cm_ratio = len / 20;
      return true;
    }
    return false;   
}

/*************************sorting points from left to right*********************/
vector<Point> reorder(vector<Point> points){
  vector<Point> newpoints;
  vector<int> sump,subp;
  for(int i=0;i<4;i++) {
     sump.push_back(points[i].x+points[i].y);
     subp.push_back(points[i].x-points[i].y);
  }
  newpoints.push_back(points[min_element(sump.begin(),sump.end())-sump.begin()]);//0

  newpoints.push_back(points[min_element(subp.begin(),subp.end())-subp.begin()]);//1
  newpoints.push_back(points[max_element(subp.begin(),subp.end())-subp.begin()]);//2
  newpoints.push_back(points[max_element(sump.begin(),sump.end())-sump.begin()]);//3
  return newpoints;
}
/****************************************************/

int main( int, char** argv )
{
	cout<<"height=="<<"height"<<"\nwidth=="<<"width"<<"\n";
	cout<<"hi\n"<<largest_area;
cv::VideoCapture cap("/dev/v4l/by-id/usb-e-con_systems_See3CAM_20CUG_0E0B9504-video-index0");
cap.set(cv::CAP_PROP_CONVERT_RGB, false);
cap.set(cv::CAP_PROP_EXPOSURE, 150);
cout<<"iam while \n";
while(1){
  cap >> src;
  cout<<"iam whilevxsgdhxnwayw \n";
  if(src.empty()) {
        cout<<"It seems like We have encountered a mechanical failure. \
We would be unable to proceed. Please contact Anzo for next steps. You can also \
try checking the camera connections at your own condition.\n";
        break;
  }

  cv::convertScaleAbs(src,src, 0.2490234375);
  //cvtColor(src,src,COLOR_BGR2GRAY);
  //imshow("original", src );
  cout<<"iam \n";
  //waitKey(1);
  if(!findPixelPerCm(src)){
    //cout<<"aruco marker not detected\n";
    continue;
  }
  //cout<<"aruco marker detected\n";
  GaussianBlur(src,src_gray,Size(5,5),0);
  //threshold(src,src, 80, 255,THRESH_TOZERO);
  //threshold(src,src_gray,80,255, THRESH_BINARY);
  //threshold(src_gray,src_gray,80,255, THRESH_BINARY);
  //threshold(src,src_gray,80,255, THRESH_BINARY);
  imshow( "src_gray", src_gray );
  vector<vector<Point>> contours;
  
  vector<Vec4i> hierarchy;
  // finding the edges of the object
  Canny( src_gray, edge,80,240);
  //perform edge detection, then perform a dilation + erosion to
  //close gaps in between object edges
  dilate(edge,dil, 5);
  erode(dil,edge,5);
  imshow( "edge", edge );
 // waitKey(1);
//**********************  
  findContours(edge,contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  vector<vector<Point>>  contours_poly(contours.size());
//********
  if (contours.size()){
     for( int i = 0; i< contours.size(); i++ ) {
       //  Find the largest area of contour, and the bounding rect for the largest contour
       double a=contourArea( contours[i],true);
       if(a>largest_area) {
         largest_area=a;
         largest_contour_index=i;          
       }
     }
    if(contours[largest_contour_index].size() < 4)
      continue;
    approxPolyDP( cv::Mat(contours[largest_contour_index]), contours_poly[largest_contour_index],20, true );
     vector<Point> points=reorder(contours_poly[largest_contour_index]);
     //cout<<points.size()<<"size\n";
     if(points.size() == 4){
     	Point tl,tr,bl,br;
        tl=points[0];
        bl=points[1];
        tr=points[2];
        br=points[3];
        float dh = br.y-tl.y;
        float dw = tr.x-tl.x;
	float height=dh/pixel_cm_ratio;
        float width=dw/pixel_cm_ratio;
        std::string text="height=" + std::to_string(height);
        putText(src,text,cv::Point(tl.x, tl.y-15),FONT_HERSHEY_SIMPLEX, 1,CV_RGB(0, 255, 0),1);
        //cout<<"\nheight="<<text;
        text="width=" + std::to_string(width);
         //cout<<"\nwidth="<<text;i
	if((height > 7 ) && (width > 7)){
 	  putText(src,text,cv::Point(br.x+15, br.y),FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0,255,0),1);
          putText(src,"bl",bl,FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0));
          putText(src,"br",br,FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0));
          putText(src,"tr",tr,FONT_HERSHEY_SIMPLEX, 1,(255, 255, 0));
          putText(src,"tl",tl,FONT_HERSHEY_SIMPLEX, 1, (255, 0, 255));
	  cout<<"height=="<<height<<"\nwidth=="<<width<<"\n";
          line(src, tl,bl,CV_RGB(255, 0, 255), 1);line(src, tr,br,CV_RGB(255, 0, 255), 1);
	  line(src, tl,tr,CV_RGB(255, 0, 255), 1);line(src, br,bl,CV_RGB(255, 0, 255), 1);
          imshow( "source_window", src );
          imshow( "edge", dil );
          waitKey(1);
          //destroyAllWindows();}
     }}
  }
//waitKey(1);
}
cap.release();
return 0;
}
