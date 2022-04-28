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
#include <cstdlib>
//#pragma once
#include"barcode/barcode.h"
#include"dimensions/calculation.h"

class Vision{
    public:
      Barcode* barcodeInfo1;
      Measurement* realTimeDimensions1;
      Vision();
      ~Vision();
};

int main(int argc, char* argv[])
{
  Barcode* barcodeInfo                    = new Barcode();
  Measurement* realTimeDimensions = new Measurement();
  //Vision* vision = new Vision();
  if((std::string)(std::getenv("BARCODE"))=="true"){  
    std::cout << "BARCODE: " << std::getenv("BARCODE") << '\n';
    barcodeInfo->barcode(argc,argv,realTimeDimensions);
  }
  if((std::string)(std::getenv("DIMENSIONS"))=="true"){
    std::cout << "dimensions " << std::getenv("DIMENSIONS")<< '\n';
    realTimeDimensions->dimension(argc,argv,barcodeInfo);
  }

  return 0;
}

Vision::Vision(){
	barcodeInfo1 = new Barcode();
	realTimeDimensions1 = new Measurement();
}
Vision::~Vision(){
	delete barcodeInfo1;
	delete realTimeDimensions1;
}

