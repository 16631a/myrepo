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
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include"barcode.h"
#include <zbar.h> 
#include "headers.h"

/*
  Function    : detectAndDecodeFrame
  Accepts     : cv::Mat&, std::vector<cv::Point>& 
  Returns     : None
  Abstraction : Take cv::Mat& image as an parameter and 
                decodes the information embeddded in it as well as
                detects the corners
  Modified by : 12-10-21 - Srikanth
*/
void Decode::performOpenCVDecode(cv::Mat& frame) {
  // TODO: What points are they?
  std::vector<cv::Point> points;
  cv::Mat thresholdImage, grayscaleImage;

  // Creating an grayscale image
  cv::cvtColor(frame, grayscaleImage, cv::COLOR_BGR2GRAY);
  // Setting an threshold property on grayscale image
  // This is expected to make black pixels more black
  // & white pixels more white.
  cv::threshold(grayscaleImage, thresholdImage, THRESHOLD_LOWER_LIMIT,
      THRESHOLD_UPPER_LIMIT, cv::THRESH_BINARY);

  // Creating an constructor/object of QRCodeDetector
  cv::QRCodeDetector qrDecoder;

  // Reads the barcode/data
  std::vector<cv::Point> decodedCorners;
  setDecodedData(qrDecoder.detectAndDecode(thresholdImage, decodedCorners));
  setDecodedCorners(decodedCorners);
}

/*
  Function    : printFrameResults
  Accepts     : std::string& , int& 
  Returns     : None
  Abstraction : Take std::string& data and int& angle as an parameter and
                 prints data along with angle on console
  Modified by : 12-10-21 - Srikanth
*/
void printFrameResults(std::string& data, int& angle) {
  std::cout << "Decoded data = " << data << " along with angle as "
            << angle << std::endl;
}

/*
 Function    : performZBarDecode
 Accepts     : cv::Mat& ,  Barcode*
 Returns     : void
 Abstraction : Decodes the data from QR code and log the data in
                data log file in case of debug level given
 Modified by : 20-10-21 - Hemant Bhargava
               12-10-22 - Srikanth setting the angle to zero in case barcode is not detected
               13-01-22 - Srikanth modified the performZbarDecode function to handle two qr codes
               22-01-22 - Srikanth modified the code to find orentation of qrcodes and dissabled the function
               related to find angle since motion control code is not depending on angle.
*/
bool Decode::performZBarDecode(cv::Mat& frame, Barcode* barcodeInfo) {
  // Create zbar scanner
  zbar::ImageScanner scanner;
  // Disable all
  scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 0);
  // Enable only QR scanning
  scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);
  // Convert image to grayscale
  // cv::Mat grayscaleImage;
  //cv::cvtColor(frame, grayscaleImage, cv::COLOR_BGR2GRAY);

  // Wrap image data in a zbar image
  zbar::Image image(frame.cols, frame.rows, "Y800",
                   (uchar *)frame.data,
  		   frame.cols * frame.rows);
  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);
  // results should be exactly 1, we do not expect more than 1 barcode
  // in single frame
  zbar::SymbolSet symbolSet = scanner.get_results();
  if(symbolSet.get_size() == 0 || symbolSet.get_size() > 2) {
    if(barcodeInfo->getDebugLevel() > 0){
      if(symbolSet.get_size() == 0) {
        barcodeInfo->logText("No data received from the frame");
      } else {
        barcodeInfo->logText("Received the data for more than 1 frame");
      } 
    }
    setDecodedData(std::string());
    setReferenceBarcodeData(std::string());
#ifdef FIND_ANGLE
    // barcodeInfo->angle->setAngle(0);
#endif
    setQrcodeData(std::string());
    return false;
  }

  // Note: Instead of using an for loop to iterate, since we know that only
  // one result is present when we come here, initializing
  // zbar::Image::SymbolIterator to image.symbol_begin() should be fine.
  for(zbar::Image::SymbolIterator symbol = image.symbol_begin();
                                  symbol != image.symbol_end();
                                  ++symbol) {
    // zbar::Image::SymbolIterator symbol = image.symbol_begin();
#ifdef FIND_ANGLE
    // corners collection
    std::vector<cv::Point> corners;
    // Show error message that we have not got all the four corners
    // of the image
    if(symbol->get_location_size() != 4) {
       std::string message = "Got only " + std::to_string(symbol->get_location_size()) +
                              " corners instead of 4 corners required to calculate angle\n";
       barcodeInfo->logText(message);
       setDecodedCorners(corners);
       return true;
    }

    // Pushing the x,y locations of the barcode
    for(int i = 0; i < symbol->get_location_size(); i++) {
    // std::cout << "x loc = " << symbol->get_location_x(i) << ", y loc = " << symbol->get_location_y(i) << std::endl;
     corners.push_back(cv::Point(symbol->get_location_x(i),symbol->get_location_y(i)));
     }
     // Set decoded corners
     setDecodedCorners(corners);
#endif

    // Writing the data to the temporary attribute
    setQrcodeData(symbol->get_data().data());

    // Finding the direction in which qr codes are read
    if(symbol->get_orientation() == 0) {
      setOrientation(goingSouth);
    } else if(symbol->get_orientation() == 1) {
      setOrientation(goingEast);
    } else if(symbol->get_orientation() == 2) {
      setOrientation(goingNorth);
    } else if(symbol->get_orientation() == 3) {
      setOrientation(goingWest);
    }

    // Erase the new line character from the end
    if (!getQrcodeData().empty() &&
         (getQrcodeData()[getQrcodeData().size()-1] == '\n') ||
	  getQrcodeData()[getQrcodeData().size()-1] == '\r') {
       setQrcodeData(getQrcodeData().erase(getQrcodeData().length()-1));
    }

    // Check if the data decoded is any of the positioned data
    bool isR = getQrcodeData().at(0) == 'R';
    bool isL = getQrcodeData().at(0) == 'L';
    bool isD = getQrcodeData().at(0) == 'D';
    bool isU = getQrcodeData().at(0) == 'U';
    // Storing the data decoded from qrcodes
    if(!isR && !isL && !isD && !isU) {
      setDecodedData(getQrcodeData());
    } else {
      setReferenceBarcodeData(getQrcodeData());
    }
  }

  return true;
}


