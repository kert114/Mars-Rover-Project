module EEE_IMGPROC(
	// global clock & reset
	clk,
	reset_n,
	
	// mm slave
	s_chipselect,
	s_read,
	s_write,
	s_readdata,
	s_writedata,
	s_address,

	// stream sink
	sink_data,
	sink_valid,
	sink_ready,
	sink_sop,
	sink_eop,
	
	// streaming source
	source_data,
	source_valid,
	source_ready,
	source_sop,
	source_eop,
	
	// conduit
	mode
	
);


// global clock & reset
input	clk;
input	reset_n;

// mm slave
input				s_chipselect;
input				s_read;
input				s_write;
output	reg	[31:0]	s_readdata;
input	[31:0]		s_writedata;
input	[2:0]		s_address;


// streaming sink
input	[23:0]     	sink_data;
input				sink_valid;
output				sink_ready;
input				sink_sop;
input				sink_eop;

// streaming source
output	[23:0]		source_data;
output				source_valid;
input				source_ready;
output				source_sop;
output				source_eop;

// conduit export
input               mode;

////////////////////////////////////////////////////////////////////////
//
parameter IMAGE_W = 11'd640;
parameter IMAGE_H = 11'd480;
parameter MESSAGE_BUF_MAX = 256;
parameter MSG_INTERVAL = 6;
parameter BB_COL_DEFAULT = 24'h00ff00;


wire [7:0]   red, green, blue, grey;
wire [7:0]   red_out, green_out, blue_out;

wire         sop, eop, in_valid, out_ready;
////////////////////////////////////////////////////////////////////////

// Conversion for HSV from RGB
wire [7:0] hue, saturation, value, delta, max, min;

assign max = (red[7:0]>green[7:0]) ? ((red[7:0]>blue[7:0]) ? red[7:0] : blue[7:0]) : (green[7:0]>blue[7:0]) ? green[7:0] : blue[7:0];
assign min = (red[7:0]<green[7:0]) ? ((red[7:0]<blue[7:0]) ? red[7:0] : blue[7:0]) : (green[7:0]<blue[7:0]) ? green[7:0] : blue[7:0];
assign delta = (max[7:0]-min[7:0]);

// HSV

assign hue = delta[7:0]==8'h0 ? 0 : max[7:0]==red[7:0] ? 60*(green[7:0] - blue[7:0])/delta[7:0] : max[7:0]==green[7:0] ? 60*(((blue[7:0]-red[7:0])/delta[7:0])+2) : 60*(((red[7:0]-green[7:0])/delta[7:0])+4);
assign saturation = max==0 ? 8'h0 : 100*delta[7:0]/max;
assign value = 100*max[7:0]/255;

// Detect coloured areas
wire red_detect;
assign red_detect = ((hue>10 && hue<35) && (saturation>70) && (value>50));
wire teal_detect;
assign teal_detect = (hue>100 && hue<153 ) && (saturation>30) && (value>35);
wire fuchsia_detect;
assign fuchsia_detect = ((hue>357 & hue<20) && (saturation<85) && (value>20));
wire blue_detect;
assign blue_detect = ((hue>140 && hue<250) && (saturation<70) && (value>8 && value<72));
wire green_detect;
assign green_detect = ((hue>90 && hue<120) && (saturation>40 && saturation<77) && (value>13));
wire yellow_detect;
assign yellow_detect = ((hue>50 && hue<77) && (saturation<90) && (value>70));

// Find boundary of cursor box

//stores values of detected previous pixels
reg r1, r2, r3, r4, r5, r6, r7, r8, r9, r10;
reg f1, f2, f3, f4, f5, f6, f7, f8, f9, f10; 
reg t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
reg b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
reg g1, g2, g3, g4, g5, g6, g7, g8, g9, g10;
reg y1, y2, y3, y4, y5, y6, y7, y8, y9, y10;


