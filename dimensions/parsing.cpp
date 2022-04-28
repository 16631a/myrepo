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

#include <getopt.h>
#include <iostream>
#include <string>
#include "calculation.h"
#include "headers.h"
#include<string>
/*
  Function    : ProcessArgs(int argc, char** argv)
  Accepts     : int argc, char** argv
  Return      : bool
  Abstraction : Take int argc, char** argv as arguments and parse the arguments and \
                retutrns boolean valuE
  modified by : 21-03-22 - Srikanth
*/
bool Measurement::processArgs(int argc, char** argv) {

if(argc > 1 && argv[1][0]=='-'){
  for(int i=1;i<argc ; i++){
    if(strcmp(argv[i],"--debug")==0){
      if(argc > i+1){
        //argumentedOPtion(int argc, char** argv);
        if(argv[i+1][0] !='-'){
          setDebugLevel(std::stoi(argv[i+1]));
          openLogFile(LOG_FILE_NAME);
        }else{
          std::cout<<argv[i]<<" option requires an argument integer."<<std::endl;
          return false;
        }
      }else{
        std::cout<<argv[i]<<" option requires an argument integer."<<std::endl;
        return false;
      }
    }

  //}
  //for(int i=1 ; i< argc ;i++){
    if(strcmp(argv[i],"--showdata")==0){
      setOutPutConsoleValue(true);
    }
    if(strcmp(argv[i],"--showimage")==0){
      setImageShowValue(true);
    }
    if(strcmp(argv[i],"--showusage")==0){
      messageText->showUsage(argv);
      return false;
    }
    if(strcmp(argv[i],"--help")==0){
      messageText->showUsage(argv);
      return false;
    }
    if(strcmp(argv[i],"--showmanpage")==0){
      messageText->showManpage(argv);
      return false;
    }
  }
}
return true;
}
/*
  Function    : dimension()
  Accepts     : int argc, char** argv,Agv*
  Return      : None
  Abstraction : Take int argc, char** argv,Barcode* as arguments and parse the arguments and based 
                upon on return value of processArgs function call the application start function
  Modified by : 21-03-22 - Srikanth
*/

void Measurement::dimension(int argc, char **argv,Barcode* agvInfo){
  if(processArgs(argc,argv)){ 
    start();
  }
}


