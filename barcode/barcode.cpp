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

#include <opencv2/videoio.hpp>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "headers.h"
#include "barcode.h"
#include <sys/stat.h>
#include <opencv2/highgui.hpp>

// Static functions
static inline bool fileExists(const std::string& name);

/*
  Function     : start
  Accepts      : None
  Returns      : int
  Abstraction  : Performs vision application
  Modified by  : 29-10-21 - Srikanth
                 09-11-21 - Srikanth introduced try,catch methods
                 11-11-21 - Srikanth integrated hardware camera
                 26-11-21 - Srikanth modified code to create a file
                 and write into file in root directory
                 04-01-22 - Srikanth added a conditon to validate camera ID
                 path
                 05-01-22 - Srikanth added a condition to show decoded data
                 on console
                 06-01-22 - Srikanth added a condition to display captured
                 image on console
                 07-01-22 - Srikanth modified that places of fileExists function and creation of object
                 for VideoCapture class are inter changed
                 12-01-22 - Srikanth modified start function that writing data in tmp directory as long as
                 camera gives the data
                 13-01-22 - Srikanth modified code to write position status of agv in tmp directory based on
                 reference qrcode
                 22-01-22 - Srikanth modified code to write orientation of qr code by which direction agv is
                 moving in tmp directory and dissabled the find angle calling function since motion control code
                 is depending on orientation instead of angle
*/

