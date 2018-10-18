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
// Module Name: f4.v
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

module f4(
     input clk,
     input reset,
     input [`int_32-1:0]  v,
     input [`int_32-1:0]  k,
     output [`int_32-1:0] out_0
);


     localparam [`int_2 - 1:0] DEFAULT = 2'b00;
     localparam [`int_2 - 1:0] ENTRY = 2'b01;
     localparam [`int_2 - 1:0] FOR_BODY = 2'b10;
     localparam [`int_2 - 1:0] FOR_END = 2'b11;

     localparam [`int_32 - 1:0] i32_uundef = 0;
     localparam [`int_32 - 1:0] i32_0 = 0;
     localparam [`int_32 - 1:0] A = 0;
     localparam [`int_32 - 1:0] i32_100 = 100;
     localparam [`int_32 - 1:0] i64_1 = 1;
     localparam [`int_32 - 1:0] i32_5 = 5;
     localparam [`int_32 - 1:0] i64_56266160 = 56266160;
     localparam [`int_32 - 1:0] i64_0 = 0;


     //wire declaration:
     wire[`int_32 - 1:0] w_0_2;
     wire[`int_32 - 1:0] w_1_41;
     wire[`int_32 - 1:0] w_1_0;
     wire[`int_32 - 1:0] w_2_26;
     wire[`int_32 - 1:0] w_2_4;
     wire[`int_32 - 1:0] w_3_54;
     wire[`int_32 - 1:0] w_3_52;
     wire[`int_32 - 1:0] w_3_48;
     wire[`int_32 - 1:0] w_3_47;
     wire[`int_32 - 1:0] w_3_30;
     wire[`int_32 - 1:0] w_3_6;
     wire[`int_32 - 1:0] w_3_2;
     wire[`int_32 - 1:0] w_5_39;
     wire[`int_32 - 1:0] w_5_4;
     wire[`int_1 - 1:0] w_6_54;
     wire[`int_1 - 1:0] w_6_52;
     wire[`int_1 - 1:0] w_6_8;
     wire[`int_32 - 1:0] w_7_46;
     wire[`int_32 - 1:0] w_7_14;
     wire[`int_32 - 1:0] w_7_6;
     wire[`int_32 - 1:0] w_10_26;
     wire[`int_32 - 1:0] w_11_20;
     wire[`int_32 - 1:0] w_11_25;
     wire[`int_32 - 1:0] w_12_21;
     wire[`int_32 - 1:0] w_12_24;
     wire[`int_32 - 1:0] w_13_27;
     wire[`int_32 - 1:0] w_13_14;
     wire[`int_32 - 1:0] w_14_23;
     wire[`int_32 - 1:0] w_14_16;
     wire[`int_32 - 1:0] w_15_14;
     wire[`int_32 - 1:0] w_16_17;
     wire[`int_1 - 1:0] w_17_53;
     wire[`int_1 - 1:0] w_17_51;
     wire[`int_1 - 1:0] w_17_19;
     wire[`int_32 - 1:0] w_18_17;
     wire[`int_32 - 1:0] w_20_51;
     wire[`int_32 - 1:0] w_21_53;
     wire[`int_32 - 1:0] w_23_24;
     wire[`int_32 - 1:0] w_24_25;
     wire[`int_32 - 1:0] w_25_33;
     wire[`int_32 - 1:0] w_25_48;
     wire[`int_32 - 1:0] w_26_32;
     wire[`int_32 - 1:0] w_26_47;
     wire[`int_32 - 1:0] w_27_49;
     wire[`int_32 - 1:0] w_27_29;
     wire[`int_32 - 1:0] w_28_27;
     wire[`int_32 - 1:0] w_29_46;
     wire[`int_32 - 1:0] w_29_30;
     wire[`int_1 - 1:0] w_30_31;
     wire[`int_32 - 1:0] w_32_51;
     wire[`int_32 - 1:0] w_33_53;
     wire[`int_32 - 1:0] w_35_37;
     wire[`int_32 - 1:0] w_36_40;
     wire[`int_32 - 1:0] w_37_40;
     wire[`int_32 - 1:0] w_37_39;
     wire[`int_32 - 1:0] w_38_37;
     wire[`int_32 - 1:0] w_40_41;
     wire[`int_1 - 1:0] w_43_49;
     wire[`int_1 - 1:0] w_43_48;
     wire[`int_1 - 1:0] w_43_47;
     wire[`int_1 - 1:0] w_43_46;
     wire[`int_1 - 1:0] w_44_43;
     wire[`int_32 - 1:0] w_45_43;
     wire[`int_32 - 1:0] w_46_10;
     wire[`int_32 - 1:0] w_47_11;
     wire[`int_32 - 1:0] w_48_12;
     wire[`int_32 - 1:0] w_49_13;
     wire[`int_32 - 1:0] w_50_49;
     wire[`int_32 - 1:0] w_51_52;
     wire[`int_32 - 1:0] w_52_35;
     wire[`int_32 - 1:0] w_53_54;
     wire[`int_32 - 1:0] w_54_36;

     //memory input wire declaration:
     wire[`int_32 - 1:0] w_load_mem_in_1;
     wire[`int_32 - 1:0] w_load_mem_in_2;
     wire[`int_32 - 1:0] w_load_mem_in_3;
     wire[`int_32 - 1:0] w_load_mem_in_4;
     wire[`int_32 - 1:0] w_store_add_mem_in_1;
     wire[`int_32 - 1:0] w_store_val_mem_in_1;
     wire[`int_32 - 1:0] w_store_add_mem_in_2;
     wire[`int_32 - 1:0] w_store_val_mem_in_2;

     //store conditions wire declaration:
     wire[`int_1 - 1:0] w_store_1;
     wire[`int_1 - 1:0] w_store_2;

     //registers declaration
     reg[`int_32- 1:0] reg_10;
     reg[`int_32- 1:0] reg_11;
     reg[`int_32- 1:0] reg_12;
     reg[`int_32- 1:0] reg_13;

     //declare local parameters for memory module
     reg [`int_32-1:0] load_val_0;
     reg [`int_32-1:0] load_val_16;
     reg [`int_32-1:0] load_val_23;
     reg [`int_32-1:0] store_val_4 = 0;
     reg [`int_32-1:0] store_val_39 = 0;
     reg [`int_32-1:0] store_val_41 = 0;
     reg [`int_10-1:0] load_add_0 = 0;
     reg [`int_10-1:0] load_add_16 = 0;
     reg [`int_10-1:0] load_add_23 = 0;
     reg [`int_32-1:0] store_add_4 = 0;
     reg [`int_32-1:0] store_add_39 = 0;
     reg [`int_32-1:0] store_add_41 = 0;
     reg store_4;
     reg store_39;
     reg store_41;
     reg[`int_32 - 1:0] CONTROL_REGISTER;
     reg[`int_1:0] state, next;


     //wire combinatorial part
     assign w_0_2 = load_val_0;
     assign w_1_41 = v;
     assign w_1_0 = v;
     assign w_2_26 = w_3_2 - w_0_2;
     assign w_2_4 = w_3_2 - w_0_2;
     assign w_3_54 = k;
     assign w_3_52 = k;
     assign w_3_48 = k;
     assign w_3_47 = k;
     assign w_3_30 = k;
     assign w_3_6 = k;
     assign w_3_2 = k;
     assign w_5_39 = i32_uundef;
     assign w_5_4 = i32_uundef;
     assign w_6_54 = w_7_6 <= w_3_6;
     assign w_6_52 = w_7_6 <= w_3_6;
     assign w_6_8 = w_7_6 <= w_3_6;
     assign w_7_46 = i32_0;
     assign w_7_14 = i32_0;
     assign w_7_6 = i32_0;
     assign w_10_26 = reg_10;
     assign w_11_20 = reg_11;
     assign w_11_25 = reg_11;
     assign w_12_21 = reg_12;
     assign w_12_24 = reg_12;
     assign w_13_27 = reg_13;
     assign w_13_14 = reg_13;
     assign w_14_23 = w_15_14 + w_7_14 + w_13_14;
     assign w_14_16 = w_15_14 + w_7_14 + w_13_14;
     assign w_15_14 = A;
     assign w_16_17 = load_val_16;
     assign w_17_53 = w_16_17 <= w_18_17;
     assign w_17_51 = w_16_17 <= w_18_17;
     assign w_17_19 = w_16_17 <= w_18_17;
     assign w_18_17 = i32_100;
     assign w_20_51 = w_11_20;
     assign w_21_53 = w_12_21;
     assign w_23_24 = load_val_23;
     assign w_24_25 = w_12_24 * w_23_24;
     assign w_25_33 = w_24_25 + w_11_25;
     assign w_25_48 = w_24_25 + w_11_25;
     assign w_26_32 = w_2_26 - w_10_26;
     assign w_26_47 = w_2_26 - w_10_26;
     assign w_27_49 = w_13_27 + w_28_27;
     assign w_27_29 = w_13_27 + w_28_27;
     assign w_28_27 = i64_1;
     assign w_29_46 = w_27_29[`int_32-1:0];
     assign w_29_30 = w_27_29[`int_32-1:0];
     assign w_30_31 = w_29_30 <= w_3_30;
     assign w_32_51 = w_26_32;
     assign w_33_53 = w_25_33;
     assign w_35_37 = w_52_35;
     assign w_36_40 = w_54_36;
     assign w_37_40 = w_35_37 + w_38_37;
     assign w_37_39 = w_35_37 + w_38_37;
     assign w_38_37 = i32_5;
     assign w_40_41 = w_36_40 + w_37_40;
     assign w_43_49 = (w_44_43 == w_45_43) ?  1 : 0;
     assign w_43_48 = (w_44_43 == w_45_43) ?  1 : 0;
     assign w_43_47 = (w_44_43 == w_45_43) ?  1 : 0;
     assign w_43_46 = (w_44_43 == w_45_43) ?  1 : 0;
     assign w_44_43 = CONTROL_REGISTER;
     assign w_45_43 = i64_56266160;
     assign w_46_10 = (w_43_46 ) ?  w_7_46 : w_29_46;
     assign w_47_11 = (w_43_47 ) ?  w_3_47 : w_26_47;
     assign w_48_12 = (w_43_48 ) ?  w_3_48 : w_25_48;
     assign w_49_13 = (w_43_49 ) ?  w_50_49 : w_27_49;
     assign w_50_49 = i64_0;
     assign w_51_52 = (w_17_51 ) ?  w_32_51 : w_20_51;
     assign w_52_35 = (w_6_52 ) ?  w_51_52 : w_3_52;
     assign w_53_54 = (w_17_53 ) ?  w_33_53 : w_21_53;
     assign w_54_36 = (w_6_54 ) ?  w_53_54 : w_3_54;
     assign out_0 = i64_56266160;

     qram qr(
     w_store_val_mem_in_1,
     w_store_val_mem_in_2,
     w_load_mem_in_1,
     w_load_mem_in_2,
     w_load_mem_in_3,
     w_load_mem_in_4,
     w_store_1,
     w_store_2,
     clk,
     load_val_0,
     load_val_16,
     load_val_23,
     );

     //wire the memory multiplexer
     mux mx(
     store_val_4,
     store_val_39,
     store_val_41,
     store_add_4,
     store_add_39,
     store_add_41,
     load_add_0,
     load_add_16,
     load_add_23,
     w_load_mem_in_1,
     w_load_mem_in_2,
     w_load_mem_in_3,
     w_load_mem_in_4,
     w_store_val_mem_in_1,
     w_store_val_mem_in_2,
     w_store_1,
     w_store_2,
     state,
     clk
     );

     //wire sequential part
     //control state
     always @(posedge clk ) begin 
          if(reset) state <= DEFAULT;
          else state <= next;
     case (state)

     //default
     DEFAULT: begin
          load_add_0 <= 0;
          store_add_4 <= 0;
          store_val_4 <= 0;
          load_add_16 <= 0;
          store_add_39 <= 0;
          store_val_39 <= 0;
          load_add_23 <= 0;
          store_add_41 <= 0;
          store_val_41 <= 0;
          reg_10 <= 0;
          reg_11 <= 0;
          reg_12 <= 0;
          reg_13 <= 0;
          next <= ENTRY;
     end//end default

     //entry
     ENTRY: begin
          load_add_0 <= w_1_0;
          store_add_4 <= w_2_4;
          store_val_4 <= w_5_4;
          load_add_16 <= {`int_10'b1};
          load_add_23 <= {`int_10'b1};
          next <= FOR_BODY;
     end// end entry

     //for body
     FOR_BODY: begin
     if(w_30_31) next <= FOR_END;
     else begin
          reg_10 <= w_46_10;
          reg_11 <= w_47_11;
          reg_12 <= w_48_12;
          reg_13 <= w_49_13;
          load_add_16 <= w_14_16;
          load_add_23 <= w_14_23;
          end     //end else
     end     //end for_body case

     //for end
     FOR_END: begin
          store_add_39 <= w_37_39;
          store_val_39 <= w_5_39;
          store_add_41 <= w_40_41;
          store_val_41 <= w_1_41;
          next <= FOR_END;
     end//end for_end

     endcase

     //end ssequensial part
     end

endmodule