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
#include <iostream>
#include <sys/stat.h>
#include"dimension.h"
#include"aruco.h"
#include"Text.h"
#pragma once
#include"../barcode/barcode.h"

// Forward diclaration of friend class Barcode 
class Barcode;
void find_squares(cv::Mat& image);
typedef std::chrono::high_resolution_clock::time_point cpuTime;

/*
  Class name       : Measurement
  Private members  : debugLevel,logFile
  Public members   : pixelsPerMetric,dimensions,messageText
  Public methods   : processArgs(int,char**),start(),logText(),logTime(),
  Getters          : getDebugLevel(),getTargetColumn(),getLogFile(),
                     bool isOutputShowOnConsole(),bool isImageShowOnConsole()
  Setters          : setDebugLevel(int),openLogFile(std::string),
                     void setOutPutConsoleValue(bool),void setImageShowValue(bool)

  Constructors     : Measurement()

  Desructor        : ~Measurement()
  Modified by      : 11-03-22 - Srikanth
*/
class Measurement {
  private:
    int debugLevel;
    std::ofstream* logFile;
    bool outPutConsoleValue,imageShowValue;
  public:
    friend class Barcode;
    Aruco *pixelsPerMetric;
    Dimensions *dimensions;
    TextMessage *messageText;
    
    int getDebugLevel();
    std::ofstream* getLogFile();
    bool isOutputShowOnConsole();
    bool isImageShowOnConsole();

    void setOutPutConsoleValue(bool);
    void setImageShowValue(bool);
    void setDebugLevel(int);
    void openLogFile(std::string filePathName);

    void dimension(int argc, char **argv,Barcode*);
    int start();
    void logText(std::string data);
    void logTime(cpuTime time);
    bool processArgs(int argc, char** argv);

    Measurement();
    ~Measurement();
};
