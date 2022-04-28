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
#include "barcode.h"
#include "headers.h"
/*
  Function    : ProcessArgs(int argc, char** argv)
  Accepts     : int argc, char** argv
  Return      : bool
  Abstraction : Take int argc, char** argv as arguments and parse the arguments and \
                returns boolean value
  Modified by : 21-10-21 - Srikanth
                04-01-21 - Srikanth showoutput switch case is added to display
                output on console
                06-01-22 - Srikanth setting the imageShowValue to display
                image on console
                07-01-22 - Srikanth modified the switch cases
                12-01-22 - Srikanth removed long_options flag since agv application not getting full pledged
                output from that,and flag g is used to debugging data in log file ,flag d is used to show output
                on console
*/
bool Barcode::processArgs(int argc, char** argv) {
  const char* const short_opts = "g:umdih";

  while (true) {
    const auto opt = getopt_long(argc, argv, short_opts,
                                 nullptr, nullptr);

    if (-1 == opt) {
      break;
    } else {
      switch (opt) {
        case 'g':
          setDebugLevel(std::stoi(optarg));
	  openLogFile(LOG_FILE_NAME);
          break;
        case 'd':
          setOutPutConsoleValue(true);
          break;
        case 'i':
          setImageShowValue(true);
          break;
        case 'u':
          messageText->showUsage(argv);
          return false;
        case 'h':
          messageText->showUsage(argv);
          return false;
        case 'm':
          messageText->showManpage(argv);
          return false;
        case '?': // Unrecognized option
        default:
          messageText->showHelp(argv);
          return false;
      }
    }
  }
  return true;
}

/*
  Function    : barcode()
  Accepts     : int argc, char** argv,Measurement*
  Return      : None
  Abstraction : Take int argc, char** argv,Measurement* as arguments and parse the arguments and based
                upon on return value of processArgs function call the application start function
  Modified by : 21-03-22 - Srikanth
*/
void Barcode::barcode(int argc, char **argv,Measurement* dimensions) {
  if(processArgs(argc, argv)) {
    start();
  }
}
