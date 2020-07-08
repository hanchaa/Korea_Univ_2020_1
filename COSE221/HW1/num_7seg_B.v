// 2019320016 차주한
module num_7seg_B (out, w, x, y, z);

input w;
input x;
input y;
input z;
output out;

assign out = (~y & ~z) | w | (x & ~z) | (x & ~y);

endmodule