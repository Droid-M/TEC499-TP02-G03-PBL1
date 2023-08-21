module binario_bcd(a, b, c, d, e, f, g, s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11);
	input a, b, c, d, e, f, g;
	output s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
	
	wire [0:80] T;
	wire [0:7] inv;
	
	not not0(inv[0], a);
	not not1(inv[1], b);
	not not2(inv[2], c);
	not not3(inv[3], d);
	not not4(inv[4], e);
	not not5(inv[5], f);

	
	//s0 = 0; s1 = 0; s2 = 0
	
	//s3 = ABC + ABD + ABE
	and and0(T[0], a, b, c);
	and and1(T[1], a, b, d);
	and and2(T[2], a, b, e);
	or or0(s3, T[0], T[1], T[2]);
	
	//s4 = A!BC +  AB!C!D!E
	and and3(T[3], a, inv[1], c);
	and and4(T[4], a, b, inv[2], inv[3], inv[4]);
	or or1(s4, T[3], T[4]);
	
	//s5 = !ABD + !D +  A!B!C
	and and76(T[76], inv[0], b, d);
	and and74(T[74], a, inv[1], inv[2]);
	or or8(s5, T[76], T[74]);

	
	//s6 = A!B + ABCD + !A!BCD + !AB!C!D + !ABCDE + !A!BC!DE
	and and5(T[5], a, inv[1]);
	and and6(T[6], a, b, c, d);
	and and7(T[7], inv[0], inv[1], c, d,);
	and and8(T[8], inv[0], b, inv[2], inv[3]);
	and and9(T[9], inv[0], b, c, d, e);
	and and10(T[10], inv[0], inv[1], c, inv[3],e);
	or or2(s6, T[5], T[6], T[7], T[8], T[9], T[10]);
	
	//s7 = !A!B!CD!EF + !A!B!CDE!F + !A!BDEF + !A!BC!D!E + !AB!C!D + !ABCD!E + !ABC!D!EF + !ABC!DE + A!B!C!DEF + A!B!CD + A!BCD!EF + A!BCDE + AB!C!D!E + AB!CDEF + ABC!D
	and and11(T[11], inv[0], inv[1], inv[2], d, inv[4], f);
	and and75(T[75], inv[0], inv[1], inv[2], d, e, inv[5]);
	and and20(T[20], inv[0], inv[1], d, e ,f);
	and and13(T[13], inv[0], inv[2], c, inv[3], inv[4]);
	and and14(T[14], inv[0], b, inv[2], inv[3]);
	and and15(T[15], inv[0], b, c, d, inv[4]);
	and and16(T[16], inv[0], b, c, inv[3], inv[4], f);
	and and17(T[17], inv[0], b, c, inv[3], e);
	and and18(T[18], a, inv[1], inv[2], inv[3], e, f);
	and and19(T[19], a, inv[1], inv[2], d);
	and and21(T[21], a, inv[1], c, d, inv[4], f);
	and and22(T[22], a, inv[1], c, d, e);
	and and23(T[23], a, b, inv[2], inv[3], inv[4]);
	and and12(T[12], a, b, inv[2], d, e ,f);
	and and79(T[79], a, b, c, inv[3]);
	or or3(s7, T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19], T[21], T[22], T[23], T[20], T[75], T[79]);
	
//s8 = !A!B!CD!E!F + !A!BCDE!F + !A!BC!D!EF + !AB!C!DEF + !ABCD!EF + !ABC!D!E!F... 
//+ A!B!C!DE!F + A!B!CDEF + A!BCD!E!F + AB!C!D!EF + AB!CDE!F + ABC!DEF
	and and24(T[24], inv[0], inv[1], inv[2], d, inv[4], inv[5]);
	and and25(T[25], inv[0], inv[1], c, d, e, inv[5]);
	and and26(T[26], inv[0], inv[1], c, inv[3], inv[4], f);
	and and27(T[27], inv[0], b, inv[2], inv[3], e, f);
	and and28(T[28], inv[0], b, c, d, inv[4], f);
	and and29(T[29], inv[0], b, c, inv[3], inv[4], inv[5]);
	and and30(T[30], a, inv[1], inv[2], inv[3], e, inv[5]);
	and and31(T[31], a, inv[1], inv[2], d, e, f);
	and and32(T[32], a, inv[1], c, d, inv[4], inv[5]);
	and and33(T[33], a, b, inv[2], inv[3], inv[4], f);
	and and34(T[34], a, b, inv[2], d, e, inv[5]);
	and and35(T[35], a, b, c, inv[3], e, f);
	or or4(s8, T[24], T[25], T[26], T[27], T[28], T[29], T[30], T[31], T[32], T[33], T[34], T[35]);

