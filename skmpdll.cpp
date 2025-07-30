
/*
My standard start using maingp as command line analyser
contains the entry G0() called by main
*/
#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_SECURE_NO_WARNINGS
//#define INCLUDE_DEBUGGING_CODE
//#define SK0FDEBUGGING
#include <utility>
#include <limits.h>

#include <stdlib.h>
#include <sys/timeb.h>
using namespace std;
//====================================
//#define ISBUIDON
// ________________________ select what has to be loaded in sk0__h.h
//#define SK0FDEBUGGING_EXT
#define SK0FLOOKUP
//#define SK0FSMALLBANDFUNCTIONS_EXT
#define SK0FFLOORS
// SK0FT128 contains include sk_bitfield.h
#define SK0FT128
// "obsolete combine cell-fix vv9
//#define SK0FZZ
#include "maingp_user.h"

//#include "maingp_cpp.h"
#include "sk0__h.h"
#include "sk0_loads_cpp.h"
#include "sk0__hext.h"
#include "bf128.h"
#include "bf128_cpp.h"

#include "UAS\TUA3X.h"
#include "UAS\TUA3X_128.h"
#include "UAS\TUA3X_2048.h"

#include "dllusers\u06skbf_user.h"
#include "dllusers\skbminlex_user.h"
#include "dllusers\u07genua_user.h"

T128 mask3x = { 0x07FFFFFF07FFFFFF, 0x0000000007FFFFFF };


typedef  struct {	char w[82];} PUZZLE;

//=============== entries  get min puzs first is from checked code  
extern "C" __declspec(dllexport) int SkMinP(BF128 p, int* sol,BF128 * Tpm,int maxtpm);
extern "C" __declspec(dllexport) int SkMinPchar(PUZZLE p,PUZZLE* Tpm, int maxtpm);
struct WW {// shared data 
	//   output mode buffer and size 
	BF128* tpm;// buffer  output BF128 mode 
	PUZZLE* tpmp;// buffer return puzzles
	int mode, maxout,ntmin;// 0 back is BF128  1 back is puzzle
	// entry puzzle and analysis
	BF128 puzbf, psingles, pnsingles, x, andw;
	BF128 myp, pout;
	int nclues, nsingles, notsingles;

	char  ze[82], wout[82], ws[82];
	char solc[82]; // grid in char mode
	int grid[81],// grid int mode
		nrv, // return from check valid number of UAs
		cell;// working cell
	TUA3X* mtua3x;// UAs table from UAgen
	TUA3X_128 tplus;// relay to shrink uas form "check not valid"

