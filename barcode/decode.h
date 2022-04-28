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
#include <opencv2/core.hpp>

/*
   This is needed to avoid the inclusion of this file multiple times
   during compiling process
   This serves as same purpose as:
   #ifndef SOME_HEADER_FILE_H
   #define SOME_HEADER_FILE_H
     your code
   #endif
*/
#pragma once

enum orientation {
  goingSouth = 0,
  goingEast,
  goingNorth,
  goingWest
};

// Forward declaration
class Barcode;

/*
  Class name        : Decode
  Private members   : currentData,lastData,decodedCornersi,referenceBarcodeData
                      orient,globalData, qrcodeData,
  Public methods    : setDecodedData(std::string decodedData),getCurrentDecodedData(),
                      getLastDecodedData(),getDecodedCorners(),setOrientation(enum orientation),
                      printFrameResults(std::string& data, int& angle),setGlobalData(std::string),
                      performOpenCVDecode(cv::Mat& frame),setQrcodeData(std::string data),
                      performZBarDecode(cv::Mat& frame),getQrcodeData(),getOrientation(),
                      setDecodedCorners(std::vector<cv::Point>& decodedCorners),getGlobalData(),
  Abstraction       : This classs uses detectAndDecodeFrame function which
                      provides decoded data and corners of qr codei
  Modified by       : 13-01-22 - Srikanth added referenceBarcodeData to store position value
                      and decodedData to handle decoded data while iterating
                      22-01-22 - Srikanth added orient attribute to store direction of agv
                      22-01-22 - Srikanth added glbalData attribute to store qrcode decoded data
                      which is used as reference still agv is reached new qrcode
                      22-01-22 - Srikanth addede qrcodeData attribute which keeps storing one qrcode
                      for each from irrespective of number of qrcodes in single frame.
*/
class Decode {
  std::string currentData;
  std::string lastData;
  std::string referenceBarcodeData;
  std::string lastReferenceBarcodeData;
  std::vector<cv::Point>  finalDecodedCorners;
  enum orientation orient;
  std::string globalData;
  std::string qrcodeData;

  public:
    void performOpenCVDecode(cv::Mat& frame);
    bool performZBarDecode(cv::Mat& frame, Barcode* agv);
    void printFrameResults(std::string& data, int& angle);
    // Setters
    void setOrientation(enum orientation);
    void setGlobalData(std::string);
    void setQrcodeData(std::string data);
    void setDecodedData(std::string decodedData);
    void setDecodedCorners(std::vector<cv::Point>& decodedCorners);
    void setReferenceBarcodeData(std::string data);
    // Getters
    std::string getQrcodeData();
    enum orientation getOrientation();
    std::string getGlobalData();
    std::string getCurrentDecodedData();
    std::string getLastDecodedData();
    std::vector<cv::Point> getDecodedCorners();
    std::string getReferenceBarcodeData();
    std::string getLastReferenceBarcodeData();
};
