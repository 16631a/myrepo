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

/*
  Class name        : Time
  Private members   : dateAndTime
  Public methods    : setDateAndTime(time_t currentTime), getDateAndTime(),
  Abstraction       : This class maintains and updates dateAndTime
                       using setDateAndTime method and returns current date & time
                       using getDateAndTime method 
  Modified by       : 12-10-21 - Srikanth
*/
class Time {
  char* dateAndTime;
  public:
    void setDateAndTime(time_t currentTime);
    std::string getDateAndTime();
};
