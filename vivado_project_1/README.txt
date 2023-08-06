如果没有vivado，不方便打开代码，我把主程序文件Counters.bit单独复制出来放在这了。


`default_nettype none

module Counters(
	input  wire [7:0]  hi_in,
	output wire [1:0]  hi_out,
	inout  wire [15:0] hi_inout,
	inout  wire        hi_aa,
	
	output wire        hi_muxsel,
	
	input  wire        clk1,//FIFO写入数据的时钟
	input wire reset,
	output wire full,//FIFO的写满标志
	output wire empty//FIFO的读空标志
	);

// Target interface bus:
wire        ti_clk;//USB传输数据的时钟，由OKHost给出
wire        rd_clk;
wire [30:0] ok1;
wire [16:0] ok2;
wire [17*5-1:0]  ok2x;

////////////////////FIFO_pipe/////////////////////

wire fifowrite;
wire fiforead;
wire [7:0] datain;
wire [7:0] dataout;
wire rst_n;
wire [15:0] wireout;

//Circuit behavior
assign rst_n = wireout[0];

//flag上升沿置位1，下降沿置为0
reg flag1;

 
reg clk1M;
reg [7:0] count=8'd0;
always @(posedge clk1)
    if(!rst_n)
    begin
        flag1<=1'b0;
    end
    else 
    begin if(count<=8'd23)
          begin 
            flag1<=1'b1;
            count<=count+8'b1;
          end
          else 
          flag1<=1'b0;
    end

fifo_generator_0 fifo (
  .rst(rst_n),        // input wire rst
  .wr_clk(clk1),  // input wire wr_clk
  .rd_clk(ti_clk),  // input wire rd_clk
  .din(count),        // input wire [15 : 0] din
  .wr_en(1'b1),    // input wire wr_en
  .rd_en(fiforead),    // input wire rd_en
  .dout(dataout),      // output wire [15 : 0] dout
  .full(full),      // output wire full
  .empty(empty)    // output wire empty
);
	
// FrontPanel module instantiations
okWireIn wire10(
     .ok1(ok1),
     .ep_addr(8'h10),
     .ep_dataout(wireout)
);
 
//okPipeIn pipe80(
//     .ok1(ok1),
//     .ok2(ok2x[0*17 +:17]),//索引从0*17开始，长度为17的子数组
//     .ep_addr(8'h80),
//     .ep_write(fifowrite),
//     .ep_dataout(datain)
//);
 
okPipeOut pipeA0(
     .ok1(ok1),
     .ok2(ok2x[1*17 +: 17]),
     .ep_addr(8'hA0),
     .ep_read(fiforead),
     .ep_datain(dataout)
);

/////////////////////////////////////////FIFO_pipe END///////////////////////////////////

assign hi_muxsel  = 1'b0;

////////////////////////////////////////////////////
//// Instantiate the okHost and connect endpoints.
wire [17*5-1:0]  ok2x;
okHost okHI(
	.hi_in(hi_in), .hi_out(hi_out), .hi_inout(hi_inout), .hi_aa(hi_aa), .ti_clk(ti_clk),
	.ok1(ok1), .ok2(ok2));

okWireOR # (.N(5)) wireOR (ok2, ok2x);



endmodule
