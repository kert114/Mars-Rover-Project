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
//wire [7:0] redprime, greenprime, blueprime;
//wire [7:0] hue, saturation, lightness, delta;
wire [7:0] hue, saturation, value, delta, max, min;

//assign redprime = red[7:0]/255;
//assign greenprime = green[7:0]/255;
//assign blueprime = blue[7:0]/255;
assign max = (red[7:0]>green[7:0]) ? ((red[7:0]>blue[7:0]) ? red[7:0] : blue[7:0]) : (green[7:0]>blue[7:0]) ? green[7:0] : blue[7:0];
assign min = (red[7:0]<green[7:0]) ? ((red[7:0]<blue[7:0]) ? red[7:0] : blue[7:0]) : (green[7:0]<blue[7:0]) ? green[7:0] : blue[7:0];
assign delta = (max[7:0]-min[7:0]);
// This was converting to HSL - we want HSV instead so see the algorithm below
//assign lightness = (max[7:0]+min[7:0])/2;
//assign saturation = delta[7:0]==8'h0 ? 8'h0 : 2*lightness[7:0]-1>8'h0 ? delta[7:0]/2*lightness[7:0] : delta[7:0]/(1-(2*lightness[7:0]-1));
//assign hue = delta[7:0]==8'h0 ? 0 : max[7:0]==red[7:0] ? 60*(green[7:0] - blue[7:0])/delta[7:0] : max[7:0]==green[7:0] ? 60*(((blue[7:0]-red[7:0])/delta[7:0])+2) : 60*(((red[7:0]-green[7:0])/delta[7:0])+4);

// HSV

assign hue = delta[7:0]==8'h0 ? 0 : max[7:0]==red[7:0] ? 60*(green[7:0] - blue[7:0])/delta[7:0] : max[7:0]==green[7:0] ? 60*(((blue[7:0]-red[7:0])/delta[7:0])+2) : 60*(((red[7:0]-green[7:0])/delta[7:0])+4);
assign saturation = max==0 ? 8'h0 : 100*delta[7:0]/max;
assign value = 100*max[7:0]/255;



// Detect coloured areas
wire red_detect;
assign red_detect = (hue>=0 && hue <20) && (saturation>55 && saturation <80) && (value>30 && value<75);
wire teal_detect;
assign teal_detect = (hue>100 && hue <155) && (saturation>20 && saturation<70) && (value>6 && value<77);
wire fuchsia_detect;
assign fuchsia_detect = ((hue>320 && hue <=359) || hue < 8) && (saturation>80) && (value>=75);
wire orange_detect;
assign orange_detect = (hue>=30 && hue<=70) && (saturation>55) && (value>=27);

// Find boundary of cursor box

// Highlight detected areas
wire [23:0] red_high;
wire [23:0] fuchsia_high;
wire [23:0] teal_high;
wire [23:0] orange_high;

