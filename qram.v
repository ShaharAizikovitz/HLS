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
// Engineer: creator
// 
// Create Date: Wed Oct 17 05:16:26 2018

// Design Name: 
// Module Name: qram.v
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

module qram(
input[`int_32-1:0]data_in_1, data_in_2, input[`int_32-1:0]addr_1, addr_2, addr_3, addr_4, input store_1, store_2, clk,
output reg[`int_32-1:0]data_out_1, data_out_2, data_out_3, data_out_4
);

wire [`int_32 - 1:0] addr_x, addr_y;
assign addr_x = (store_1) ? addr_1 : addr_3;
assign addr_y = (store_2) ? addr_2 : addr_4;

dram mem1 (data_in_1, data_in_2, addr_x, addr_y, load_1, load_2, clk,data_out_1, data_out_2);
dram mem2 (data_in_1, data_in_2, addr_x, addr_y, load_1, load_2, clk,data_out_3, data_out_4);
endmodule