int Barcode::start() {
  // Validating the provided camera ID path
  if(!fileExists(CAMERA_ID)){
    std::cout << "Invalid camera ID path\n" << std::endl;
    return 0;
  }

  // Starts the video camera based on the camera ID path.
  // Keep the camera ON.
  cv::VideoCapture cap(CAMERA_ID);

  if(!cap.isOpened()) {
    std::string text ="Can not open the camera for technical reasons \
Please contact Anzo.\n";
    logText(text);
    logFile->close();
    return 0;
  }

  // Declaring a beforeScalingImageMat object to store captured image
  // and imageMat to the casted unsingned char (8-bit) image
  cv::Mat imageMat, beforeScalingImageMat;
  cap.set(cv::CAP_PROP_CONVERT_RGB, false);
  cap.set(cv::CAP_PROP_EXPOSURE, EXPOSURE_VALUE);
  bool value = false;
  while(true) {
    // Declarations & capturing the current cpu time
    cpuTime startSystemTime = std::chrono::high_resolution_clock::now();

    // Setting the date & time
    dateAndTime->setDateAndTime(time(0));

    // Logging current time and date
    if (getDebugLevel() > 0) {
      logText(dateAndTime->getDateAndTime());
    }

    // Increasing the frame number
    setFrameNumber(getFrameNumber()+1);

    // Exception handling
    try {
      // Reading per frame and if the frame did not get read properly,
      // log the error and continue with next frame
      // TODO: We should stop somewhere to avoid not logging much.
      cap >> beforeScalingImageMat;

      if(beforeScalingImageMat.empty()) {
        std::string text = "It seems like We have encountered a mechanical failure. \
We would be unable to proceed. Please contact Anzo for next steps. You can also \
try checking the camera connections at your own condition.\n";
        logText(text);
	break;
      }

      // Cast the result(with saturation) to an unsigned char (8-bit).
      cv::convertScaleAbs(beforeScalingImageMat, imageMat, 0.2490234375);

      // Dispalying the captured image on console after scaling
      if(isImageShowOnConsole()) {
        cv::imshow("Video show", imageMat);
        cv::waitKey(1);
      }

      // Populated the data
      // decodedInfo->performOpenCVDecode(imageMat);
      value = decodedInfo->performZBarDecode(imageMat, this);
    } catch(cv::Exception& e) {
      const char* err_msg = e.what();
      std::string errorText = "Unable to proceed further due to unforeseen problems, \
Please contact Anzo Controls Pvt Ltd. \n";
      std::cout << errorText << std::endl;
      logText(errorText);
      logText("Exception caught:\n");
      logText(err_msg);
      break; 
    }
    if(!value) {
      // If we were not able to detect and decode the data, then
      // log about the problem and continue with next frame
      // Logging frame number in failure case 
      if(getDebugLevel() > 0) {
        std::string text = "Can not detect/decode any data from the frame " +
                            std::to_string(getFrameNumber());
        logText(text);
      }
      // Log time calculation
      if(getDebugLevel() > 2) {
        logTime(startSystemTime);
      }
    } else {
      // Logging the decoded data from QR code
      if(getDebugLevel() > 0) {
        std::string text = decodedInfo->getCurrentDecodedData() + "\t" +
                           decodedInfo->getReferenceBarcodeData() + "\t" +
                           std::to_string(getFrameNumber());
        logText(text);
      }

#ifdef FIND_ANGLE
      // Calling getAngle function to get in which QR code detected 
      if(decodedInfo->getCurrentDecodedData().size()){
        angle->findAngle(this);
      }
#endif

      // Logs the time
      if(getDebugLevel() > 2) {
        logTime(startSystemTime);
      }
      logText("\n");

#ifdef FIND_ANGLE
      // Logging four corners of QR code in x,y coordinates
      if(getDebugLevel() > 3) {
        for(auto& corner : decodedInfo->getDecodedCorners()) {
          logText(cv::format("[%d, %d]", corner.x, corner.y));
        }
      }
#endif

    }
    // Algo: If the data is there, then we have to write it.
    // If the tmp file is not there, write in normal mode.
    // If the tmp file is there, truncate that file and write the data.
    if(isOutputShowOnConsole()) {
      std::cout << "Current : " << decodedInfo->getCurrentDecodedData() << ", Last : " << decodedInfo->getLastDecodedData() << ", Ref : " << decodedInfo->getReferenceBarcodeData() << ", Last : " << decodedInfo->getLastReferenceBarcodeData() << ", Global : " << decodedInfo->getGlobalData() << ", Frame : " << std::to_string(getFrameNumber()) <<" ,ori:"<<decodedInfo->getOrientation()<< std::endl;
    }

    // This condition checks is any qr code is decoded
    if(decodedInfo->getQrcodeData().size()) {
      bool isRef  = decodedInfo->getReferenceBarcodeData().size();
      bool isRefL = isRef && decodedInfo->getReferenceBarcodeData().at(0) == 'L';
      bool isRefR = isRef && decodedInfo->getReferenceBarcodeData().at(0) == 'R';
      bool isRefU = isRef && decodedInfo->getReferenceBarcodeData().at(0) == 'U';
      bool isRefD = isRef && decodedInfo->getReferenceBarcodeData().at(0) == 'D';
      // If this condition is true means agv is moving in ether South or North direction that means we don't
      // have to consider R & L reference data
      if((decodedInfo->getOrientation() == 1 || decodedInfo->getOrientation() == 3) && (isRefL || isRefR) &&
         !decodedInfo->getCurrentDecodedData().size()) {
	 continue;
      }
      // If this condition is true means agv is moving in ether East or West direction that means we don't
      // have to consider U & D reference data
      if((decodedInfo->getOrientation() == 2 || decodedInfo->getOrientation() == 0) && (isRefU || isRefD) &&
         !decodedInfo->getCurrentDecodedData().size()) {
         continue;
      }

      // If this condition confirms that we have decoded some data ,and data sholud be pass to motion control code
      if((decodedInfo->getCurrentDecodedData().size() &&
	   decodedInfo->getCurrentDecodedData() != decodedInfo->getGlobalData()) ||
         (decodedInfo->getReferenceBarcodeData().size() &&
	   decodedInfo->getReferenceBarcodeData().substr(1) != decodedInfo->getGlobalData())) {
        // Creating the file lock so that motion control code can not read the
        // data file unless .anzo file is there.
        std::ofstream lockFile(DATA_LOCK_FILE);
        // Writing the data to data output file
        std::ofstream ofs;
        if(!fileExists(DATA_OUTPUT_FILE)) {
          ofs.open(DATA_OUTPUT_FILE);
        } else {
          ofs.open(DATA_OUTPUT_FILE, std::fstream::trunc);
        }
        // Algo: If binary data & reference data are there, then
        //         Write data, angle & position
        //       If only binary data is there
        //         Write only data & angle
        //       If only reference data is there
        //         Write only position
        std::string text;
        if(decodedInfo->getCurrentDecodedData().size() && decodedInfo->getReferenceBarcodeData().size()) {
          text  = "D:" + decodedInfo->getCurrentDecodedData() + "\n";
          text += "O:" + std::to_string(decodedInfo->getOrientation()) + "\n";
          text += "P:" + decodedInfo->getReferenceBarcodeData() + "\n";
        } else if(decodedInfo->getCurrentDecodedData().size()) {
          text = "D:" + decodedInfo->getCurrentDecodedData() + "\n" ;
	        text += "O:" + std::to_string(decodedInfo->getOrientation()) + "\n";
        } else if(decodedInfo->getReferenceBarcodeData().size()){
          text = "O:" + std::to_string(decodedInfo->getOrientation()) + "\n";
          text += "P:" + decodedInfo->getReferenceBarcodeData() + "\n";
        }
        text += "F:" + std::to_string(getFrameNumber());

        ofs << text;
        ofs.close();
	// File written DONE.

	std::string tmpData;
        if(decodedInfo->getQrcodeData().size() &&
           decodedInfo->getQrcodeData().at(0) != 'R' &&
           decodedInfo->getQrcodeData().at(0) != 'L' &&
           decodedInfo->getQrcodeData().at(0) != 'U' &&
           decodedInfo->getQrcodeData().at(0) != 'D') {
          tmpData = decodedInfo->getQrcodeData();
	} else {
          tmpData = decodedInfo->getQrcodeData().substr(1);
	}
	if(tmpData != decodedInfo->getGlobalData()) {
          decodedInfo->setGlobalData(tmpData);
	}

        std::remove(DATA_LOCK_FILE);
      }
    }

    // To add new line character in data log file
    if(getDebugLevel() > 0) {
      logText("\n");
    }
  }

  logFile->close();
  cap.release();
  return 0;
}

