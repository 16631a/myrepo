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
  Class name        : Text
  Public methods    : boldON(), boldOFF(),showHelp(char** argv),
                      showManpage(char** argv),showUsage(char** argv)
  Abstraction       : This classs contain functions which provides 
                       text message
  Modified by       : 12-10-21 - Srikanth
*/
class Text {
  public:
    std::string boldON();
    std::string boldOFF();
    void showManpage(char** argv);
    void showUsage(char** argv);
    void showHelp(char** argv);
};
