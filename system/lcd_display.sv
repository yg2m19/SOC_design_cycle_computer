///////////////////////////////////////////////////////////////////////
//
// LCD display module
//
//    this version of the module is empty
//
//    a more advanced version might attempt to decode the inputs
//    to produce a value representing the number on the display
//
//    this module might also be the location of assert statements
//    to help with debugging
//
// Display module information:
//   Type: Nokia 5110 48 x 84 pixels
//   Driver chip: Philips PCD8544
//
//   Ports:
//	nRES	(Input - Active Low)
//		Reset pin
//
//	SCLK	(Input)
//		Clock for Serial Data
//
//	SDIN	(Input)
//		Serial Data
//
//	DnC	(Input)
//      	Data/nCommand 
//
//	nSCE	(Input - Active Low)
//      	Chip Enable
//
//
///////////////////////////////////////////////////////////////////////

`include "options.sv"

module lcd_display (
  input nRES,
  input SCLK,
  input SDIN,
  input DnC,
  input nSCE
  );

timeunit 1ns;
timeprecision 100ps;

  //
  // serial bus timing data from PCD8544 datasheet
  //
  // values are conservative, having been adjusted
  // assuming rise/fall time no greater than 10ns
  //
  specify
    specparam tcy=250ns;     // clock cycle SCLK

    specparam twh1=100ns;    // SCLK pulse width HIGH
    specparam twl1=100ns;    // SCLK pulse width LOW

    specparam tsu2=60ns;     // nSCE setup time
    specparam th2=100ns;     // nSCE hold time
    specparam twh2=100ns;    // nSCE minimum HIGH time

    // this is not needed? - given the checks applied below
    // specparam th5=100;    // nSCE start hold time

    specparam tsu3=100ns;    // DnC setup time
    specparam th3=100ns;     // DnC hold time

    specparam tsu4=100ns;    // SDIN setup time
    specparam th4=100ns;     // SDIN hold time

    specparam max_slew=10ns;

    // Serial clock timings
    $period(posedge SCLK, tcy);
    $width(posedge SCLK, twh1 + (2*max_slew));
    $width(negedge SCLK, twl1 + (2*max_slew));

    // Chip Enable timings relative to the clock
    $setup(nSCE, posedge SCLK, tsu2 + (2*max_slew));
    $hold(nSCE, posedge SCLK, th2 + (2*max_slew));
    // Chip Enable minimum inactive time
    $width(posedge nSCE, twh2 + (2*max_slew));


    // DnC timings relative to the clock
    $setup(DnC, posedge SCLK, tsu3 + (2*max_slew));
    $hold(DnC, posedge SCLK, th3 + (2*max_slew));

    // Serial Data timings relative to the clock
    $setup(SDIN, posedge SCLK, tsu4 + (2*max_slew));
    $hold(SDIN, posedge SCLK, th4 + (2*max_slew));


  endspecify
  
endmodule