/*
  Function     : fileExists
  Accepts      : file address
  Returns      : bool
  Abstraction  : Take file path as input and checks file wheather exists
                 or not, and returns boolean value
  Modified by  : 26-11-21 - Srikanth
*/
static inline bool fileExists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

/*
  Function     : logTime
  Accepts      : cpuTime startSystemTime
  Returns      : None
  Abstraction  : Logs the time taken by each frame by
                 calculating difference between stop and start high
  Modified by  : 13-10-21 - Srikanth 
                 10-11-21 - Srikanth added the inline keyword to reduce the
                 function call overhead
*/
inline void Barcode::logTime(cpuTime startSystemTime) {
  cpuTime stopSystemTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> timeDifferencePerFrame = stopSystemTime - startSystemTime;
  logText(std::to_string(timeDifferencePerFrame.count()));
}

/*
  Function     : logText
  Accepts      : std::string data
  Returns      : None
  Abstraction  : Take string data as arguments and
                 log the data
  Modified by  : 13-10-21 - Srikanth
                 10-11-21 - Srikanth added the inline keyword to reduce the
                 function call overhead
                 04-01-22 - Srikanth removed inline keyword to resolve
                 compiler dependency problems
*/
void Barcode::logText(std::string data) {
  if(getDebugLevel() > 0) {
    *(getLogFile()) << data << "\t";
  }
}