initial begin
	r1 <= 0;	r2 <= 0;	r3 <= 0;	r4 <= 0;	r5 <= 0;	r6 <= 0;	r7 <= 0;	r8 <= 0;
	f1 <= 0;	f2 <= 0;	f3 <= 0;	f4 <= 0;	f5 <= 0;	f6 <= 0;	f7 <= 0;	f8 <= 0;
	t1 <= 0;	t2 <= 0;	t3 <= 0;	t4 <= 0;	t5 <= 0;	t6 <= 0;	t7 <= 0;	t8 <= 0;
	b1 <= 0;	b2 <= 0;	b3 <= 0;	b4 <= 0;	b5 <= 0;	b6 <= 0;	b7 <= 0;	b8 <= 0;
	g1 <= 0;	g2 <= 0;	g3 <= 0;	g4 <= 0;	g5 <= 0;	g6 <= 0;	g7 <= 0;	g8 <= 0;
	y1 <= 0;	y2 <= 0;	y3 <= 0;	y4 <= 0;	y5 <= 0;	y6 <= 0;	y7 <= 0;	y8 <= 0;
end

wire red_median, fuchsia_median, teal_median, blue_median, green_median, yellow_median;
always @(negedge clk) begin
	r8 <= r7;	r7 <= r6;	r6 <= r5;	r5 <= r4;	r4 <= r3;	r3 <= r2;	r2 <= r1;	r1 <= red_detect;
	f8 <= f7;	f7 <= f6;	f6 <= f5;	f5 <= f4;	f4 <= f3;	f3 <= f2;	f2 <= f1;	f1 <= fuchsia_detect;
	t8 <= t7;	t7 <= t6;	t6 <= t5;	t5 <= t4;	t4 <= t3;	t3 <= t2;	t2 <= t1;	t1 <= teal_detect;
	b8 <= b7;	b7 <= b6;	b6 <= b5;	b5 <= b4;	b4 <= b3;	b3 <= b2;	b2 <= b1;	b1 <= blue_detect;
	g8 <= g7;	g7 <= g6;	g6 <= g5;	g5 <= g4;	g4 <= g3;	g3 <= g2;	g2 <= g1;	g1 <= green_detect;
	y8 <= y7;	y7 <= y6;	y6 <= y5;	y5 <= y4;	y4 <= y3;	y3 <= y2;	y2 <= y1;	y1 <= yellow_detect;	
end


//since values are either only 1 or 0, just add the values together, if it's <6 then the median is 1 else the median is 0
function median;
	input p1;
	input p2;
	input p3;
	input p4;
	input p5;
	input p6;
	input p7;
	input p8;
	input p9;

	reg [3:0] px;

	px=p1+p2+p3+p4+p5+p6+p7+p8+p9;

	if (px>4) begin
		median=1'b1;
	end
	else begin
		median=1'b0;
	end

endfunction

assign red_median = median(red_detect, r1, r2, r3, r4, r5, r6, r7, r8);
assign fuchsia_median = median(fuchsia_detect, f1, f2, f3, f4, f5, f6, f7, f8);
assign teal_median = median(teal_detect, t1, t2, t3, t4, t5, t6, t7, t8);
assign blue_median = median(blue_detect, b1, b2, b3, b4, b5, b6, b7, b8);
assign green_median = median(green_detect, g1, g2, g3, g4, g5, g6, g7, g8);
assign yellow_median = median(yellow_detect, y1, y2, y3, y4, y5, y6, y7, y8);

// Highlight detected areas
wire [23:0] colour_high;
assign grey = green[7:1] + red[7:2] + blue[7:2]; //Grey = green/2 + red/4 + blue/4
assign colour_high  =  (red_detect  && red_median && r1 && r2 && r3) ? {8'hff, 8'h00, 8'h00} :  (fuchsia_detect && fuchsia_median && f1 && f2 && f3) ? {8'hff, 8'h00, 8'hff} : 
(teal_detect  && teal_median && t1 && t2 && t3) ? {8'h00, 8'h80, 8'h80} : (blue_detect && blue_median && b1 && b2 && b3) ? {8'h00, 8'h00, 8'hff} : 
(green_detect && green_median && g1 && g2 && g3) ? {8'h00, 8'hff, 8'h00} : (yellow_detect && yellow_median && y1 && y2 && y3) ? {8'hff, 8'hff, 8'h00} : {grey, grey, grey};

