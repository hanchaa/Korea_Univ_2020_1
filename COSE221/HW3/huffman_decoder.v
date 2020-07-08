module huffman_decoder(
    input x,
    output [2:0] y,
    input clk,
    input reset
);

reg [3:0] present_state;
reg [3:0] next_state;

// `define state_t0 4'b0000
// `define state_A 4'b0001
// `define state_B 4'b0010
// `define state_C 4'b0011
// `define state_D 4'b0100
// `define state_E 4'b0101
// `define state_F 4'b0110
// `define state_t1 4'b0111
// `define state_t2 4'b1000
// `define state_t3 4'b1001
// `define state_t4 4'b1010

parameter state_t0 = 4'b0000, state_A = 4'b0001, state_B = 4'b0010, state_C = 4'b0011, state_D = 4'b0100, state_E = 4'b0101, state_F = 4'b0110, state_t1 = 4'b0111, state_t2 = 4'b1000, state_t3 = 4'b1001, state_t4 = 4'b1010;

always @(posedge reset or posedge clk)
begin
    if(reset == 1'b1)
        present_state <= state_t0;
    else
        present_state <= next_state;
end

always @(*)
begin
    case (present_state)
    state_t0: if(x) next_state = state_t1; else next_state = state_A;
    state_A: if(x) next_state = state_t1; else next_state = state_A;
    state_B: if(x) next_state = state_t1; else next_state = state_A;
    state_C: if(x) next_state = state_t1; else next_state = state_A;
    state_D: if(x) next_state = state_t1; else next_state = state_A;
    state_E: if(x) next_state = state_t1; else next_state = state_A;
    state_F: if(x) next_state = state_t1; else next_state = state_A;
    state_t1: if(x) next_state = state_t3; else next_state = state_t2;
    state_t2: if(x) next_state = state_B; else next_state = state_C;
    state_t3: if(x) next_state = state_D; else next_state = state_t4;
    state_t4: if(x) next_state = state_E; else next_state = state_F;
    default: next_state = 4'bxxxx;
    endcase
end

assign y[2] = (present_state[2] & ~present_state[1]) | (present_state[2] & ~present_state[0]);
assign y[1] = (~present_state[3] & ~present_state[2] & present_state[1]) | (~present_state[3] & present_state[1] & ~present_state[0]);
assign y[0] = (~present_state[3] & ~present_state[2] & present_state[0]) | (~present_state[3] & ~present_state[1] & present_state[0]);

endmodule