module input_soc(
		 input HCLK,
		 input HRESETn,

                 input nMode,
                 input nTrip,
                 input nFork,
                 input nCrank,



                 output logic [31:0]Switch_count_of_total_time,
  		 output logic [15:0]Switch_count_of_fork,
		 output logic [15:0]Switch_count_of_time_per_cycle_of_crank,
		 output logic [15:0]Switch_count_of_time_per_cycle_of_fork,
	  	 output logic [15:0]Switch_count_of_mode,
		 output logic [15:0]Switch_count_of_trip
);

timeunit 1ns;
timeprecision 100ps;

logic last_nFork;
logic last_nCrank;
logic [15:0]count_time_of_fork_per_cycle;
logic [15:0]count_time_of_crank_per_cycle;
logic [2:0]counter;
logic SIZE;
always_ff @(posedge HCLK, negedge HRESETn)
begin
     if(!HRESETn)
                 begin
                      Switch_count_of_total_time<=0;
  		      Switch_count_of_fork<=0;
		      Switch_count_of_time_per_cycle_of_crank<=0;
		      Switch_count_of_time_per_cycle_of_fork<=0;
	  	      Switch_count_of_mode<=0;
		      Switch_count_of_trip<=0;
                      count_time_of_fork_per_cycle<=0;
                      count_time_of_crank_per_cycle<=0;
                      last_nFork<=0;
                      last_nCrank<=0;
                      counter<=0;
                      SIZE<=0;
                 end
     else
         begin
              if(!nTrip)
                        begin
                             Switch_count_of_trip<=Switch_count_of_trip+1;
                        end
              else
                  begin
                       Switch_count_of_trip<=0;
                  end
              if(!nMode)
                        begin
                             Switch_count_of_mode<=Switch_count_of_mode+1;
                        end
              else
                  begin
                       Switch_count_of_mode<=0;
                  end
              count_time_of_fork_per_cycle<=count_time_of_fork_per_cycle+1;
              count_time_of_crank_per_cycle<=count_time_of_crank_per_cycle+1;
              last_nFork<=nFork;
              last_nCrank<=nCrank;
              if((last_nFork==1)&&(nFork==0))
                                begin
                                     Switch_count_of_fork<=Switch_count_of_fork+1;
                                     Switch_count_of_total_time<=Switch_count_of_total_time+count_time_of_fork_per_cycle;
                                     Switch_count_of_time_per_cycle_of_fork<=count_time_of_fork_per_cycle;
                                     count_time_of_fork_per_cycle<=0;
                                end
              if((last_nCrank==1)&&(nCrank==0))
                                               begin
                                                    Switch_count_of_time_per_cycle_of_crank<=count_time_of_crank_per_cycle;
                                                    count_time_of_crank_per_cycle<=0;
                                               end
if((Switch_count_of_trip>320)&&(Switch_count_of_mode>320))
begin
     SIZE<=1;
end

if(SIZE)
            begin
                 if((Switch_count_of_mode==320)&&(Switch_count_of_trip==0))
                    begin
                         counter<=counter+1;
                    end
                    if(counter==4)
                       begin
                            counter<=0;
                            SIZE<=0;
                            Switch_count_of_fork<=0;
			    Switch_count_of_total_time<=0;
                       end
             end
else
    begin
              if((Switch_count_of_trip==320)&&(Switch_count_of_mode==0))
                                                                        begin
                                                                             Switch_count_of_total_time<=0;
  		      							     Switch_count_of_fork<=0;
		      							     Switch_count_of_time_per_cycle_of_crank<=0;
		 							     Switch_count_of_time_per_cycle_of_fork<=0;
	  								     Switch_count_of_mode<=0;
									     Switch_count_of_trip<=0;
                  							     count_time_of_fork_per_cycle<=0;
                  							     count_time_of_crank_per_cycle<=0;
                   							     last_nFork<=0;
                  							     last_nCrank<=0;
                                                                        end
     end
              end
end
endmodule
               
                  
