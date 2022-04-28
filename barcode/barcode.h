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

#include <fstream>
#include "angle.h"
#include "decode.h"
#include "time.h"
#include "text.h"
#pragma once
#include "../dimensions/calculation.h"
#pragma once
//#include "../main.h"

// Forward diclaration of Measurement class
class Measurement;
typedef std::chrono::high_resolution_clock::time_point cpuTime;

/*
  Class name       : Barcode
  Private members  : targetRow, targetColumn,sourceRow, sourceColumn,
                     currentRow, frameNumber, currentColumn
  Public members   : decodedInfo, angle,dateAndTime,messageText
  Getters          : getTargetRow(),getTargetColumn(),getSourceRow(),getSourceColumn(),
                     getSourceColumn(),getCurrentColumn(),getFrameNumber(),getDebugLevel(),
                     getLogFile()
  Setters          : setTargetRow(int targetRow),setTargetColumn(int targetColumn),
                     setSourceRow(int sourceRow),setSourceColumn(int sourceColumn),
                     setCurrentRow(int currentRow),setCurrentColumn(int currentColumn),
                     setCurrentColumn(int currentColumn),setDebugLevel(int debugValue),
                     openLogFile(std::string filePathName)

  Constructors     : Barcode()

  Desructor        : ~Barcode()
  Modified by      : 29-10-21 - Srikanth
                     05-01-22 - Srikanth added outputConsoleValue attribute to take decision
                     whether to show output on console or not
*/
class Barcode {
  private:
    int targetRow, targetColumn;
    int sourceRow, sourceColumn;
    int currentRow, frameNumber, currentColumn;
    int debugLevel;
    bool outPutConsoleValue,imageShowValue;
    std::ofstream* logFile;
  public:
    friend class Measurement;
    // Pointers to other classes
    Decode* decodedInfo;
    Angle*  angle;
    Time*   dateAndTime;
    Text*  messageText;
    // Getters
    int getDebugLevel();
    bool isOutputShowOnConsole();
    bool isImageShowOnConsole();
    int getTargetRow();
    int getTargetColumn();
    int getSourceRow();
    int getSourceColumn();
    int getCurrentRow();
    int getFrameNumber();
    int getCurrentColumn();
    std::ofstream* getLogFile();

    // Setters
    void setOutPutConsoleValue(bool);
    void setImageShowValue(bool);
    void setDebugLevel(int debugValue);
    void setTargetRow(int targetRow);
    void setTargetColumn(int targetColumn);
    void setSourceRow(int sourceRow);
    void setSourceColumn(int sourceColumn);
    void setCurrentRow(int currentRow);
    void setFrameNumber(int frameNumber);
    void setCurrentColumn(int currentColumn);
    void openLogFile(std::string filePathName);

    // Logging functions
    void logText(std::string message);
    void logTime(cpuTime time);

    // Important functions
    void barcode(int argc, char **argv,Measurement*);
    int start();
    
    // Parse functions
    void showUsage(char** argv);
    void showManpage(char** argv);
    void showHelp(char** argv);
    bool processArgs(int argc, char** argv);
    // Constructors & destructors
    Barcode();
    ~Barcode();
};
