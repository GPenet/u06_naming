
/*
My standard start using maingp as command line analyser
contains the entry G0() called by main
*/
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <sys/timeb.h>
using namespace std;
// nothing to do here
#include "maingp_cpp.h"
#include "sk0\sk0_cpp_lookup.h"
#include "dllusers\u05skvcar_user.h"
#include "dllusers\u06skbf_user.h"
#include "dllusers\skgrid_minlex_user.h"



/* ascii values
0-9		48-57		0-9
A-Z		65-90		10-35
a-z		97-122		36-61
{}    123-125       62-63
*/

const char* bit6 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}";
//                  1234567890123456789012345678901234567890123456789012345678901234
void PuzzleInBitField(char* puz, char* d) {
	// d is 14*6=84 bits in 14 char
	for (int i = 0, k = 0; i < 14; i++) {
		register int v = 0;
		for (int j = 0; j < 6; j++,k++) {
			if (puz[k] != '.')v |= 1 << j;
			d[i] = bit6[v];
			if(k==80) break;
		}
	}
}
int Get6Bits(int i, char* bf14) {
	register int v = bf14[i];
	if (v >= 97 && v <= 123)v -= 61; // a z (
	else if (v >= 65 && v <= 90)v -= 55; // A Z
	else if (v >= 48 && v <= 57)v -= 48; // 0-9
	else if (v == 125) v = 63; // ) 
	else v = 0; // not valid put to empty
	return v;
}
void Expand14to81(char* puz, char* bf14, char * sol) {
	int k = 0;
	for (int i = 0, k = 0; i < 14; i++) {// 6 bits to expand
		register int v = Get6Bits(i,bf14); 
		int nd = (i == 13) ? 3 : 6;
		for (int j = 0; j < nd; j++,v>>=1,k++) {
			if (v & 1)puz[k] = sol[k] ;
			else puz[k] = '.';
		}
	}
}


//============================================================================================ processes
void C0() {
	cout << " C0_ find canonical for a file of sudokus sgo.vx[5]= " << sgo.vx[5] << endl;
	char* ze = finput.ze, zem[82], zes[200], zem2[82]; zem[81] = 0; zem2[81] = 0;
	int smin[81];
	ptpgc = SkbsSetModeWithAutos();
	pzhe = SkbfGetZhePointer();
	int* vv = pzhe->gsol;
	if (SkvcatSetModeGetVCDESK(2, &pvcdesc)) { cout << " set mode failed" << endl;		return; }
	while (finput.GetLigne()) {
		if (strlen(ze) < 81)continue;// no empty line
		memcpy(zes, ze, 200);// keep ze for output
		// stop if the entry if far from a valid sudoku
		int ncell = 0;
		for (int i = 0; i < 81; i++) {
			char c = ze[i];
			if (c == '.') continue;
			if (c > '0' && c <= '9') { ncell++; continue; }
			cout << ze << "not valid format" << endl;
			return; 
		}
		if (ncell < 18) { cout << ze << "missing clues stop" << endl; return; }
		if (SkbfCheckValidityQuick(ze) != 1) { cout << ze << " invalid puzzle " << endl; return; }
		SkbsGetMin(vv, smin);
		uint64_t r = SkvcatGetRankFromSolMin(smin);
		cout << "r=" << r << endl;
		GRIDPERM wgperm;
		char* pbase = ptpgc->t[0];
		wgperm.Import19(vv, pbase);
		wgperm.MorphPuzzle(ze, zem);
		for (int i = 1; i < ptpgc->nt; i++) {// check auto morph get the smallest
			wgperm.Import19(vv, ptpgc->t[i]);
			wgperm.MorphPuzzle(ze, zem2);
			register int x = 0;
			for (; x < 81; x++)if (zem[x] != zem2[x])break;
			if (x < 81 && zem2[x] == '.') memcpy(zem, zem2, 81);
		}
		char zbit[15]; zbit[14] = 0;
		PuzzleInBitField(zem, zbit);
		fout1.width(10);		fout1 << r;		fout1 << " " << zbit << endl;
		if (sgo.vx[4])
			fout2 << zem << ";" << r << ";" << zbit << endl;
	}
	cout << "end of file   " << endl;
}