//s9 = !A!B!C!DE + !A!B!CEF + !A!BCD!E + !A!BC!E!F + !AB!C!D + !AB!CDE + !ABCD!E!F... 
//+ !ABC!DEF + A!B!C!D!E + A!B!CD + A!BCEF + A!BC!DE + AB!C!E!F + AB!CD!E + ABCDE + ABC!D
	and and36(T[36], inv[0], inv[1], inv[2], inv[3], e);
	and and37(T[37], inv[0], inv[1], inv[2], e, f);
	and and38(T[38], inv[0], inv[1], c, d, inv[4]);
	and and39(T[39], inv[0], inv[1], c, inv[4], inv[5]);
	and and40(T[40], inv[0], b, inv[2], inv[3]);
	and and41(T[41], inv[0], b, inv[2], d, e);
	and and42(T[42], inv[0], b, c, d, inv[4], inv[5]);
	and and43(T[43], inv[0], b, c, inv[3], e, f);
	and and44(T[44], a, inv[1], inv[2], inv[3], inv[4]);
	and and45(T[45], a, inv[1], inv[2], d);
	and and46(T[46], a, inv[1], c, e, f);
	and and47(T[47], a, inv[1], c, inv[3], e);
	and and48(T[48], a, b, inv[2], d, inv[4]);
	and and49(T[49], a, b, c, d, e);
	and and50(T[50], a, b, c, inv[3]);
	or or5(s9, T[36], T[37], T[38], T[39], T[40], T[41], T[42], T[43], T[44], T[45], T[46], T[47], T[48], T[49], T[50]);
	
//s10 = !A!B!C!D!EF + !A!B!C!DEF + !A!B!CDE!F + !A!BCD!EF +  !A!BC!D + !AB!C!D!E!F + !AB!C!DE!F + !AB!CD!EF  + !AB!CD!EF + !ABCD + !ABC!DE!F + A!B!C!D!EF + A!B!CD!E!F + A!BDE!F 
//+ A!BC!D!EF + A!BC!DEF + AB!C!D!E!F + AB!C!DEF + ABD!EF + ABCDEF + ABC!D!E!F + ABC!DE!F
	and and51(T[51], inv[0], inv[1], inv[2], inv[3], inv[4], f);
	and and52(T[52], inv[0], inv[1], inv[2], inv[3], e, f);
	and and53(T[53], inv[0], inv[1], inv[2], d, e, inv[5]);
	and and54(T[54], inv[0], inv[1], c, d, inv[4], f);
	and and55(T[55], inv[0], inv[1], c, inv[3]);
	and and56(T[56], inv[0], b, inv[2], inv[3], inv[4], inv[5]);
	and and57(T[57], inv[0], b, inv[2], inv[3], e, inv[5]);
	and and58(T[58], inv[0], b, inv[2], d, inv[4], inv[5]);
	and and77(T[71], inv[0], b, inv[2], d, inv[4], f);
	and and59(T[59], inv[0], b, c, d);
	and and60(T[60], inv[0], b, c, inv[3], e, inv[5]);
	and and61(T[61], a, inv[1], inv[2], inv[3], inv[4], f);
	and and62(T[62], a, inv[1], inv[2], d, inv[4], inv[5]);
	and and63(T[63], a, inv[1], d, e, inv[5]);
	and and64(T[64], a, inv[1], c, inv[3], inv[4], f);
	and and65(T[65], a, inv[1], c, inv[3], e, f);
	and and66(T[66], a, b, inv[2], inv[3], inv[4], inv[5]);
	and and78(T[78], a, b, inv[2], inv[3], e, f);
	and and67(T[67], a, b, d, inv[4], f);
	and and68(T[68], a, b, c, d, e, f);
	and and69(T[69], a, b, c, inv[3], inv[4], inv[5]);
	and and70(T[70], a, b, c, inv[3], e, inv[5]);
	or or6(s10, T[51], T[52], T[53], T[54], T[55], T[56], T[57], T[58], T[59], T[60], T[61], T[62], T[63], T[64], T[65], T[66], T[67], T[68], T[69], T[70], T[71], T[78]);
	
//s11 = !EG + EG
	and and72(T[72], inv[4], g);
	and and73(T[73], e, g);
	or or7(s11, T[72], T[73]);
	
endmodule
	
	
	
