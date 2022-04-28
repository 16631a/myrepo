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
#include <ctime>
#include <regex>
#include "time.h"

/*
  Function    : setDateAndTime()
  Arguments   : time_t currentTime
  Returns     : None
  Abstraction : setDateAndTime function updates current date and time
  Modified by : 12-10-21 - Srikanth
*/
void Time::setDateAndTime(time_t currentTime) {
  dateAndTime = ctime(&currentTime);
}

/*
  Function    : getDateAndTime
  Arguments   : None
  Returns     : std::string
  Abstraction : It returns the dateAndTime attribute of Time
                class in string format
  Modified by : 12-10-21 - Srikanth
*/
std::string Time::getDateAndTime() {
  std::regex replace("\n+");
  return std::regex_replace(dateAndTime, replace,"");
}


