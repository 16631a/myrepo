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

#include"calculation.h"
#include"headers.h"
#include <fstream>

static inline bool fileExists(const std::string& name);

/*
Function     : start
Accepts      : None
Returns      : int
Abstraction  : This functions measures the dimensions of object
in real time
Modified by  : 14-03-22 - Srikanth
*/
int Measurement::start()
{
  // Validating the provided camera ID path
  if(!fileExists(CAMERA_ID)){
    std::cout << "Invalid camera ID path\n" << std::endl;
    return 0;
  }
  cv::Mat src,srcGray,edge,dil;
  cv::VideoCapture cap(CAMERA_ID);
  if(!cap.isOpened()) {
    std::string text ="Can not open the camera for technical reasons \
Please contact Anzo.\n";
    logText(text);
    logFile->close();
    return 0;
  }
  cap.set(cv::CAP_PROP_CONVERT_RGB, false);
  cap.set(cv::CAP_PROP_EXPOSURE,100);
  bool value = false;
  while(1){
    // Declarations & capturing the current cpu time
    cpuTime startSystemTime = std::chrono::high_resolution_clock::now();
    try{
      // Reading image from camera feed
      cap >> src;
      //std::cout << "Width : " << src.size().width << std::endl;
      //std::cout << "Height: " << src.size().height << std::endl;
      if(src.empty()) {
        std::cout<<"It seems like We have encountered a mechanical failure. \
					We would be unable to proceed. Please contact Anzo for next steps. You can also \
					try checking the camera connections at your own condition.\n";
        break;
      }
      // Cast the result(with saturation) to an unsigned char (8-bit).
      cv::convertScaleAbs(src,src, 0.2490234375);
      if(isImageShowOnConsole()){
        cv::imshow( "original", src );
        cv::waitKey(1);
      }
      if(pixelsPerMetric->isfindArucoMarker(src)){
        // Removing noise from image 
        cv::GaussianBlur(src,srcGray, cv::Size(7,7),0);
        // Converting the image into binary image
        cv::threshold(srcGray,srcGray, 90, 110, cv::THRESH_BINARY);
        //cv::imshow( "src_gray", src_gray );
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        //perform edge detection, then perform a dilation + erosion to
        //close gaps in between object edges
        cv::Canny( srcGray, edge,90,110,3);
        cv::dilate(edge,dil, 7,cv::Point(-1,-1),5);
        cv::erode(dil,edge,7,cv::Point(-1,-1),5);
        // Finding contours of object in image
        cv::findContours(edge,contours, cv::RETR_EXTERNAL ,cv::CHAIN_APPROX_SIMPLE );
        // Finding dimensions of object in image
        value = dimensions->isFindDimensions(this,contours,src);
      }
    }catch(cv::Exception& e) {
       const char* err_msg = e.what();
       std::string errorText = "Unable to proceed further due to unforeseen problems, \
Please contact Anzo Controls Pvt Ltd. \n";
       std::cout << errorText << std::endl;
       logText(errorText);
       logText("Exception caught:\n");
       logText(err_msg);
       break;
    }
    if(value){
      if(getDebugLevel() > 1){
        // Logging height and width in dimensions_data.log file
        std::string text= "Height:" + std::to_string(dimensions->getLength()) + "\t";
        text += "Width:" + std::to_string(dimensions->getWidth());
        logText(text);
      }
      // Logging the time taken to proceess one frame 
      if(getDebugLevel() > 0) {
        logText("process_time: ");
        logTime(startSystemTime);
      }

      // Writing the data to data output file in /tmp directory
      std::ofstream ofs;
      if(!fileExists(DATA_OUTPUT_FILE)) {
         ofs.open(DATA_OUTPUT_FILE);
      } else {
         ofs.open(DATA_OUTPUT_FILE, std::fstream::trunc);
      }
      std::string text = "W:" + std::to_string(dimensions->getWidth()) + "\n";
      text += "H:" + std::to_string(dimensions->getLength());
      ofs << text;
      ofs.close();
      // For new line character in data.log file
      logText("\n");
    }

    if((std::string)(std::getenv("DEBUG_DIMENSIONS"))=="true"){
      static int i=0;
      if(value){
        // Writting successfully proccessed images
        std::string detectedFrameName = "/home/srikanth/Vision/dimensions/detected" + std::to_string(i) + ".jpg";
        cv::imwrite(detectedFrameName,src);
      }else {  
	 // Writting unsuccessfully proccessed images
	 std::string notDetectedFrameName = "/home/srikanth/Vision/dimensions/notdetected/" + std::to_string(i) + ".jpg";
	 cv::imwrite(notDetectedFrameName,src);
      }
      i++;
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
Modified by  : 10-03-22 - Srikanth
*/
static inline bool fileExists(const std::string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

/*
Function     : logText
Accepts      : std::string data
Returns      : None
Abstraction  : Take string data as arguments and
log the data
Modified by  : 10-03-22 - Srikanth
*/
void Measurement::logText(std::string data) {
  if(getDebugLevel() > 0) {
    *(getLogFile()) << data << "\t";
  }
}

/*
Function     : logTime
Accepts      : cpuTime startSystemTime
Returns      : None
Abstraction  : Logs the time taken by each frame by
calculating difference between stop and start high
Modified by  : 10-03-22 - Srikanth 
*/
inline void Measurement::logTime(cpuTime startSystemTime) {
  cpuTime stopSystemTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> timeDifferencePerFrame = stopSystemTime - startSystemTime;
  logText(std::to_string(timeDifferencePerFrame.count()));
}

/*
Function       : Measurement::Measurement(), constructor
Accepts        : None
Returns        : None
Abstraction    : Constructor
Modified by    : 10-03-22 - Srikanth
*/
Measurement::Measurement() {
  pixelsPerMetric = new Aruco;
  dimensions = new Dimensions;
  messageText = new TextMessage;
  logFile = new std::ofstream();
  setDebugLevel(DEFAULT_VALUE);
  openLogFile(LOG_FILE_NAME);
  setOutPutConsoleValue(DEFAULT_CONSOLE_VALUE);
  setImageShowValue(DEFAULT_CONSOLE_VALUE);
}

/*
Function       : Measurement::~Measurement(), destructor
Accepts        : None
Returns        : None
Abstraction    : Destructor
Modified by    : 10-03-22 - Srikanth
*/
Measurement::~Measurement() {
  delete pixelsPerMetric;
  delete dimensions;
  delete messageText;

  logFile->close();
}

/*
Function     : getDebugLevel()
Accepts      : None
Returns      : int
Abstraction  : Take nothing and returns debugLevel value 
Modified by  : 10-03-22 - Srikanth 
*/
int Measurement::getDebugLevel(){
  return debugLevel;
}

/*
Function      : getLogFile
Accepts       : None
Returns       : std::ofstream*
Abstraction   : This function returns address of std::ofstream* logFile pointer
Modified by   : 10-03-22 - Srikanth
*/
inline std::ofstream* Measurement::getLogFile() {
  return logFile;
}

/*
  Function       : isOutputShowOnConsole
  Accepts        : None
  Returns        : bool
  Abstraction    : Returns outPutConsoleValue attribute of Agv class
  Modified by    : 10-03-22 - Srikanth
*/
bool Measurement::isOutputShowOnConsole() {
  return outPutConsoleValue;
}

/*
  Function       : isImageShowOnConsole
  Accepts        : None
  Returns        : bool
  Abstraction    : Returns imageShowValue attribute of Agv class
  Modified by    : 10-03-22 - Srikanth
*/
bool Measurement::isImageShowOnConsole() {
  return imageShowValue;
}

/**************** Setters **************/

/*
  Function       : setImageShowValue
  Accepts        : bool
  Returns        : None
  Abstraction    : Take bool value as argument and sets
                   imageShowValue attribute of Agv class
  Modified by    : 10-03-22 - Srikanth
*/
void Measurement::setImageShowValue(bool value) {
  imageShowValue = value;
}

/*
  Function       : setOutPutConsoleValue
  Accepts        : bool
  Returns        : None
  Abstraction    : Take bool value as argument and sets
                   outPutConsoleValue attribute of Agv class
  Modified by    : 10-03-22 - Srikanth
*/
void Measurement::setOutPutConsoleValue(bool value) {
  outPutConsoleValue = value;
}

/*
Function       : setDebugLevel
Accepts        : int
Returns        : None
Abstraction    : Take int debugValue as argument and sets
debugLevel attribute of Agv class
Modified by    : 10-03-22 - Srikanth
*/
void Measurement::setDebugLevel(int debugValue) {
  debugLevel = debugValue;
}

/*
Function      : openLogFile
Accepts       : std::string
Returns       : None
Abstraction   : Takes std::string,int as parameter and
opens the file with std::ofstream* logFile pointer
Modified by   : 10-03-22 - Srikanth
*/
void Measurement::openLogFile(std::string filePathName) {
  if(getDebugLevel() > 0) {
    std::cout<<getDebugLevel();
    logFile->open(filePathName, std::ios::app);
  }
}

