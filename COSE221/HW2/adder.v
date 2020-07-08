module cla_gen (P, G, C0, C);
parameter W = 4;

input [W-1:0] P, G;
input C0;
output [W:0] C;

assign C[0] = C0;

genvar i;
generate
    for (i = 0; i < W; i = i + 1)
    begin: cla_gen
        assign C[i + 1] = G[i] | (P[i] & C[i]);
    end
endgenerate

endmodule

module addsub_cla (A, B, S, C, M, V);
parameter W = 4;

input [W-1:0] A, B;
output [W-1:0] S;
output C;
input M;
output V;

wire [W-1:0] P, G;
wire [W:0] Carry; 

assign P[W-1:0] = A[W-1:0] ^ (B[W-1:0] ^ {W{M}});
assign G[W-1:0] = A[W-1:0] & (B[W-1:0] ^ {W{M}});
assign S[W-1:0] = P[W-1:0] ^ Carry[W-1:0];

cla_gen #(.W(W)) CLAGEN(.P(P), .G(G), .C0(M), .C(Carry));

assign C = Carry[W];
assign V = Carry[W] ^ Carry[W-1];

endmodule