// Show bounding box
wire [23:0] new_image;
wire bb_active_red, bb_active_teal, bb_active_orange, bb_active_fuchsia, bb_active_blue, bb_active_green, bb_active_yellow;
assign bb_active_red = (x == r_left) | (x == r_right);
assign bb_active_fuchsia =(x == f_left) | (x == f_right);
assign bb_active_teal = (x == t_left) | (x == t_right);
assign bb_active_blue = (x == b_left) | (x == b_right);
assign bb_active_green = (x == g_left) | (x == g_right);
assign bb_active_yellow = (x == y_left) | (x == y_right);
assign new_image = bb_active_red ?{8'hff, 8'h00, 8'h00} : bb_active_teal ? {8'h00, 8'h80, 8'h80} : bb_active_fuchsia ? {8'hff, 8'h00, 8'hff} : 
bb_active_blue ? {8'h00, 8'h00, 8'hff} : bb_active_green ? {8'h00, 8'hff, 8'h00} : bb_active_yellow ? {8'hff, 8'hff, 8'h00} : colour_high;


// Switch output pixels depending on mode switch
// Don't modify the start-of-packet word - it's a packet discriptor
// Don't modify data in non-video packets
assign {red_out, green_out, blue_out} = (mode & ~sop & packet_video) ? new_image : {red,green,blue};

//Count valid pixels to tget the image coordinates. Reset and detect packet type on Start of Packet.
reg [10:0] x, y;
reg packet_video;
always@(posedge clk) begin
	if (sop) begin
		x <= 11'h0;
		y <= 11'h0;
		packet_video <= (blue[3:0] == 3'h0);
	end
	else if (in_valid) begin
		if (x == IMAGE_W-1) begin
			x <= 11'h0;
			y <= y + 11'h1;
		end
		else begin
			x <= x + 11'h1;
		end
	end
end



reg [10:0] r_x_min, r_x_max, f_x_min, f_x_max, t_x_min, t_x_max, b_x_max, b_x_min, g_x_min, g_x_max, y_x_min, y_x_max;
always@(posedge clk) begin
	//Find first and last red pixels
	if (red_detect && r1 && r2 && r3 && r4 && r5 && red_median && in_valid && (y>240)) begin	//Update bounds when the pixel is red
		if (x < r_x_min) r_x_min <= x;
		if (x > r_x_max) r_x_max <= x;
	end

	//Find first and last fuchsia pixels
	if (fuchsia_detect && f1 && f2 && f3 && f4 && f5 && fuchsia_median && in_valid && (y>240)) begin	//Update bounds when the pixel is fuchsia
		if (x < f_x_min) f_x_min <= x;
		if (x > f_x_max) f_x_max <= x;
	end

	//Find first and last teal pixels
	if (teal_detect && t1 && t2 && t3 && t4 && t5 && teal_median && in_valid && (y>240)) begin	//Update bounds when the pixel is teal
		if (x < t_x_min) t_x_min <= x;
		if (x > t_x_max) t_x_max <= x;
	end

	//Find first and last blue pixels
	if (blue_detect && b1 && b2 && b3 && b4 && b5 && blue_median && in_valid && (y>240)) begin	//Update bounds when the pixel is blue
		if (x < b_x_min) b_x_min <= x;
		if (x > b_x_max) b_x_max <= x;
	end

	//Find first and last green pixels
	if (green_detect && g1 && g2 && g3 && g4 && g5 && green_median && in_valid && (y>240)) begin	//Update bounds when the pixel is green
		if (x < g_x_min) g_x_min <= x;
		if (x > g_x_max) g_x_max <= x;
	end

	//Find first and last yellow pixels
	if (yellow_detect && y1 && y2 && y3 && y4 && y5 && yellow_median && in_valid && (y>240)) begin	//Update bounds when the pixel is yellow
		if (x < y_x_min) y_x_min <= x;
		if (x > y_x_max) y_x_max <= x;
	end

	if (sop & in_valid) begin	//Reset bounds on start of packet
		r_x_min <= IMAGE_W-11'h1;
		r_x_max <= 0;
		f_x_min <= IMAGE_W-11'h1;
		f_x_max <= 0;
		t_x_min <= IMAGE_W-11'h1;
		t_x_max <= 0;
		b_x_min <= IMAGE_W-11'h1;
		b_x_max <= 0;
		g_x_min <= IMAGE_W-11'h1;
		g_x_max <= 0;
		y_x_min <= IMAGE_W-11'h1;
		y_x_max <= 0;

	end
end


//Process bounding box at the end of the frame.
reg [3:0] msg_state;
reg [10:0] r_left, r_right, r_mid, r_width;
reg [10:0] f_left, f_right, f_mid, f_width;
reg [10:0] t_left, t_right, t_mid, t_width;
reg [10:0] b_left, b_right, b_mid, b_width;
reg [10:0] g_left, g_right, g_mid, g_width;
reg [10:0] y_left, y_right, y_mid, y_width;

reg [7:0] frame_count;
always@(posedge clk) begin
	if (eop & in_valid & packet_video) begin  //Ignore non-video packets
		
		//Latch edges for display overlay on next frame
		r_left <= r_x_min;
		r_right <= r_x_max;
		r_width <= (r_x_max-r_x_min);
		r_mid <= (r_x_max-(r_width>>1));
		
		f_left <= f_x_min;
		f_right <= f_x_max;
		f_width <= (f_x_max-f_x_min);
		f_mid <= (f_x_max-(f_width>>1));
		
		t_left <= t_x_min;
		t_right <= t_x_max;
		t_width <= (t_x_max-t_x_min);
		t_mid <= (t_x_max-(t_width>>1));

		b_left <= b_x_min;
		b_right <= b_x_max;
		b_width <= (b_x_max-b_x_min);
		b_mid <= (b_x_max-(b_width>>1));
		
		g_left <= g_x_min;
		g_right <= g_x_max;
		g_width <= (g_x_max-g_x_min);
		g_mid <= (g_x_max-(g_width>>1));

		y_left <= y_x_min;
		y_right <= y_x_max;
		y_width <= (y_x_max-y_x_min);
		y_mid <= (y_x_max-(y_width>>1));
		
		//Start message writer FSM once every MSG_INTERVAL frames, if there is room in the FIFO
		frame_count <= frame_count - 1;
		
		if (frame_count == 0 && msg_buf_size < MESSAGE_BUF_MAX - 3) begin
			msg_state <= 4'b0001;
			frame_count <= MSG_INTERVAL-1;
		end
	end
	
	//Cycle through message writer states once started
	if (msg_state != 4'b0000) begin
		if (msg_state == 4'b0110) begin
			msg_state <= 4'b0000;
		end
		else begin
			msg_state <= msg_state + 4'b0001;
		end
	end
end


wire [10:0] r_dist, f_dist, y_dist, b_dist, t_dist, g_dist;
assign r_dist = 2650/r_width;
assign f_dist = 2650/f_width;
assign y_dist = 2650/y_width;
assign b_dist = 2650/b_width;
assign t_dist = 2650/t_width;
assign g_dist = 2650/g_width;

wire [10:0] r_angle, f_angle, y_angle, b_angle, t_angle, g_angle;
assign r_angle = ((r_mid*70)/640)-35;
assign f_angle = ((f_mid*70)/640)-35;
assign y_angle = ((y_mid*70)/640)-35;
assign b_angle = ((b_mid*70)/640)-35;
assign t_angle = ((t_mid*70)/640)-35;
assign g_angle = ((g_mid*70)/640)-35;

	
//Generate output messages for CPU
reg [31:0] msg_buf_in; 
wire [31:0] msg_buf_out;
reg msg_buf_wr;
wire msg_buf_rd, msg_buf_flush;
wire [7:0] msg_buf_size;
wire msg_buf_empty;


`define RED_BOX_MSG_ID "RBB"
`define FUCHSIA_BOX_MSG_ID "FBB"
`define TEAL_BOX_MSG_ID "TBB"
`define BLUE_BOX_MSG_ID "BBB"
`define GREEN_BOX_MSG_ID "GBB"
`define YELLOW_BOX_MSG_ID "YBB"

always@(*) begin	//Write words to FIFO as state machine advances
	case(msg_state)
		4'b0000: begin
			msg_buf_in = 32'b0;
			msg_buf_wr = 1'b0;
		end
		//red
		4'b0001: begin
			msg_buf_in = {5'b0, r_dist, 5'b0, r_angle};
			msg_buf_wr = 1'b1;
		end
		//teal
		4'b0010: begin
			msg_buf_in = {5'b0, t_dist, 5'b0, t_angle};	
			msg_buf_wr = 1'b1;
		end
		//fuchsia
		4'b0011: begin
			msg_buf_in = {5'b0, f_dist, 5'b0, f_angle};
			msg_buf_wr = 1'b1;
		end
		//blue
		4'b0100: begin
			msg_buf_in = {5'b0, b_dist, 5'b0, b_angle};	
			msg_buf_wr = 1'b1;
		end
		//green
		4'b0101: begin
			msg_buf_in = {5'b0, g_dist, 5'b0, g_angle};	
			msg_buf_wr = 1'b1;
		end
		//yellow
		4'b0110: begin
			msg_buf_in = {5'b0, y_dist, 5'b0, y_angle};
			msg_buf_wr = 1'b1;
		end
	endcase
end


//Output message FIFO
MSG_FIFO	MSG_FIFO_inst (
	.clock (clk),
	.data (msg_buf_in),
	.rdreq (msg_buf_rd),
	.sclr (~reset_n | msg_buf_flush),
	.wrreq (msg_buf_wr),
	.q (msg_buf_out),
	.usedw (msg_buf_size),
	.empty (msg_buf_empty)
	);


//Streaming registers to buffer video signal
STREAM_REG #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red,green,blue,sop,eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red_out, green_out, blue_out, sop, eop})
);


/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define REG_STATUS    			0
`define READ_MSG    			1
`define READ_ID    				2
`define REG_BBCOL				3

//Status register bits
// 31:16 - unimplemented
// 15:8 - number of words in message buffer (read only)
// 7:5 - unused
// 4 - flush message buffer (write only - read as 0)
// 3:0 - unused


// Process write

reg  [7:0]   reg_status;
reg	[23:0]	bb_col;

always @ (posedge clk)
begin
	if (~reset_n)
	begin
		reg_status <= 8'b0;
		bb_col <= BB_COL_DEFAULT;
	end
	else begin
		if(s_chipselect & s_write) begin
		   if      (s_address == `REG_STATUS)	reg_status <= s_writedata[7:0];
		   if      (s_address == `REG_BBCOL)	bb_col <= s_writedata[23:0];
		end
	end
end


//Flush the message buffer if 1 is written to status register bit 4
assign msg_buf_flush = (s_chipselect & s_write & (s_address == `REG_STATUS) & s_writedata[4]);


// Process reads
reg read_d; //Store the read signal for correct updating of the message buffer

// Copy the requested word to the output port when there is a read.
always @ (posedge clk)
begin
   if (~reset_n) begin
	   s_readdata <= {32'b0};
		read_d <= 1'b0;
	end
	
	else if (s_chipselect & s_read) begin
		if   (s_address == `REG_STATUS) s_readdata <= {16'b0,msg_buf_size,reg_status};
		if   (s_address == `READ_MSG) s_readdata <= {msg_buf_out};
		if   (s_address == `READ_ID) s_readdata <= 32'h1234EEE2;
		if   (s_address == `REG_BBCOL) s_readdata <= {8'h0, bb_col};
	end
	
	read_d <= s_read;
end

//Fetch next word from message buffer after read from READ_MSG
assign msg_buf_rd = s_chipselect & s_read & ~read_d & ~msg_buf_empty & (s_address == `READ_MSG);
						


endmodule

