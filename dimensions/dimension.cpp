/*
 * AUTHOR : ANZO Controls Pvt Ltd
 *
 * This document is strictly confidential communication to and solely for the use
 * of the recipient and may not be reproduced or circulated without Anzo Controls's
 * Private Limited's prior written consent. If you are not the intended recipient,
 * you should not disclose or use the information in this document in any way.
 * The information is not intended as an offer or solicitation with respect to the
 * usage of this code.
 *
 * Strict legal proceeding would happen in case anyone has been found in violation
 * of the notice as above.
 */

#include"dimension.h"
#include"calculation.h"

double largest_area=0;
int largest_contour_index,i=0;

bool Dimensions::isFindDimensions(Measurement* Measurement,std::vector<std::vector<cv::Point>>& contours,cv::Mat& src){
  std::vector<std::vector<cv::Point>>  contours_poly(contours.size());
  if(contours.size()){
    for( int i = 0; i< contours.size(); i++ ) {
      //Find the largest area of contour, and the bounding rect for the largest contour
      double a=cv::contourArea( contours[i],true);
      if(a>largest_area) {
        largest_area=a;
        largest_contour_index=i;          
      }
    }
    if(contours[largest_contour_index].size() < 4){
      //std::cout<<contours[largest_contour_index]<<"\n\n";
      return false;
    }
    // Performing approximation method on detected contours 
    cv::approxPolyDP( cv::Mat(contours[largest_contour_index]), contours_poly[largest_contour_index],20, true );
    // Sort the vector points in ascending order
    std::vector<cv::Point> points=sort(contours_poly[largest_contour_index]);
    std::vector<cv::Point> corners[4];
    //length=height=width=breadth=0;
    if(points.size() == 4){
      for(i=0;i<4;i++){
        if(!validatePoints(points[i])){
	  return false;
        }
      }
      cv::Point mt,mb,ml,mr;
      mt=midPoint(points[0],points[2]);
      mb=midPoint(points[3],points[1]);
      mr=midPoint(points[3],points[2]);
      ml=midPoint(points[1],points[0]);
      
      setPixelLength(distance(mt,mb));////length=distance(mt,mb);
      setPixelWidth(distance(mr,ml));//breadth=distance(mr,ml);

      setLength(getPixelLength()/Measurement->pixelsPerMetric->getPixelPerCm());
      setWidth(getPixelWidth()/Measurement->pixelsPerMetric->getPixelPerCm());
      if((getLength() > 9.0 ) && (getWidth() > 9.0)){
	if(Measurement->isOutputShowOnConsole()){
	  std::cout<<"\n\nEucledian_Length="<<getLength()<<"\nEucledian_Width="<<getWidth()<<"\n\n";
	}
	if(Measurement->isImageShowOnConsole()){
	  std::string text;	
	  text="length=" + std::to_string(getLength());
          putText(src,text,cv::Point(ml.x-10, mt.y+30),cv::FONT_HERSHEY_SIMPLEX, 1,(255, 0, 0),1);
          text="width=" + std::to_string(getWidth());
          putText(src,text,cv::Point(ml.x-10,ml.y),cv::FONT_HERSHEY_SIMPLEX, 1,(255,0,0),1);
        
	  /*putText(src,"mt",mt,cv::FONT_HERSHEY_SIMPLEX, 1,cv::Scalar(255, 0, 0), 1);
          putText(src,"ml",ml,cv::FONT_HERSHEY_SIMPLEX, 1,cv:: Scalar(255, 0, 0),1);
          putText(src,"mr",mr,cv::FONT_HERSHEY_SIMPLEX, 1,cv::Scalar(255, 0, 0),1);
          putText(src,"mb",mb,cv::FONT_HERSHEY_SIMPLEX, 1,cv:: Scalar(255, 0, 0),1);*/
	  cv::imshow( "source_window", src );
          cv::waitKey(1);
	}
        //destroyAllWindows();}
	return true;
     }
    return false;
  }
  }
return false;
}

