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

// Forward declaration
class Barcode;

/*
  Class name      : Angle
  Private members : currentAngle, lastAngle
  Public methods  : findAngle(Barcode* agv),
                    getLastAngle(), setAngle(int angle),setAngle(int angle)
  Abstraction     : This class use findAngle function to find
                    angle with help of decodedCorners, and also 
                   maintains current angle & last angle
 Modified by     : 12-10-21 - Srikanth
*/
class Angle {
  int currentAngle;
  int lastAngle;
  bool currentDirection;
  bool lastDirection;
  public:
    void findAngle(Barcode*);
    void setAngle(int angle);
    int getCurrentAngle();
    int getLastAngle();
    void setDirection(bool direction);
    bool getCurrentDirection();
    bool getLastDirection();
};
