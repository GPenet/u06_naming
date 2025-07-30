

void  BF128::ClearDiag(int band, int stack) {
	// stack appears here as a band
	int tp[32], ntp=0;
	BitsInTable32(tp, ntp, band);
	for (int i = 0; i < ntp; i++) {
		int cell=tp[i], celld = C_transpose_d[cell + 27 * stack];
		Clear_c(celld);
	}
}
void BF128::ClearRow(int clear, int row) {
	for (int i = 0,bit=1; i < 9; i++,bit<<=1) {
		if(bit&clear)Clear_c(9 * row + i);
	}
}
void BF128::ClearCol(int clear, int col) {
	for (int i = 0, bit = 1; i < 9; i++, bit <<= 1) {
		if (bit&clear)Clear_c(9 * i + col);
	}
}
void BF128::Diag3x27(BF128 & r){
	bf.u64[0] = bf.u64[1] = 0;
	if (r.isEmpty()) return;
	for (int iband = 0; iband < 3; iband++){// build diagonal sym
		int tc[27], ntc=0;
		BitsInTable32(tc, ntc, r.bf.u32[iband], 27 * iband);
		for (int i = 0; i < ntc; i++) {
			int celld = C_transpose_d[tc[i]];
			Set_c(celld);
		}
	}
}

int BF128::Table3X27(int * r) {	
	int n = 0;
	BitsInTable32(r, n, bf.u32[0]);
	BitsInTable32(r, n, bf.u32[1],27);
	BitsInTable32(r, n, bf.u32[2], 54);
	return n;
}
int BF128::Table128(int * r) {
	int n = 0;
	BitsInTable64(r, n, bf.u64[0]);
	BitsInTable64(r, n, bf.u64[1], 64);
	return n;
}
int BF128::Table64_0(int * r) {
	int n = 0;
	BitsInTable64(r, n, bf.u64[0]);
	return n;
}
int BF128::Table64_1(int * r) {
	int n = 0;
	BitsInTable64(r, n, bf.u64[1], 64);
	return n;
}

char * BF128::String3X_Rev(char * ws){
	memset(ws, '.', 81); ws[81] = 0;
	uint32_t *bfw = bf.u32;
	unsigned int w = bfw[0];
	for (int j = 26; j >= 0; j--) if (w & (1 << j))
		ws[j] = '1';
	w = bfw[1];
	for (int j = 26; j >= 0; j--) if (w & (1 << j))
		ws[j + 27] = '1';
	w = bfw[2];
	for (int j = 26; j >= 0; j--) if (w & (1 << j))
		ws[j + 54] = '1';
	return ws;

}
char * BF128::String3X(char * ws){
	memset(ws, '.', 81); ws[81] = 0;
	for (int j = 0; j<81; j++) if (On(C_To128[j]))
		ws[j] = '1';
	return ws;
}
char * BF128::String128(char * ws){
	ws[128] = 0;
	for (int j = 0; j<128; j++)
		if (On(j))		ws[j] = '1';		else ws[j] = '.';
	return ws;

}
void BF128::PrintCells() {// assumed not empty checked by caller
	for (int ib = 0; ib < 3; ib++) {// three bands
		uint32_t v = bf.u32[ib];
		if (!v) continue;
		for (int irr = 0; irr < 3; irr++, v >>= 9) {
			int row = v & 0x1ff;
			if (!row)continue;
			cout << " R" << 3 * ib + irr + 1 << "C";
			for (int ic = 0, bit = 1; ic < 9; ic++, bit <<= 1)
				if (row&bit) cout << ic + 1;
		}
	}
	cout << endl;
}
#ifdef SK0FDEBUGGING
void BF128::Print(const char * lib) {
		cout << "BF128 status for " << lib << endl;
		uint64_t *w = bf.u64, id = 0;
		for (int i = 0; i < 2; i++, id += 64, w++)
			if (*w)cout << Char64out(*w) << " " << id << endl;

	}
void BF128::Print3(const char * lib) {
		cout << "BF128 3x27 status for " << lib << endl;
		for (int i = 0; i < 3; i++)
			cout << Char27out(bf.u32[i])  << endl;
	}
void BF128::PrintUa() { 
	for (int ib = 0; ib < 3; ib++) {
		register int b = bf.u32[ib];
		for (int irow = 0; irow < 3; irow++,b>>=9) {
			register int r = b & 0777, bit = 1;;
			for (int is = 0; is < 3; is++) {
				for (int i = 0; i < 3; i++,bit<<=1) {
					if (bit & r) cout << 'X';
					else cout<< '.';
				}
				if (is < 2)cout << " "; else cout << endl;
			}
		}
		cout << endl;
	}
	cout  << endl;
}
void BF128::Print81() {
	for (int ib = 0; ib < 3; ib++)
		cout << Char27out(bf.u32[ib]) << "_";
	cout << endl;
}
void BF128::Print9(int* g) {
	cout << "BF128 from a grid  " << endl;
	for (int i = 0, j = 0; i < 3; i++) {
		int v = bf.u32[i];
		for (int k = 0; k < 3; k++) {
			int vl = v & 0x1ff; v >>= 9;
			char zl[10]; zl[9] = 0;
			for (int l = 0, bit = 1; l < 9; l++, bit <<= 1, j++)
				if (vl & bit) zl[l] = g[j] + '1';
				else zl[l] = '.';
			cout << zl << endl;
		}
		cout << endl; 

	}
	
}
#endif