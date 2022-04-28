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
#include<iomanip>
#include "text.h"

/*
  Function    : boldON()
  Accepts     : None
  Return      : std::string
  Abstraction : This function on the bold text
  Modified by : 22-10-21 - Srikanth
                10-11-21 - Srikanth added the inline keyword
*/
inline std::string Text::boldON() {
  return "\e[1m";
}

/*
  Function    : boldON()
  Accepts     : None
  Return      : std::string
  Abstraction : This function off the bold text
  Modified by : 21-10-21 - Srikanth
                10-11-21 - Srikanth added the inline keyword
*/
inline std::string Text::boldOFF() {
  return "\e[0m";
}

/*
  Function    : showManpage
  Accepts     : None
  Return      : None
  Abstraction : Take noting and displays the manual page on console
  Modified by : 22-10-21 - Srikanth
                12-01-22 - Srikanth changed flags usage according to the parser function
*/
void Text::showManpage(char** argv) {
  std::cout << boldON() << "NAME\n" << std::setw(50) <<
              "Anzo controls vision solutions \n" <<
               boldOFF() << std::endl;
  std::cout << boldON() << "SYNOPSIS \n" <<
               boldOFF() << "\t\t" << argv[0] << " [OPTION]..." << std::endl;
  std::cout << std::endl;

  std::cout << boldON() << "DESCRIPTION" <<
               boldOFF() << std::endl;
  std::cout << "\t\t" <<
               "This application is used to detects and decode the QR code for moving automated guided vehicle. It \
also figures out the angle." << std::endl;
  std::cout << std::endl;
  std::cout << boldON() << "\t\t" << "-g, log debug info\n" <<
               boldOFF() <<
               "\t\t\tThis option would store the information for each barcode detection into log file(data.log)." << std::endl;
  std::cout << boldON() << "\t\t" << "-u, usage guide\n" <<
               boldOFF() << "\t\t\t" <<
               "Displays how this application can be used along with its options.\n";
  std::cout << boldON() << "\t\t" << argv[0] << " \n" <<
               boldOFF() << "\t\t\t" << "Performs the task \n";
}

/*
  Function    : showUsage
  Accepts     : None
  Return      : None
  Abstraction : Take char** as argument and shows how to execute barcode application
  Modified by : 21-10-21 - Srikanth
                12-01-22 - Srikanth chnaged flags usage according to parser function
*/
void Text::showUsage(char** argv) {
  std::cout << std::endl;
  std::cout << boldON() << "Anzo controls barcode application can be used by below given methods:" <<
               boldOFF() << std::endl;
  std::cout << boldON() << "EXAMPLE \n" << boldOFF() << std::endl;

  std::cout << std::setw(15) << argv[0] << " -m" << std::endl;
  std::cout << "\t\tTo get manual page of barcode application\n" << std::endl;

  std::cout << std::setw(15) << argv[0] << " -g [positive integer]" << std::endl <<
               "\t\tLog information based upon the debug level. Standard permissible levels are 1 to 5.\n" << std::endl;

  std::cout << std::setw(15) << argv[0] << " [No option]" << std::endl <<
               "\t\tTo execute barcode application\n" << std::endl;
}

/*
  Function    : PrintHelp()
  Accepts     : None
  Return      : None
  Abstraction : Take noting and shows the help message on console
  Modified by : 21-10-21 - Srikanth
*/
void Text::showHelp(char** argv) {
  std::cout << std::endl;
  std::cout << "Try '" << argv[0] << " --help' for more information\n";
}