	GINT16 tc[40]; int ntc;// puzzle in GINT16 mode 
	//============ functons of the main process
	void Getpuz() {// from BF128
		BF128 x = puzbf;
		memset(ze, '.', 81); ze[81] = 0;
		while ((cell = x.getFirstCell()) >= 0) {
			x.Clear_c(cell);
			//cout << ntc << " c " << cell << " d " << grid[cell] + 1 << endl;
			ze[cell] = solc[cell] ;
		}

	}
	void GetTc16(BF128 x);
	void Get_wout_from_t16(int n);
	int CheckValid(BF128  x);
	void NotValid();
	int SeeIfMinimal();
	void AddBack(BF128& p);
	void BF128ToPuz(BF128& wp, char* pout);
	//================ Main process
	int ComMinP();
	int Expand();
}ww;
//=============================== entries of the DLL
int SkMinP(BF128 p, int* sol, BF128* tpm, int maxtpm) {
	ww.mode = 0; ww.maxout = maxtpm; ww.tpm = tpm;
	memcpy(ww.grid, sol, sizeof ww.grid);
	ww.puzbf = p;
	for (int i = 0; i < 81; i++) ww.solc[i] = ww.grid[i] + '1';
	ww.Getpuz();
	pzhe = SkbfGetZhePointer();
	return ww.ComMinP();
}
int SkMinPchar(PUZZLE p, PUZZLE* tpm, int maxtpm) {
	ww.mode = 1; ww.maxout = maxtpm; ww.tpmp = tpm;
	char* pu = p.w; 
	memcpy(ww.ze,pu,82);
	ww.puzbf.SetAll_0();
	for (int i = 0; i < 81; i++) if (pu[i] != '.') ww.puzbf.Set_c(i);
	// check the puzzle validity back 0 if not valid
	{
		if (pu[81]) return 0;
		if (strlen(pu) != 81)return 0;// must be string
		for (int i = 0; i < 81; i++) {
			char c = pu[i];
			if (c == '.')continue;// . as "no clue"
			if (c < '1' || c> '9') return 0;
		}
		if (SkbfCheckValidityQuick(pu) != 1) return 0;// puzzle must be valid
	}
	//return -1;
	pzhe = SkbfGetZhePointer();
	memcpy(ww.grid, pzhe->gsol, sizeof ww.grid);
	for (int i = 0; i < 81; i++) ww.solc[i] = ww.grid[i] + '1';
	return ww.ComMinP();
}
//========================== WW functions
void WW::GetTc16(BF128 x) {
	ntc = 0;
	while ((cell = x.getFirstCell()) >= 0) {
		x.Clear_c(cell);
		//cout << ntc << " c " << cell << " d " << grid[cell] + 1 << endl;
		tc[ntc++].u16 = cell | (grid[cell] << 8);
	}
}
void WW::Get_wout_from_t16(int n) {
	//wout[81] = 0; 
	memset(wout, '.', 81);
	for (int i = 0; i < n; i++) {
		int v = tc[i].u16, c = v & 0xff, d = v >> 8;
		wout[c] = d + '1';
	}
}
int WW::CheckValid(BF128  x) {
	GetTc16(x);
	nrv = SkbfCheckOkForSol(grid, tc, ntc, 0);
	return nrv;
}
void WW::NotValid() {// insert new UAs in expand uas table
	tplus.Init();// reduce first to  puzbf pattern 
	for (int i = 0; i < pzhe->npat; i++) {
		BF128 x = pzhe->tpat[i] & puzbf;
		tplus.Insert(x);
	}
	for (int i = 0; i < tplus.nua; i++) tu2048a.Append(tplus.tua[i]);
}
int WW::SeeIfMinimal() {
	BF128 singlesexpand = psingles;
	// forget if redundant clues
	for (int i = 0; i < tu2048a.nua; i++) {
		BF128 y = x & tu2048a.tua[i];
		if (y.Count96() == 1)singlesexpand |= y;
	}
	if (singlesexpand == x) return 1;// already minimum
	BF128 y = x - singlesexpand;// possible redundant clue
	while (1) {// try without one possible redundant clue
		int cell = y.getFirstCell();// always one
		BF128 z = x; z.Clear_c(cell);
		if (!CheckValid(z)) return 0;// not minmal stop at first
		NotValid();// store uas
		y.Clear_c(cell);
		if (y.isEmpty())break;
	}
	return 1;// it is minimal no clue to clear
}
void WW::AddBack(BF128& p) {
	if (ntmin >= maxout)return;// protect against overflow
	if (!mode) {// return a BF 128
		tpm[ntmin++] = p;
		return;
	}
	// load convert BF128 to puzzle  in the return table
	BF128ToPuz(p, tpmp[ntmin++].w);
}
void WW::BF128ToPuz(BF128& wp, char* pout) {// pout expected to be char[82]
	memset(pout, '.', 81);		pout[81] = 0;
	//return;
	BF128 x = wp;
	while ((cell = x.getFirstCell()) >= 0) {
		x.Clear_c(cell);	pout[cell] = solc[cell];
	}
}

