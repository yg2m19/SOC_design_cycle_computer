// Example code for an M0 AHBLite System
//  Iain McNally
//  ECS, University of Soutampton
//
// This module is an AHB-Lite Slave containing seven read-only locations
//
// Number of addressable locations : 7
// Size of each addressable location : 32 bits
// Supported transfer sizes : Word
// Alignment of base address : Double Word aligned
//
// Address map :
//   Base addess + 0 : 
//     Read Switches count of TRIP
//   Base addess + 4 : 
//     Read Switches count of MODE
//   Base addess + 8 : 
//     Read Switches count of fork
//   Base addess + 12:
//     Read Switches count of total time
//   Base addess + 16:
//     Read Switches count of time per cycle of crank
//   Base addess + 20:
//     Read Swtiches count of time per cycle of fork
//   Base addess + 24:
//     Read Swtiches status
//
// Bits within status register :
//      this status bit is cleared when this data is read by the bus master)
//   Bit 5   DataValid[5] 
//     (data has been entered by count of time per cycle of fork)
//      this status bit is cleared when this data is read by the bus master)
//   Bit 4   DataValid[4]
//     (data has been entered by count of time per cycle of crank)
//      this status bit is cleared when this data is read by the bus master)
//   Bit 3   DataValid[3]
//     (data has been entered by count of total time)
//      this status bit is cleared when this data is read by the bus master)
//   Bit 2   DataValid[2] 
//     (data has been entered by count of fork)
//      this status bit is cleared when this data is read by the bus master)
//   Bit 1   DataValid[1]
//     (data has been entered by count of MODE)
//      this status bit is cleared when this data is read by the bus master)
//   Bit 0   DataValid[0] 
//     (data has been entered by count of TRIP)
//      this status bit is cleared when this data is read by the bus master)



module ahb_switches(

  // AHB Global Signals
  input HCLK,
  input HRESETn,

  // AHB Signals from Master to Slave
  input [31:0] HADDR, // With this interface HADDR is ignored
  input [31:0] HWDATA,
  input [2:0] HSIZE,
  input [1:0] HTRANS,
  input HWRITE,
  input HREADY,
  input HSEL,

  // AHB Signals from Slave to Master
  output logic [31:0] HRDATA,
  output HREADYOUT,

  //Non-AHB Signals
  input [31:0] Switch_count_of_total_time,
  input [15:0] Switch_count_of_fork,
  input [15:0] Switch_count_of_time_per_cycle_of_crank,
  input [15:0] Switch_count_of_time_per_cycle_of_fork,
  input [15:0] Switch_count_of_mode,
  input [15:0] Switch_count_of_trip
);

timeunit 1ns;
timeprecision 100ps;

  // AHB transfer codes needed in this module
  localparam No_Transfer = 2'b0;

  // Storage for seven different switch values  
  logic [31:0] switch_store[0:5];
  logic [5:0] DataValid;

  //control signals are stored in registers
  logic read_enable;
  logic [2:0] word_address;
 
  logic [31:0] Status;

  //Update the switch_store values only when the appropriate button is pressed
  always_ff @(posedge HCLK, negedge HRESETn)
    if ( ! HRESETn )
      begin
        switch_store[0] <= '0;
        switch_store[1] <= '0;
        switch_store[2] <= '0;
        switch_store[3] <= '0;
        switch_store[4] <= '0;
        switch_store[5] <= '0;
        DataValid <= 0;
      end
    else
      begin
            switch_store[0] <= Switch_count_of_trip;
            DataValid[0] <= 1;
        if ( read_enable && ( word_address == 0 ) )
          begin
            DataValid[0] <= 0;
          end
            switch_store[1] <= Switch_count_of_mode;
            DataValid[1] <= 1;
        if ( read_enable && ( word_address == 1 ) )
          begin
            DataValid[1] <= 0;
          end
            switch_store[2] <= Switch_count_of_fork;
            DataValid[2] <= 1;
        if ( read_enable && ( word_address == 2 ) )
          begin
            DataValid[2] <= 0;
          end
            switch_store[3] <= Switch_count_of_total_time;
            DataValid[3] <= 1;
        if ( read_enable && ( word_address == 3 ) )
          begin
            DataValid[3] <= 0;
          end
            switch_store[4] <= Switch_count_of_time_per_cycle_of_crank;
            DataValid[4] <= 1;
        if ( read_enable && ( word_address == 4 ) )
          begin
            DataValid[4] <= 0;
          end           
            switch_store[5] <= Switch_count_of_time_per_cycle_of_fork;
            DataValid[5] <= 1;
        if ( read_enable && ( word_address == 5 ) )
          begin
            DataValid[5] <= 0;
          end
      end


  //Generate the control signals in the address phase
  always_ff @(posedge HCLK, negedge HRESETn)
    if ( ! HRESETn )
      begin
        read_enable <= '0;
        word_address <= '0;
      end
    else if ( HREADY && HSEL && (HTRANS != No_Transfer) )
      begin
        read_enable <= ! HWRITE;
        word_address <= HADDR[4:2];
     end
    else
      begin
        read_enable <= '0;
        word_address <= '0;
     end

  //Act on control signals in the data phase

  // define the bits in the status register
  assign Status = { 26'd0, DataValid};

  //read
  always_comb
    if ( ! read_enable )
      // (output of zero when not enabled for read is not necessary
      //  but may help with debugging)
      HRDATA = '0;
    else
      case (word_address)
        0 : HRDATA = switch_store[0];
        1 : HRDATA = switch_store[1];
        2 : HRDATA = switch_store[2];
        3 : HRDATA = switch_store[3];
        4 : HRDATA = switch_store[4];
        5 : HRDATA = switch_store[5];
        6 : HRDATA = Status;
        // unused address - returns zero
        default : HRDATA = '0;
      endcase

  //Transfer Response
  assign HREADYOUT = '1; //Single cycle Write & Read. Zero Wait state operations



endmodule