/*
  Function     : setDecodedData
  Accepts      : std::string
  Returns      : None
  Abstraction  : Take std::string decodedData as parameter and 
                 updates currentDecodedData and lastDecodedData every time
  Modified by : 12-10-21 - Srikanth
                10-11-21 - Srikanth added the inline keyword
                04-01-22 - Srikanth removed inline keyword to resolve
                compiler dependency problems
*/
void Decode::setDecodedData(std::string decodedData) {
  if(decodedData.size()) {
    lastData    = currentData;
  }
  currentData = decodedData;
}

/*
  Function    : getCurrentDecodedData
  Accepts     : None
  Returns     : std::string currentData
  Abstraction : Takes nothing and returns Decode class currentData attribute
  Modified by : 12-10-21 - Srikanth
*/
std::string Decode::getCurrentDecodedData(){
  return currentData;
}

/*
  Function    : getLastDecodedData
  Accepts     : None
  Returns     : std::string lastData
  Abstraction : Takes nothing and returns Decode class lastData attribute
  Modified by : 12-10-21 - Srikanth
*/
std::string Decode::getLastDecodedData(){
  return lastData;
}

/*
  Function       : setDecodedCorners
  Accepts        : std::vector<cv::Point>&
  Returns        : None
  Abstraction    : Take decoded corners  and updates the 
                   finalDecodedCorners attribute of Decode class
  Modified by    : 12-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword
                   04-01-22 - Srikanth removed inline keyword to resolve
                   compiler dependency problems
*/
void Decode::setDecodedCorners(std::vector<cv::Point>& decodedCorners){
  finalDecodedCorners=decodedCorners;
}

/*
  Function       : setReferenceBarcodeData
  Accepts        : std::string
  Returns        : None
  Abstraction    : Takes string data as input and sets the Decode class referenceBarcodeData attribute
  Modified by    : 13-01-22 - Srikanth
*/
void Decode::setReferenceBarcodeData(std::string data){
  if(data.size()) {
    lastReferenceBarcodeData = referenceBarcodeData;
  }
  referenceBarcodeData = data;
}

/*
  Function       : setOrientation
  Accepts        : std::string
  Returns        : None
  Abstraction    : Takes string data as input and sets the Decode clas orient attribute
  Modified by    : 22-01-22 - Srikanth
*/
void Decode::setOrientation(enum orientation direction){
	orient = direction;
}

/*
  Function       : setQrcodeData
  Accepts        : std::string
  Returns        : None
  Abstraction    : Takes string data as input and sets the Decode class qrcodeData attribute
  Modified by    : 22-01-22 - Srikanth
*/
void Decode::setQrcodeData(std::string data){
	qrcodeData = data;
}

/*
  Function       : setGlobalData
  Accepts        : std::string
  Returns        : None
  Abstraction    : Takes string data as input and sets the Decode class globalData attribute
  Modified by    : 22-01-22 - Srikanth
*/
void Decode::setGlobalData(std::string data){
	globalData = data;
}

/*
  Function       : getOrientation
  Accepts        : None
  Returns        : enum orientation
  Abstraction    : Takes nothing and returns Decode class orient attribute
  Modified by    : 22-01-22 - Srikanth
*/
enum orientation Decode::getOrientation(){
	return orient;
}

/*
  Function       : getQrcodeData
  Accepts        : None
  Returns        : std::string
  Abstraction    : Takes nothing and returns Decode class qrcodeData attribute
  Modified by    : 22-01-22 - Srikanth
*/
std::string Decode::getQrcodeData(){
	return qrcodeData;
}

/*
  Function       : getGlobalData
  Accepts        : None
  Returns        : std::string
  Abstraction    : Takes nothing and returns Decode class globalData attribute
  Modified by    : 22-01-22 - Srikanth
*/
std::string Decode::getGlobalData(){
	return globalData;
}

/*
  Function       : getDecodedCorners
  Accepts        : None
  Returns        : std::vector<cv::Point>
  Abstraction    : Takes nothing and returns Decode class finalDecodedCorners attribute
  Modified by    : 12-10-21 - Srikanth
*/
std::vector<cv::Point> Decode::getDecodedCorners(){
  return finalDecodedCorners;
} 

/*
  Function       : getReferenceBarcodeData
  Accepts        : None
  Returns        : std::string
  Abstraction    : Returns the Decode class referenceBarcodeData attribute
  Modified by    : 13-01-22 - Srikanth
*/
std::string Decode::getReferenceBarcodeData(){
  return referenceBarcodeData;
}
/*
  Function       : getLastReferenceBarcodeData
  Accepts        : None
  Returns        : std::string
  Abstraction    : Returns the Decode class referenceBarcodeData attribute
  Modified by    : 13-01-22 - Srikanth
*/
std::string Decode::getLastReferenceBarcodeData(){
  return lastReferenceBarcodeData;
}