//============ main process
int WW::ComMinP() {
	Sk07GetTua3x(&mtua3x);
	mtua3x->nua = 0;
	tu2048a.Init();
	ntmin = 0;
	// find all non removable singles minimal if all compulsory closed
	{
		psingles.SetAll_0();
		BF128 x = puzbf;
		int cell;
		while ((cell = x.getFirstCell()) >= 0) {
			char zw[82];	memcpy(zw, ze, 82);
			x.Clear_c(cell);
			zw[cell] = '.';
			if (SkbfCheckValidityQuick(zw) != 1)
				psingles.Set_c(cell);
		}
		if (puzbf == psingles) {//  ok, entry valid
			AddBack(puzbf);
			return ntmin;
		}
		pnsingles = puzbf - psingles;
		nclues = puzbf.Count96();
		nsingles = psingles.Count96();
		notsingles = nclues - nsingles;
	}
	// try direct if small number of non compulsory and enough clues
	if(notsingles<4 && nclues>=22){
		if(CheckValid(ww.psingles)) {// use UAs to expand max 3 digits 
			NotValid();// create first set of uas to expand
			return Expand();
		}
		else {// this is a valid using only singles
			AddBack(psingles);	return ntmin;	}
	}

	int tc[80], ntc; 	// find all non removable pairs
	{
		pnsingles = puzbf - psingles;
		BF128 x = pnsingles;
		ntc = x.Table3X27(tc);
		for (int i = 0; i < ntc - 1; i++) {
			char zw1[82];	memcpy(zw1, ze, 82);
			zw1[tc[i]] = '.';
			for (int j = i + 1; j < ntc; j++) {
				char zw2[82];	memcpy(zw2, zw1, 82);
				zw2[tc[j]] = '.';
				if (SkbfCheckValidityQuick(zw2) != 1) {
					BF128 w; w.SetAll_0();
					w.Set_c(tc[i]); w.Set_c(tc[j]);
					tu2048a.Append(w);//no subset superset here
				}
			}
		}
	}
	// try direct if no compulsory pair
	if ((!tu2048a.nua) && nsingles >= 20) {
		if (!CheckValid(ww.psingles)) {// this is a valid using only singles
			AddBack(psingles);	return ntmin;
		}
		else {
			for (int i = 0; i < pzhe->npat; i++) {
				BF128 w = pzhe->tpat[i] & puzbf;
				tu2048a.Insert(w);
			}

		}
	}
	// get start uas table and morph it to expand table
	SkGenUas3x(grid);// get start uas table
	{
		BF128* tt = mtua3x->tua; int ntt = mtua3x->nua;
		for (int i = 0; i < ntt; i++) {
			BF128 w = tt[i] & puzbf;
			if ((w & psingles).isNotEmpty())continue;
			//if (tu2048a.nua && w.Count96() < 3) continue;
			tu2048a.Insert(w);
		}
	}
	//return -1000* tu2048a.nua;
	if(tu2048a.nua>5) return Expand();
	// should only be singles plus one or 2 clues see if more uas
	{
		BF128 s = tu2048a.tua[0], x = s;
		int cell;
		while ((cell = x.getFirstCell()) >= 0) {
			x.Clear_c(cell);
			BF128 y = ww.psingles; y.Set_c(cell);
			if (!CheckValid(ww.psingles)) {// this is a valid using only singles
				AddBack(psingles);	s.Clear_c(cell);
			}
			else {
				for (int i = 0; i < pzhe->npat; i++) {
					BF128 w = pzhe->tpat[i] & puzbf;
					tu2048a.Insert(w);
				}
			}
		}
		if (s.isEmpty())return ntmin;
		tu2048a.Insert(s);// reinsert first ua less solved
	}

	return Expand();
}

struct SPOT {
	BF128 cur_ua, all_previous_cells, dead_cells, all_cells;
	uint32_t iua, ispot, cell;
	void Start() {
		iua = ispot = 0;
		all_previous_cells = ww.psingles;
		dead_cells = ww.psingles;
		cur_ua = tu2048a.tua[iua];
	}
	int GetNext() {
		cell = cur_ua.getFirstCell();
		if (cell < 0) return -1;
		cur_ua.Clear_c(cell);
		dead_cells.Set_c(cell);
		return cell;
	}
	int Netxua(TUA3X_2048& tu, int op = 0) {
		char ws[82]; ws[81] = 0;
		all_cells = all_previous_cells;
		all_cells.Set_c(cell);
		if (op)cout << all_cells.String3X(ws) << " all cells iua=" << iua << endl;

		for (int i = iua + 1; i < tu.nua; i++) {
			BF128 w = tu.tua[i];// w.bf.u32[3] = 0;
			if (op) {
				cout << w.String3X(ws) << " iua=" << i << endl;
			}
			if ((w & all_cells).isNotEmpty()) continue;
			return i;
		}
		return -1; // no more ua
	}
	void Copy(SPOT* o, int iuae, BF128 u) {
		*this = *o; ispot++; iua = iuae; cur_ua = u;
		all_previous_cells.Set_c(cell);// add last cell
	}
	void Dump() {
		cout << " s status  " << ispot << " " << cell << " " << iua << endl;
		char ws[82]; ws[81] = 0;
		cout << cur_ua.String3X(ws) << " cur_ua" << endl;
		cout << all_previous_cells.String3X(ws) << " all_previous_cells" << endl;
		cout << all_cells.String3X(ws) << " all_cells" << endl;
		cout << dead_cells.String3X(ws) << " dead_cells" << endl;


	}
}spt[40];

int WW::Expand() {
	char ws[82]; ws[81] = 0;
	TUA3X_2048& tu = tu2048a;
	spt[0].Start();
	SPOT* s, * sp;
	sp = spt;	s = spt + 1;	*s = *spt;
next:
	if ((cell = s->GetNext()) < 0) {// go back
		if (--s > spt) goto next;		return ntmin;
	}
	int iua = s->Netxua(tu);
	if (iua < 0) {// see if valid
		x = s->all_cells;
		if (CheckValid(x)) NotValid();
		else {// see if minimal
			if (SeeIfMinimal()) AddBack(x);
			goto next;
		}

	}
	// assign next in this ua with active cells
	BF128 y = (tu.tua[iua] & puzbf) - s->dead_cells;
	if (y.isNotEmpty()) { sp = s++;	s->Copy(sp, iua, y); }
	goto next;
}
