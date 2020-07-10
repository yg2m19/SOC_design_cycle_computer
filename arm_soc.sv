// Example code for an M0 AHBLite System
//  Iain McNally
//  ECS, University of Soutampton

module arm_soc(

  input HCLK, HRESETn,
  
  input [31:0] Switch_count_of_total_time,
  input [15:0] Switch_count_of_fork,
  input [15:0] Switch_count_of_time_per_cycle_of_crank,
  input [15:0] Switch_count_of_time_per_cycle_of_fork,
  input [15:0] Switch_count_of_mode,
  input [15:0] Switch_count_of_trip,

  output [31:0] DataOut,
  output DataValid,
  output LOCKUP

);
 
timeunit 1ns;
timeprecision 100ps;

  // Global & Master AHB Signals
  wire [31:0] HADDR, HWDATA, HRDATA;
  wire [1:0] HTRANS;
  wire [2:0] HSIZE, HBURST;
  wire [3:0] HPROT;
  wire HWRITE, HMASTLOCK, HRESP, HREADY;

  // Per-Slave AHB Signals
  wire HSEL_ROM, HSEL_RAM, HSEL_SW, HSEL_DOUT;
  wire [31:0] HRDATA_ROM, HRDATA_RAM, HRDATA_SW, HRDATA_DOUT;
  wire HREADYOUT_ROM, HREADYOUT_RAM, HREADYOUT_SW, HREADYOUT_DOUT;

  // Non-AHB M0 Signals
  wire TXEV, RXEV, SLEEPING, SYSRESETREQ, NMI;
  wire [15:0] IRQ;
  
  // Set this to zero because simple slaves do not generate errors
  assign HRESP = '0;

  // Set all interrupt and event inputs to zero (unused in this design) 
  assign NMI = '0;
  assign IRQ = {16'b0000_0000_0000_0000};
  assign RXEV = '0;

  // Coretex M0 DesignStart is AHB Master
  CORTEXM0DS m0_1 (

    // AHB Signals
    .HCLK, .HRESETn,
    .HADDR, .HBURST, .HMASTLOCK, .HPROT, .HSIZE, .HTRANS, .HWDATA, .HWRITE,
    .HRDATA, .HREADY, .HRESP,                                   

    // Non-AHB Signals
    .NMI, .IRQ, .TXEV, .RXEV, .LOCKUP, .SYSRESETREQ, .SLEEPING

  );


  // AHB interconnect including address decoder, register and multiplexer
  ahb_interconnect interconnect_1 (

    .HCLK, .HRESETn, .HADDR, .HRDATA, .HREADY,

    .HSEL_SIGNALS({HSEL_DOUT,HSEL_SW,HSEL_RAM,HSEL_ROM}),
    .HRDATA_SIGNALS({HRDATA_DOUT,HRDATA_SW,HRDATA_RAM,HRDATA_ROM}),
    .HREADYOUT_SIGNALS({HREADYOUT_DOUT,HREADYOUT_SW,HREADYOUT_RAM,HREADYOUT_ROM})

  );


  // AHBLite Slaves
        
  ahb_rom rom_1 (

    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
    .HSEL(HSEL_ROM),
    .HRDATA(HRDATA_ROM), .HREADYOUT(HREADYOUT_ROM)

  );

  ahb_ram ram_1 (

    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
    .HSEL(HSEL_RAM),
    .HRDATA(HRDATA_RAM), .HREADYOUT(HREADYOUT_RAM)

  );

  ahb_switches switches_1 (

    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
    .HSEL(HSEL_SW),
    .HRDATA(HRDATA_SW), .HREADYOUT(HREADYOUT_SW),

    .Switch_count_of_total_time(Switch_count_of_total_time), .Switch_count_of_fork(Switch_count_of_fork), .Switch_count_of_time_per_cycle_of_crank(Switch_count_of_time_per_cycle_of_crank), .Switch_count_of_time_per_cycle_of_fork(Switch_count_of_time_per_cycle_of_fork), .Switch_count_of_mode(Switch_count_of_mode), .Switch_count_of_trip(Switch_count_of_trip)
  );

  ahb_out out_1 (

    .HCLK, .HRESETn, .HADDR, .HWDATA, .HSIZE, .HTRANS, .HWRITE, .HREADY,
    .HSEL(HSEL_DOUT),
    .HRDATA(HRDATA_DOUT), .HREADYOUT(HREADYOUT_DOUT),

    .DataOut(DataOut), .DataValid(DataValid)

  );

endmodule