/*
  Function     : getDebugLevel()
  Accepts      : None
  Returns      : int
  Abstraction  : Take nothing and returns debugLevel value 
  Modified by  : 13-10-21 - Srikanth 
                 10-11-21 - Srikanth added the inline keyword to reduce the
                 function call overhead
                 04-01-22 - Srikanth removed inline keyword to resolve
                 compiler dependency problems
*/
int Barcode::getDebugLevel(){
  return debugLevel;
}
/*
  Function       : getTargetRow
  Accepts        : None
  Returns        : int tergetRow
  Abstraction    : Returns targetRow attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline int Barcode::getTargetRow() {
  return targetRow;
}

/*
  Function       : getTargetColumn
  Accepts        : None
  Returns        : int tergetColumn
  Abstraction    : Returns targetColumn attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline int Barcode::getTargetColumn() {
  return targetColumn;
}

/*
  Function       : getSourceRow
  Accepts        : None
  Returns        : int sourceColumn
  Abstraction    : Returns  attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline int Barcode::getSourceRow() {
  return sourceRow;
}

/*
  Function       : getSourceColumn
  Accepts        : None
  Returns        : int sourceColumn
  Abstraction    : Returns sourceColumn attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline int Barcode::getSourceColumn() {
  return sourceColumn;
}

/*
  Function       : getCurrentRow
  Accepts        : None
  Returns        : int currentRow
  Abstraction    : Returns currentRow attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline int Barcode::getCurrentRow() {
  return currentRow;
}

/*
  Function       : getFrameNumber
  Accepts        : None
  Returns        : int frameNumber
  Abstraction    : Returns frameNumber attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline int Barcode::getFrameNumber() {
  return frameNumber;
}

/*
  Function       : getCurrentColumn
  Accepts        : None
  Returns        : int currentColumn
  Abstraction    : Returns currentColumn attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline int Barcode::getCurrentColumn() {
  return currentColumn;
}

/*
  Function       : isOutputShowOnConsole
  Accepts        : None
  Returns        : bool
  Abstraction    : Returns outPutConsoleValue attribute of Barcode class
  Modified by    : 4-01-22 - Srikanth
*/
inline bool Barcode::isOutputShowOnConsole() {
  return outPutConsoleValue;
}

/*
  Function       : isImageShowOnConsole
  Accepts        : None
  Returns        : bool
  Abstraction    : Returns imageShowValue attribute of Barcode class
  Modified by    : 6-01-22 - Srikanth
*/
inline bool Barcode::isImageShowOnConsole() {
  return imageShowValue;
}


// Setters for Barcode class

/*
  Function       : setImageShowValue
  Accepts        : bool
  Returns        : None
  Abstraction    : Take bool value as argument and sets
                   imageShowValue attribute of Barcode class
  Modified by    : 6-01-22 - Srikanth
*/
void Barcode::setImageShowValue(bool value) {
  imageShowValue = value;
}

/*
  Function       : setOutPutConsoleValue
  Accepts        : bool
  Returns        : None
  Abstraction    : Take bool value as argument and sets
                   outPutConsoleValue attribute of Barcode class
  Modified by    : 4-01-22 - Srikanth
*/
void Barcode::setOutPutConsoleValue(bool value) {
  outPutConsoleValue = value;
}

/*
  Function       : setDebugLevel
  Accepts        : int
  Returns        : None
  Abstraction    : Take int debugValue as argument and sets
                   debugLevel attribute of Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
void Barcode::setDebugLevel(int debugValue) {
  debugLevel = debugValue;
}

/*
  Function       : setTargetRow
  Accepts        : int
  Returns        : None
  Abstraction    : Take int val as a parameter and sets
                   targetRow attribute in Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline void Barcode::setTargetRow(int val) {
  targetRow = val;
}

/*
  Function       : setTargetColumn
  Accepts        : int
  Returns        : None
  Abstraction    : Take int val as a parameter and sets
                   targetColumn attribute in Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
 */
inline void Barcode::setTargetColumn(int val) {
  targetColumn = val;
}

