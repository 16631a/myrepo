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

#include<iostream>
#include<iostream>
#include"barcode.h"
#include<cmath>
#include"headers.h"

/*
  Function    : findAngle
  Accepts     : Barcode*
  Returns     : None
  Abstraction : Take Barcode* pointer object as an parameter using Barcode class members it
                find the angle at which the photo was taken and logs the angle in data.log file
  Modified by : 12-10-21 - Saksham
*/
void Angle::findAngle(Barcode* barcodeInfo) {
  // After coming here, we are certain that we have valid
  // decoded corners
  std::vector<cv::Point> decodedCorners =
                         barcodeInfo->decodedInfo->getDecodedCorners();
  if(barcodeInfo->getDebugLevel() > 1) {
    if (decodedCorners.size() != 4) {
      barcodeInfo->logText("Can not get the right set of decoded corners\n");
      setAngle(0);

      return;
    }
  }

  // Let's sort the corner points based on first parameter
  std::sort(decodedCorners.begin(), decodedCorners.end(),
    [](const cv::Point& left, const cv::Point& right) {
    return left.x < right.x;
  });

  cv::Point first  = decodedCorners[0];
  cv::Point second = decodedCorners[1];

  int degrees = 0;
  std::string direction;

  std::string data;
  if(second.x != first.x) {
    double slope    = (second.y - first.y)/(second.x - first.x);
    double raidians = atan2((second.y - first.y),(second.x - first.x));
    degrees         = (raidians * (180/PI));
    if (degrees > 0) {
      degrees = 90 - degrees;
      direction = "Anti-clockwise";
      setDirection(false);

    } else {
      degrees = 90 + degrees;
      direction = "clockwise";
      setDirection(true);
    } 
   }
   data  = std::to_string(degrees);
   setAngle(degrees);

  // Logging the direction with value

  if (barcodeInfo->getDebugLevel() > 1) {
    barcodeInfo->logText(direction);
    barcodeInfo->logText(data);
  }
}

/*
  Function    : setAngle
  Accepts     : int
  Returns     : None
  Abstraction : Take float degrees as an parameter and 
                updates the last angle & current angle
  Modified by : 12-10-21 - Srikanth
                10-11-21 - Srikanth added the inline keyword
                to reduce the function call overhead
*/
inline void Angle::setAngle(int degrees) {
  lastAngle    = currentAngle;
  currentAngle = degrees;
}

/*
  Function    : getCurrentAngle
  Accepts     : None
  Returns     : int
  Abstraction : Returns current angle in degrees 
  Modified by : 12-10-21 - Srikanth
                10-11-21 - Srikanth added the inline keyword to reduce the
                function call overhead
                04-01-22 - Srikanth removed inline keyword to resolve
                compiler dependency problems
*/ 
int Angle::getCurrentAngle() {
  return currentAngle;
}

/*
  Function    : getLastAngle
  Accepts     : None
  Returns     : int
  Abstraction : Returns last angle in degrees
  Modified by : 12-10-21 - Srikanth
                10-11-21 - Srikanth added the inline keyword
                04-01-22 - Srikanth removed inline keyword to resolve
                compiler dependency problems
*/
inline int Angle::getLastAngle() {
  return lastAngle;
}

/*
  Function    : setDirection
  Accepts     : bool
  Returns     : None
  Abstraction : Take bool direction as an parameter and updates the last angle & current angle
  Modified by : 30-11-21 - Srikanth
                30-11-21 - Srikanth added the inline keyword to reduce the
                function call overhead
*/
inline void Angle::setDirection(bool direction) {
  lastDirection    = currentDirection;
  currentDirection = direction;
}

/*
  Function    : getCurrentDirection
  Accepts     : None
  Returns     : bool
  Abstraction : Returns current direction in bool
  Modified by : 30-11-21 - Saksham
*/
bool Angle::getCurrentDirection() {
  return currentDirection;
}

/*
  Function    : getLastDirection
  Accepts     : None
  Returns     : bool
  Abstraction : Returns last direction in bool
  Modified by : 30-11-21 - Saksham
*/
bool Angle::getLastDirection() {
  return lastDirection;
}

