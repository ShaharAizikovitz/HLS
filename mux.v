//`define timescale 1ns/1ps
`define int_0 0
`define int_1 1
`define int_2 2
`define int_10 10
`define int_32 32
`define int_64 64
`define mem_size 1024
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: shahar
// 
// Create Date: Wed Oct 17 05:16:26 2018

// Design Name: 
// Module Name: mux.v
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
//
// Dependencies: 
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

module mux(
     input [`int_32-1:0] store_val_4,
     input [`int_32-1:0] store_val_39,
     input [`int_32-1:0] store_val_41,
     input [`int_32-1:0] store_add_4,
     input [`int_32-1:0] store_add_39,
     input [`int_32-1:0] store_add_41,
     input [`int_32-1:0] load_add_0,
     input [`int_32-1:0] load_add_16,
     input [`int_32-1:0] load_add_23,
     output reg[`int_32-1:0] out_add_1,
     output reg[`int_32-1:0] out_add_2,
     output reg[`int_32-1:0] out_add_3,
     output reg[`int_32-1:0] out_add_4,
     output reg[`int_32-1:0] out_store_val_1,
     output reg[`int_32-1:0] out_store_val_2,
     output reg store_1, store_2,
     input [`int_2-1:0] state,
     input clk
);

     always @(posedge clk) begin
     case(state)
     01:
          begin
          out_add_1 <= store_add_4;
          out_store_val_1 <= store_val_4;
          out_add_2 <= load_add_0;
          store_1 <= 1;
          store_2 <= 0;
          end
     10:
          begin
          out_add_1 <= load_add_16;
          out_add_2 <= load_add_23;
          store_1 <= 0;
          store_2 <= 0;
          end
     11:
          begin
          out_add_1 <= store_add_39;
          out_store_val_1 <= store_val_39;
          out_add_2 <= store_add_41;
          out_store_val_2 <= store_val_41;
          store_1 <= 1;
          store_2 <= 1;
          end

     //end case 
     endcase
     //end always block 
     end
endmodule