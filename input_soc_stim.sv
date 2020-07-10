module input_soc_stim;
timeunit 1ns;
timeprecision 100ps;

logic HRESETn, HCLK; 
logic nMode;
logic nTrip;
logic nFork;
logic nCrank;
logic Test;
logic SDI;
logic SDO;
logic [31:0] Switch_count_of_total_time;
logic [15:0] Switch_count_of_fork;
logic [15:0] Switch_count_of_time_per_cycle_of_crank;
logic [15:0] Switch_count_of_time_per_cycle_of_fork;
logic [15:0] Switch_count_of_mode;
logic [15:0] Switch_count_of_trip;




input_soc in1 (.HCLK, .HRESETn, .nMode, .nTrip, .nFork, .nCrank, .Test, .SDI, .SDO, .Switch_count_of_total_time, .Switch_count_of_fork, .Switch_count_of_time_per_cycle_of_crank, .Switch_count_of_time_per_cycle_of_fork, .Switch_count_of_mode, .Switch_count_of_trip);
 initial
   begin
        HCLK=0;
        forever #78125ns HCLK=~HCLK;
   end



  initial
    begin
            HRESETn = 0;
      #10000ns HRESETn = 1;
    end
  initial
    begin
            nFork=0;
     forever
            begin
      		 #0.25s nFork=1;
     		 #0.02s nFork=0;
            end
    end
  initial
    begin
            nCrank=0;
     forever
            begin
      		 #0.25s nCrank=1;
     		 #0.02s nCrank=0;
            end
    end
  initial
    begin
            nMode=1;
         forever
                begin
                     #5s nMode=0;
                     #1s nMode=1;
                end
    end
  initial
    begin
            nTrip=1;
           #10s nTrip=0;
           #1s nTrip=1;
    end
  initial
    begin
        #50s $stop;
    end
endmodule      
