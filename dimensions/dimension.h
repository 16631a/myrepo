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

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#pragma once
class Measurement;

/*
  Class name        : Dimensions
  Private members   : length.width,pixelLength,pixelWidth
  Public methods    : isFindDimensions(Measurement*,std::vector<std::vector<cv::Point>>&,cv::Mat&),
                      distance(const cv::Point2f &,const cv::Point2f &),sort(std::vector<cv::Point>),
		      midPoint(const cv::Point &,const cv::Point &),validatePoints(cv::Point p)
  Getters           : getLength(),getWidth(),getPixelLength(),getPixelWidth(),
  Setters           : setPixelLength(double),setPixelLength(double),setLength(double),setPixelWidth(double),setWidth(double)
  Modified by       : 11-03-22 - Srikanth
*/
class Dimensions{
  private:
    double length,width;
    double pixelLength,pixelWidth;
  public:	 
    bool isFindDimensions(Measurement*,std::vector<std::vector<cv::Point>>& contours,cv::Mat& src);
    double distance(const cv::Point2f &a, const cv::Point2f &b);
    cv::Point midPoint(const cv::Point& a, const cv::Point& b);
    std::vector<cv::Point> sort(std::vector<cv::Point> points);
    void setLength(double);
    void setWidth(double);
    bool validatePoints(cv::Point p);
    double getLength();
    double getWidth();
    void setPixelLength(double);
    void setPixelWidth(double);
    double getPixelWidth();
    double getPixelLength(); 
};