assign grey = green[7:1] + red[7:2] + blue[7:2]; //Grey = green/2 + red/4 + blue/4
assign red_high  =  red_detect ? {8'hff, 8'h00, 8'h00} : {grey, grey, grey};
assign fuchsia_high  =  fuchsia_detect ? {8'hff, 8'h00, 8'hff} : {grey, grey, grey};
assign teal_high  =  teal_detect ? {8'h00, 8'h80, 8'h80} : {grey, grey, grey};
assign orange_high  =  orange_detect ? {8'hff, 8'ha5, 8'h00} : {grey, grey, grey};

// Show bounding box
wire [23:0] new_image;
wire bb_active;
assign bb_active = (x == r_left) | (x == r_right) | (x == f_left) | (x == f_right) | (x == t_left) | (x == t_right) | (x == o_left) | (x == o_right) ;
assign new_image = bb_active ? bb_col : red_detect ? red_high : teal_detect ? teal_high : fuchsia_detect ? fuchsia_high : orange_high;

// Switch output pixels depending on mode switch
// Don't modify the start-of-packet word - it's a packet discriptor
// Don't modify data in non-video packets
assign {red_out, green_out, blue_out} = (mode & ~sop & packet_video) ? new_image : {red,green,blue};

//Count valid pixels to tget the image coordinates. Reset and detect packet type on Start of Packet.
reg [10:0] x;
reg packet_video;
always@(posedge clk) begin
	if (sop) begin
		x <= 11'h0;
		packet_video <= (blue[3:0] == 3'h0);
	end
	else if (in_valid) begin
		if (x == IMAGE_W-1) begin
			x <= 11'h0;
		end
		else begin
			x <= x + 11'h1;
		end
	end
end




//Find first and last red pixels
reg [10:0] r_x_min, r_x_max;
always@(posedge clk) begin
	if (red_detect & in_valid) begin	//Update bounds when the pixel is red
		if (x < r_x_min) r_x_min <= x;
		if (x > r_x_max) r_x_max <= x;

	end
	if (sop & in_valid) begin	//Reset bounds on start of packet
		r_x_min <= IMAGE_W-11'h1;
		r_x_max <= 0;

	end
end

//Find first and last fuchsia pixels
reg [10:0] f_x_min, f_x_max;
always@(posedge clk) begin
	if (fuchsia_detect & in_valid) begin	//Update bounds when the pixel is red
		if (x < f_x_min) f_x_min <= x;
		if (x > f_x_max) f_x_max <= x;

	end
	if (sop & in_valid) begin	//Reset bounds on start of packet
		f_x_min <= IMAGE_W-11'h1;
		f_x_max <= 0;

	end
end


//Find first and last teal pixels
reg [10:0] t_x_min, t_x_max;
always@(posedge clk) begin
	if (teal_detect & in_valid) begin	//Update bounds when the pixel is red
		if (x < t_x_min) t_x_min <= x;
		if (x > t_x_max) t_x_max <= x;

	end
	if (sop & in_valid) begin	//Reset bounds on start of packet
		t_x_min <= IMAGE_W-11'h1;
		t_x_max <= 0;

	end
end


//Find first and last orange pixels
reg [10:0] o_x_min, o_x_max;
always@(posedge clk) begin
	if (orange_detect & in_valid) begin	//Update bounds when the pixel is red
		if (x < o_x_min) o_x_min <= x;
		if (x > o_x_max) o_x_max <= x;
	end
	if (sop & in_valid) begin	//Reset bounds on start of packet
		o_x_min <= IMAGE_W-11'h1;
		o_x_max <= 0;
	end
end


//Process bounding box at the end of the frame.
reg [1:0] msg_state;
reg [10:0] r_left, r_right;
reg [10:0] f_left, f_right;
reg [10:0] t_left, t_right;
reg [10:0] o_left, o_right;
reg [7:0] frame_count;
always@(posedge clk) begin
	if (eop & in_valid & packet_video) begin  //Ignore non-video packets
		
		//Latch edges for display overlay on next frame
		r_left <= r_x_min;
		r_right <= r_x_max;
		
		f_left <= f_x_min;
		f_right <= f_x_max;
		
		
		t_left <= t_x_min;
		t_right <= t_x_max;

		o_left <= o_x_min;
		o_right <= o_x_max;
		
		
		//Start message writer FSM once every MSG_INTERVAL frames, if there is room in the FIFO
		frame_count <= frame_count - 1;
		
		if (frame_count == 0 && msg_buf_size < MESSAGE_BUF_MAX - 3) begin
			msg_state <= 2'b01;
			frame_count <= MSG_INTERVAL-1;
		end
	end
	
	//Cycle through message writer states once started
	if (msg_state != 2'b00) msg_state <= msg_state + 4'b0001;

end
	
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
`define ORANGE_BOX_MSG_ID "OBB"

always@(*) begin	//Write words to FIFO as state machine advances
	case(msg_state)
		2'b00: begin
			msg_buf_in = 32'b0;
			msg_buf_wr = 1'b0;
			
		end
		2'b01 && red_detect: begin
			msg_buf_in = `RED_BOX_MSG_ID;	//Message ID red
			msg_buf_wr = 1'b1;
		end
		2'b01 && teal_detect: begin
			msg_buf_in = `TEAL_BOX_MSG_ID;	//Message ID teal
			msg_buf_wr = 1'b1;
		end
		2'b01 && fuchsia_detect: begin
			msg_buf_in = `FUCHSIA_BOX_MSG_ID;	//Message ID fuchsia
			msg_buf_wr = 1'b1;
		end
		2'b01 && orange_detect: begin
			msg_buf_in = `ORANGE_BOX_MSG_ID;	//Message ID orange
			msg_buf_wr = 1'b1;
		end
		//red
		2'b10 && red_detect: begin
			msg_buf_in = {5'b0, r_x_min};	//Top left coordinate red
			msg_buf_wr = 1'b1;
		end
		2'b11 && red_detect: begin	
			msg_buf_in = {5'b0, r_x_max}; //Bottom right coordinate red
			msg_buf_wr = 1'b1;
		end
		//fuchsia
		2'b10 && fuchsia_detect: begin
			msg_buf_in = {5'b0, f_x_min};	//Top left coordinate fuchsia
			msg_buf_wr = 1'b1;
		end
		2'b11 && fuchsia_detect: begin
			msg_buf_in = {5'b0, f_x_max}; //Bottom right coordinate fuchsia
			msg_buf_wr = 1'b1;
		end
		//teal
		2'b10 && teal_detect: begin
			msg_buf_in = {5'b0, t_x_min};	//Top left coordinate teal
			msg_buf_wr = 1'b1;
		end
		2'b11 && teal_detect: begin
			msg_buf_in = {5'b0, t_x_max}; //Bottom right coordinate teal
			msg_buf_wr = 1'b1;
		end
		//orange
		2'b10 && orange_detect: begin
			msg_buf_in = {5'b0, o_x_min};	//Top left coordinate orange
			msg_buf_wr = 1'b1;
		end
		2'b11 && orange_detect: begin
			msg_buf_in = {5'b0, o_x_max}; //Bottom right coordinate orange
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
`define READ_MSG    				1
`define READ_ID    				2
`define REG_BBCOL					3

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

