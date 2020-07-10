module arm_soc_stim();
     
timeunit 1ns;
timeprecision 100ps;

  logic HRESETn, HCLK; 

  wire [31:0] DataOut;
  wire DataValid;
  wire LOCKUP;

  logic [31:0] Switch_count_of_total_time;
  logic [15:0] Switch_count_of_fork;
  logic [15:0] Switch_count_of_time_per_cycle_of_crank;
  logic [15:0] Switch_count_of_time_per_cycle_of_fork;
  logic [15:0] Switch_count_of_mode;
  logic [15:0] Switch_count_of_trip;




  arm_soc dut(.HCLK, .HRESETn, .DataOut, .DataValid, .Switch_count_of_total_time, .Switch_count_of_fork, .Switch_count_of_time_per_cycle_of_crank, .Switch_count_of_time_per_cycle_of_fork, .Switch_count_of_mode, .Switch_count_of_trip, .LOCKUP);

 initial
   begin
        HCLK=0;
        forever #5ns HCLK=~HCLK;
   end



  initial
    begin
            HRESETn = 0;
            Switch_count_of_fork=0;
            Switch_count_of_time_per_cycle_of_crank=0;
            Switch_count_of_total_time=0;
            Switch_count_of_time_per_cycle_of_fork=0;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;
      #10.0ns HRESETn = 1;
            
      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;


      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=330;
            Switch_count_of_trip = 0;


      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=330;
            Switch_count_of_trip = 0;


      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=330;
            Switch_count_of_trip = 0;


      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=330;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=330;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=330;
            Switch_count_of_trip = 0;

      #10us Switch_count_of_fork=10000;
            Switch_count_of_time_per_cycle_of_crank=12800;
            Switch_count_of_total_time=46080000;
            Switch_count_of_time_per_cycle_of_fork=12800;
            Switch_count_of_mode=0;
            Switch_count_of_trip = 0;

      #50us $stop;
            $finish;
    end
       
endmodule