void C1() {}
void C2() {
	cout << " C2_ convert r+b to 19"  << endl;
	char* ze = finput.ze; 
	while (finput.GetLigne()) {
		if (strlen(ze) < 25)continue;// no empty line
		int i = 0;
		if (ze[10] == ' ') i = 10;// fix length for r
		else for (; i < 15; i++)if (ze[i] == ';') break;
		if (i > 10) { cout << ze << "not expected format" << endl; return; }
		char* zeb = &ze[i + 1];
		int ll = (int)strlen(zeb);
		if (ll != 14) { cout << "not rigth length" << endl;		return; }
		ze[i] = 0;
		uint64_t rank = strtoll(ze, 0, 10);
		//assumed valid to pack
		char z19[20]; memset(z19, 0, 20);
		register uint64_t r = rank, r2;
		{
			r2 = (r >> 27) & 0x3f; z19[0] = bit6[r2];//  highest bits
			r2 = (r >> 21) & 0x3f; z19[1] = bit6[r2];
			r2 = (r >> 15) & 0x3f; z19[2] = bit6[r2];
			r2 = (r >> 9) & 0x3f; z19[3] = bit6[r2];
			r2 = (r >> 3) & 0x3f; z19[4] = bit6[r2];
		}
		register int rx = (int)r & 7;// last 3  bits
		for (int i = 0; i < 14; i++) {
			register int ry = Get6Bits(i, zeb);
			rx = (rx << 3) | ry & 7;
			z19[i + 5] = bit6[rx];
			rx = ry >> 3;
		}
		fout1 <<z19<<";" << ze << endl;
	}
	cout << "end of file   " << endl;
}
void C3() {
	cout << " C4_ convert 19 to r+bit" << sgo.vx[4] << endl;
	char* ze = finput.ze; 
	while (finput.GetLigne()) {
		if (strlen(ze) != 19) { cout << ze << "not expected format" << endl; return; }
		uint64_t rank = 0;
		for (int i = 0; i < 5; i++) {
			register int ry = Get6Bits(i, ze);
			rank = (rank <<= 6) | ry;
		}
		register int rx = Get6Bits(5, ze);// 3 bits rank
		rank = (rank <<= 3) | (rx >> 3);
		rx &= 7;// residual bits for the 14 bytes field
		char zb[15]; zb[14] = 0;
		for (int i = 6; i < 19; i++) {
			register int ry = Get6Bits(i, ze);
			rx = rx | ry & 070;
			zb[i - 6] = bit6[rx];
			rx = ry & 7;
		}
		zb[13] = bit6[rx];// last three bits
		if(sgo.vx[4])		fout1 << rank << ";" << zb <<ze<< endl;
		else {
			fout1.width(10);
			fout1 << rank;
			fout1 << " " << zb<<ze << endl;

		}
	}
	cout << "end of file   " << endl;
}

void C4() {// find sudoku out puz or puz;r;bitf  filter option <10 or <10.5
	cout << " C4_ find sudoku min lex from canonical  sgo.vx[4]= " << sgo.vx[4] << endl;
	char* ze = finput.ze; 
	ptpgc = SkbsSetModeWithAutos();
	pzhe = SkbfGetZhePointer();
	int* vv = pzhe->gsol;
	if (SkvcatSetModeGetVCDESK(2, &pvcdesc)) { cout << " set mode failed" << endl;		return; }
	while (finput.GetLigne()) {
		if (strlen(ze) < 25)continue;// no empty line
		int i = 0;
		if (ze[10] == ' ') i = 10;// fix length for r
		else for (; i < 15; i++)if (ze[i] == ';') break;
		if (i > 10) { cout << ze << "not expected format" << endl; return; }
		char* zeb = &ze[i + 1];
		int ll = (int)strlen(zeb);
		if (ll != 14) { cout << "not rigth length" << endl;		return; }
		ze[i] = 0;
		uint64_t rank = strtoll(ze, 0, 10);
		//assumed valid to expand
		char zout[82]; zout[81] = 0;
		char zsol[82]; zsol[81] = 0;
		if (SkvcatFinSolForRank(rank) < 0) {	cout<< ze << "rank false" << endl; return;	}
		else memcpy(zsol, pvcdesc->g.b1, 81);
		Expand14to81(zout, zeb, zsol);
		fout1 << zout;
		if (sgo.vx[4]) fout1 << ";" << rank << ";" << zeb;
		fout1 << endl;
	}
	cout << "end of file   " << endl;
}