/*
  Function       : setSourceRow
  Accepts        : int
  Returns        : None
  Abstraction    : Take int val as a parameter and sets
                   sourceRow attribute in Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline void Barcode::setSourceRow(int val) {
  sourceRow = val;
}

/*
  Function       : setSourceColumn
  Accepts        : int
  Returns        : None
  Abstraction    : Take int val as a parameter and sets
                   sourceColumn attribute in Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline void Barcode::setSourceColumn(int val) {
  sourceColumn = val;
}

/*
  Function       : setCurrentRow
  Accepts        : int
  Returns        : None
  Abstraction    : Take int val as a parameter and sets
                   currentRow attribute in Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline void Barcode::setCurrentRow(int val) {
  currentRow = val;
}

/*
  Function       : setFrameNumber
  Accepts        : int
  Returns        : None
  Abstraction    : Take int val as a parameter and sets
                   frameNumber attribute in Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline void Barcode::setFrameNumber(int val) {
  frameNumber = val;
}

/*
  Function       : setCurrentColumn
  Accepts        : int
  Returns        : None
  Abstraction    : Take int val as a parameter and sets
                   currentColumn attribute in Barcode class
  Modified by    : 13-10-21 - Srikanth
                   10-11-21 - Srikanth added the inline keyword to reduce the
                   function call overhead
*/
inline void Barcode::setCurrentColumn(int val) {
  currentColumn = val;
}

/*
  Function       : Barcode::Barcode(), constructor
  Accepts        : None
  Returns        : None
  Abstraction    : Constructor
  Modified by    : 29-10-21 - Srikanth
                   04-01-22 - Srikanth added setOutPutConsoleValue
                   method in agv constructor
                   06-01-22 - Srikanth added setImageShowValue method
                   in agv constructor
*/
Barcode::Barcode() {
  decodedInfo = new Decode;
  angle       = new Angle;
  dateAndTime = new Time;
  messageText = new Text;
  setTargetRow(TARGET_ROW);
  setTargetColumn(TARGET_COLUMN);
  setSourceRow(SOURCE_ROW);
  setSourceColumn(SOURCE_COLUMN);
  setOutPutConsoleValue(DEFAULT_CONSOLE_VALUE);
  setImageShowValue(DEFAULT_CONSOLE_VALUE);
  setCurrentRow(CURRENT_ROW);
  setFrameNumber(FRAME_NUMBER);
  setCurrentColumn(CURRENT_COLUMN);
  setDebugLevel(DEFAULT_VALUE);
  logFile = new std::ofstream();
  openLogFile(LOG_FILE_NAME);

  // TODO: handle this gracefully. We have added this here because
  // creating ctor at decode class is having undefined reference problems.
  decodedInfo->setOrientation(goingSouth);
}

/*
  Function       : Barcode::~Barcode(), destructor
  Accepts        : None
  Returns        : None
  Abstraction    : Destructor
  Modified by    : 29-10-21 - Srikanth
*/
Barcode::~Barcode() {
  delete decodedInfo;
  delete angle;
  delete dateAndTime;
  delete messageText;

  logFile->close();
}

/*
  Function      : getLogFile
  Accepts       : None
  Returns       : std::ofstream*
  Abstraction   : This function returns address of std::ofstream* logFile pointer
  Modified by   : 13-10-21 - Srikanth
                  10-11-21 - Srikanth added the inline keyword to reduce the
                  function call overhead
*/
inline std::ofstream* Barcode::getLogFile() {
  return logFile;
}

/*
  Function      : openLogFile
  Accepts       : std::string
  Returns       : None
  Abstraction   : Takes std::string,int as parameter and 
                  opens the file with std::ofstream* logFile pointer
  Modified by   : 13-10-21 - Srikanth
                  10-11-21 - Srikanth added the inline keyword  to reduce the
                  function call overhead
                  04-01-22 - Srikanth removed inline keyword to resolve
                  compiler dependency problems
*/
void Barcode::openLogFile(std::string filePathName) {
  if(getDebugLevel() > 0) {
    logFile->open(filePathName, std::ios::app);
 }
}
