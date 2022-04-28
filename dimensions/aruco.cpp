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

#include "aruco.h"

/*
  Function       : isfindArucoMarker
  Accepts        : cv::Mat&
  Returns        : bool
  Abstraction    : Take cv::Mat& as argument and finds aruco marker
                   and also determines pixels per centimeter
  Modified by    : 10-03-22 - Srikanth
*/
bool Aruco::isfindArucoMarker(cv::Mat& image){
  cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
  //cv::Mat cameraMatrix, distCoeffs;
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_100);
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners;
  cv::aruco::detectMarkers(image, dictionary, corners,ids);
  
  //vector<vector<Point>> contours1;
  setPixelPerCm(0);
  if(ids.size()==0) {
   return false;
  }
  static int len;
  if(corners[0].size() == 4){
    len=arcLength(corners[0], true);
    setPixelPerCm(len/20);
    return true;
  }

 return false;
}

/*
  Function       : setPixelPerCm
  Accepts        : double
  Returns        : None
  Abstraction    : Take double PixelPerCm as argument and sets
                   PixelPerCm attribute of Dimensions class
  Modified by    : 10-03-22 - Srikanth
*/
void Aruco::setPixelPerCm(int value){
  PixelPerCm = value;
}

/*
  Function       : getPixelPerCm
  Accepts        : None
  Returns        : int
  Abstraction    : This function returns PixelPerCm of Aruco class attribute
  Modified by    : 10-03-22 - Srikanth
*/

int Aruco::getPixelPerCm(){
  return PixelPerCm;
}