void C5() {// work on skfr output first cut to first / and kill dot
	// entry must be here a puzzle in minimal morph
	cout << " C5_ skfr output -> puz+ER+ canonical name ER filter sgo.vx[4]= " << sgo.vx[4] << endl;
	char* ze = finput.ze,  zes[100]; zes[95] = 0; 
	ptpgc = SkbsSetModeWithAutos();
	pzhe = SkbfGetZhePointer();
	int* vv = pzhe->gsol;
	if (SkvcatSetModeGetVCDESK(2, &pvcdesc)) { cout << " set mode failed" << endl;		return; }
	while (finput.GetLigne()) {
		if (strlen(ze) < 95)continue;// mini "81"+ " ED=1.2/1.2/1.2"
		memcpy(zes, ze, 95);// keep ze for process 

		if (SkbfCheckValidityQuick(ze) != 1) {	cout << zes << " invalid puzzle " << endl;	continue;		}
		uint64_t r = SkvcatGetRankFromSolMin(vv);// we have a min entry
		if(!r) { cout << zes << " not min lex puzzle " << endl;	continue; }
		// move the skfr output to puy;ER   entry must have " ED=x.y/" or " ED)1x.y/"
		if (zes[82] != 'E' || zes[83] != 'D' || zes[84] != '=') {		cout << zes << " not skfr output " << endl;	return;		}
		int aig = 0;
		for (int i = 85; i < 91; i++) {
			if (zes[i] == '/') {
				zes[i - 2] = zes[i - 1];
				zes[i - 1] = 0;
				aig = i;// new point to add 
				break;
			}
		}
		if(!aig) { cout << zes << " not skfr slash " << endl;	return; }
		// kill " ED="
		zes[81] = ';';
		for (int i = 82; i < 86; i++) {
			zes[i] = zes[i + 3];
		}
		// filter on ER  10.0 or 10.5
		if (sgo.vx[4]) {
			if (!zes[84])continue; // below 100
			if (sgo.vx[4] == 2 && zes[83] == '0' && zes[84] < '5')continue; // below 105
		}


		char zbit[15]; zbit[14] = 0;
		PuzzleInBitField(zes, zbit);
		fout1 << zes << ";" << r << ";" << zbit << endl;
	}
	cout << "end of file   " << endl;
}
int cellsInGroup[27][9] =
{
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8},{ 9,10,11,12,13,14,15,16,17},{18,19,20,21,22,23,24,25,26},
	{27,28,29,30,31,32,33,34,35},{36,37,38,39,40,41,42,43,44},{45,46,47,48,49,50,51,52,53},
	{54,55,56,57,58,59,60,61,62},{63,64,65,66,67,68,69,70,71},{72,73,74,75,76,77,78,79,80},
	{ 0, 9,18,27,36,45,54,63,72},{ 1,10,19,28,37,46,55,64,73},{ 2,11,20,29,38,47,56,65,74},
	{ 3,12,21,30,39,48,57,66,75},{ 4,13,22,31,40,49,58,67,76},{ 5,14,23,32,41,50,59,68,77},
	{ 6,15,24,33,42,51,60,69,78},{ 7,16,25,34,43,52,61,70,79},{ 8,17,26,35,44,53,62,71,80},
	{ 0, 1, 2, 9,10,11,18,19,20},{ 3, 4, 5,12,13,14,21,22,23},{ 6, 7, 8,15,16,17,24,25,26},
	{27,28,29,36,37,38,45,46,47},{30,31,32,39,40,41,48,49,50},{33,34,35,42,43,44,51,52,53},
	{54,55,56,63,64,65,72,73,74},{57,58,59,66,67,68,75,76,77},{60,61,62,69,70,71,78,79,80}
}; 
int IsVali81z(char* z) {
	int g[81];
	for (int i = 0; i < 81; i++) {
		char c = z[i];
		if (c < '1' || c>'9') return 0;
		g[i] = c - '1';
	}
	for (int i1 = 0; i1 < 27; i1++) {// r,c,b
		register int r = 0, * tc = cellsInGroup[i1];
		for (int i2 = 0; i2 < 9; i2++) // cells
			r |= 1 << g[tc[i2]];
		if (r != 0777) return 0;
	}
	return 1;
}
int MovCheckGridz(char* z, int* g) {
	if (!IsVali81z(z)) return 1;
	for (int i = 0; i < 81; i++)
		g[i] = z[i] - '1';
	return 0;
}



void Go_0() {
	if (!sgo.finput_name) {
		cerr << "missing input file name"  << endl; return;
	}
	finput.open(sgo.finput_name);
	if (!finput.is_open()) {
		cerr << "error open file " << sgo.finput_name << endl;
		return;
	}
	// open  outputs files 1.txt
	if (!sgo.foutput_name) {
		cerr << "missing output file root"  << endl; return;
	} 
	char zn[200];
	strcpy(zn, sgo.foutput_name);
	int ll = (int)strlen(zn);
	strcpy(&zn[ll], "_file1.txt");
	fout1.open(zn);
	if (sgo.command == 0 && sgo.vx[4]) {
		strcpy(&zn[ll], "_file2.txt");
		fout2.open(zn);
	}

	cerr << "running command " << sgo.command << endl;
	switch (sgo.command) {
	case 0: C0(); break;// Naming puz
	case 2: C2(); break;// r+b to 19
	case 3: C3(); break;// 19 to r+b
	case 4: C4(); break;// r b / r;b to puz
	case 5: C5(); break;// skfr ed to p er r b

	}
	cerr << "go_0 return" << endl;
}

