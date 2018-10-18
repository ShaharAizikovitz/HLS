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
// Module Name: dram.v
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

module dram(
input[`int_32-1:0]data_in_1, data_in_2, input[`int_32-1:0]addr_1, addr_2, input load_1, load_2, clk,
output reg[`int_32-1:0]data_out_1, data_out_2
);

reg [`int_32 - 1:0] memory[`int_1024-1:0];
//reading memory
always @(posedge clk) begin
if(!load_1) memory[addr_1] <= data_in_1;
else data_out_1 <= memory[addr_1];
end

always @(posedge clk) begin
if(!load_2) memory[addr_2] <= data_in_2;
else data_out_2 <= memory[addr_2];
end

endmodule