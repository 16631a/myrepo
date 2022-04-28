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
#include "Text.h"

/*
  Function    : boldON()
  Accepts     : None
  Return      : std::string
  Abstraction : This function on the bold text
  Modified by : 10-03-22 - Srikanth
*/
inline std::string TextMessage::boldON() {
  return "\e[1m";
}

/*
  Function    : boldON()
  Accepts     : None
  Return      : std::string
  Abstraction : This function off the bold text
  Modified by : 10-03-22 - Srikanth
*/
inline std::string TextMessage::boldOFF() {
  return "\e[0m";
}

/*
  Function    : showManpage
  Accepts     : None
  Return      : None
  Abstraction : Take noting and displays the manual page on console
  Modified by : 10-03-22 - Srikanth
*/
void TextMessage::showManpage(char** argv) {
  std::cout << boldON() << "NAME\n" << std::setw(50) <<
              "Anzo controls real time dimensions measurement \n" <<
               boldOFF() << std::endl;
  std::cout << boldON() << "SYNOPSIS \n" <<
               boldOFF() << "\t\t" << argv[0] << " [OPTION]..." << std::endl;
  std::cout << std::endl;

  std::cout << boldON() << "DESCRIPTION" <<
               boldOFF() << std::endl;
  std::cout << "\t\t" <<
               "This is a computer vision application,which determines dimensions of rectangular and \
square shape objects in real world." << std::endl;
  std::cout << std::endl;
  std::cout << boldON() << "\t\t" << "-g, log debug info\n" <<
               boldOFF() <<
               "\t\t\tThis option would store the processed data by application into log file(dimensions_data.log)." << std::endl;
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
  Abstraction : Take char** as argument and shows how to execute vision application
  Modified by : 10-03-22 - Srikanth
*/
void TextMessage::showUsage(char** argv) {
  std::cout << std::endl;
  std::cout << boldON() << "Anzo controls real time demensions measurement:" <<
               boldOFF() << std::endl;
  std::cout << boldON() << "EXAMPLE \n" << boldOFF() << std::endl;

  std::cout << std::setw(15) << argv[0] << " -m" << std::endl;
  std::cout << "\t\tTo get manual page of real time dimensions application\n" << std::endl;

  std::cout << std::setw(15) << argv[0] << " -g [positive integer]" << std::endl <<
               "\t\tLog information based upon the debug level. Standard permissible levels are 1 and 2.\n" << std::endl;

  std::cout << std::setw(15) << argv[0] << " [No option]" << std::endl <<
               "\t\tTo execute the application\n" << std::endl;
}

/*
  Function    : PrintHelp()
  Accepts     : None
  Return      : None
  Abstraction : Take noting and shows the help message on console
  Modified by : 10-3-22 - Srikanth
*/
void TextMessage::showHelp(char** argv) {
  std::cout << std::endl;
  std::cout << "Try '" << argv[0] << " -help' for more information\n";
}