/*
  Function       : setLength
  Accepts        : double
  Returns        : None
  Abstraction    : Take double length as argument and sets
                   length attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
void  Dimensions::setLength(double value){
	length = value;
}

/*
  Function       : setWidth
  Accepts        : double
  Returns        : None
  Abstraction    : Take double width as argument and sets
                   width attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
void  Dimensions::setWidth(double value){
        width = value;
}

/*
  Function       : setPixelLengt
  Accepts        : double
  Returns        : None
  Abstraction    : Take double pixelLength as argument and sets
                   pixelLength attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
void  Dimensions::setPixelLength(double value){
        pixelLength = value;
}


/*
  Function       : setPixelWidth
  Accepts        : double
  Returns        : None
  Abstraction    : Take double pixelWidth as argument and sets
                   pixelWidth attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
void  Dimensions::setPixelWidth(double value){
        pixelWidth = value;
}

/*
  Function       : getLength
  Accepts        : None
  Returns        : double length
  Abstraction    : Returns length attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
double  Dimensions::getLength(){
  return length;
}

/*
  Function       : getWidth
  Accepts        : None
  Returns        : double width
  Abstraction    : Returns width attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
double  Dimensions::getWidth(){
  return width;
}

/*
  Function       : getPixelLength
  Accepts        : None
  Returns        : double pixelLength
  Abstraction    : Returns pixelLength attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
double Dimensions::getPixelLength(){
  return pixelLength;
}

/*
  Function       : getPixelWidth
  Accepts        : None
  Returns        : double pixelWidth
  Abstraction    : Returns pixelWidth attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
double  Dimensions::getPixelWidth(){
  return pixelWidth;
}

/*
  Function       : sort
  Accepts        : std::vector<cv::Point>
  Returns        : std::vector<cv::Point>
  Abstraction    : Sort the vector points from ascending order of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
std::vector<cv::Point> Dimensions::sort(std::vector<cv::Point> points){
  std::vector<cv::Point> newpoints;
  std::vector<int> sump,subp;
  for(int i=0;i<4;i++) {
     sump.push_back(points[i].x+points[i].y);
     subp.push_back(points[i].x-points[i].y);
  }
  newpoints.push_back(points[std::min_element(sump.begin(),sump.end())-sump.begin()]);//0

  newpoints.push_back(points[std::min_element(subp.begin(),subp.end())-subp.begin()]);//1
  newpoints.push_back(points[std::max_element(subp.begin(),subp.end())-subp.begin()]);//2
  newpoints.push_back(points[std::max_element(sump.begin(),sump.end())-sump.begin()]);//3
  return newpoints;
}

/*
  Function       : midPoint
  Accepts        : const cv::Point&,const cv::Point&
  Returns        : cv::Point
  Abstraction    : Finds the mid points of two points
  Modified by    : 10-03-22 - Srikanth
*/
cv::Point Dimensions::midPoint(const cv::Point& a, const cv::Point& b) {
  cv::Point ret;
  ret.x = (a.x + b.x) / 2;
  ret.y = (a.y + b.y) / 2;
  return ret;
}

/*
  Function       : midPoint
  Accepts        : const cv::Point2f,const cv::Point2f
  Returns        : double
  Abstraction    : Finds the Euclidien distance of two points 
  Modified by    : 10-03-22 - Srikanth
*/
double Dimensions::distance(const cv::Point2f &a, const cv::Point2f &b)
{
  double xDiff = a.x - b.x;
  double yDiff = a.y - b.y;
  return std::sqrt((xDiff * xDiff) + (yDiff * yDiff));
}

/*
  Function       : validatePoints
  Accepts        : cv::Point
  Returns        : bool
  Abstraction    : Validate the points with in the range
  Modified by    : 10-03-22 - Srikanth
*/
bool Dimensions::validatePoints(cv::Point p){
  if(p.x < 1 || p.x > 650){
    return false;
  }else if(p.y < 1 || p.y > 650){
    return false;
  }else{
    return true;
  }
}


