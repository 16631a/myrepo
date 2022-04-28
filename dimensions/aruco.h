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

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/aruco.hpp>

/*
  Class name        : Aruco
  Private members   : PixelPerCm
  Public methods    : findPixelPerCm(cv::Mat&)
  Setters           : setPixelPerCm(int)
  Getters           : getPixelPerCm()
  Modified by       : 11-03-22 - Srikanth
*/
class Aruco{
  private:
    int PixelPerCm;
  public:
    bool isfindArucoMarker(cv::Mat& image);
    void setPixelPerCm(int);
    int getPixelPerCm();
    void findPixelPerCm();
};
