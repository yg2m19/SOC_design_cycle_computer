module comp_core(input logic Clock, nReset, nMode, nTrip, nFork, nCrank,SDI,Test,
                 output logic SegA, SegB, SegC, SegD, SegE, SegF, SegG, DP,SDO, logic [3:0]nDigit);
timeunit 1ns;
timeprecision 10ps;
logic [31:0]DataOut;
logic [31:0]Switch_count_of_total_time;
logic [15:0]Switch_count_of_fork;
logic [15:0]Switch_count_of_time_per_cycle_of_crank;
logic [15:0]Switch_count_of_time_per_cycle_of_fork;
logic [15:0]Switch_count_of_mode;
logic [15:0]Switch_count_of_trip;

LED_display LED_display1 (.HCLK(Clock), .HRESETn(nReset), .DataOut, .SegA, .SegB, .SegC, .SegD, .SegE, .SegF, .SegG, .DP, .nDigit);
arm_soc arm_soc1 (.HCLK(Clock), .HRESETn(nReset), .DataOut, .Switch_count_of_total_time, .Switch_count_of_fork, .Switch_count_of_time_per_cycle_of_crank, .Switch_count_of_time_per_cycle_of_fork, .Switch_count_of_mode, .Switch_count_of_trip);
input_soc input_soc1(.HCLK(Clock), .HRESETn(nReset), .nMode, .nTrip, .nFork, .nCrank, .Switch_count_of_total_time, .Switch_count_of_fork, .Switch_count_of_time_per_cycle_of_crank, .Switch_count_of_time_per_cycle_of_fork, .Switch_count_of_mode, .Switch_count_of_trip);
endmodule
