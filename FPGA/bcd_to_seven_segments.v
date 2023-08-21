module bcd7segs (a, b, c, d, s0, s1, s2, s3, s4, s5, s6);
	input a, b, c, d;
	output s0, s1, s2, s3, s4, s5, s6;
	
	wire [0:20] T;
	wire [0:4] inv;
	
	not not0(inv[0], a);
	not not1(inv[1], b);
	not not2(inv[2], c);
	not not3(inv[3], d);
	
	and andAB(T[0], a, b);
	and andAC(T[1], a, c);
	and andAD(T[2], a, d);
	and andBC(T[3], b, c);
	and andBD(T[4], b, d);
	and andCD(T[5], c, d);
	
	//g = AC + AB +!A!B!C + BCD
	and and0(T[6], inv[0], inv[1], inv[2]);
	and and10(T[16], T[3], d);
	or or0(s0, T[0], T[1], T[16], T[6]);
	
	//f = AC + CD + AB + !BC + !A!BD
	and and1(T[7], inv[1], c);
	and and2(T[8], inv[0], inv[1], d);
	or or1(s1, T[1], T[5], T[0], T[7], T[8]);
	
	//e = D + AB + AC + B!C
	and and3(T[9], b, inv[2]);
	or or2(s2, d, T[0], T[1], T[9]);
	
	//d = AC + AB  + B!C!D +  BCD +!A!B!CD 
	and and7(T[13], b, inv[2], inv[3]);
	and and8(T[14], T[3], d);
	and and9(T[15], inv[0], inv[1], inv[2], d);
	or or3(s3, T[1], T[0], T[13], T[14], T[15]);
	
	//c = AC + AB + !BC!D
	and and4(T[10], inv[1], c, inv[3]);
	or or4(s4, T[1], T[0], T[10]);
	
	//b = AC + AB + B!CD + !BC!D
	and and11(T[17], b, inv[2], d);
	and and12(T[18], inv[1], c, inv[3]);
	or or5(s5, T[1], T[0], T[17], T[18]);
	
	//a = AC + AB + B!C!D + !A!B!CD
	and and5(T[11], b, inv[2], inv[3]);
	and and6(T[12], inv[0], inv[1], inv[2], d);
	or or6(s6, T[1], T[0], T[11], T[12]);
endmodule
