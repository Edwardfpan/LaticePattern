// Edward Pan, edwardfpan@yahoo.com, Feb 1, 2016

// Rev 2, Feb 5, 2016

#ifndef __LATTICEPATTERN_H
#define __LATTICEPATTERN_H

typedef unsigned char BYTE;

typedef struct PLL
{
	int SqSize;
	int Size_h;
	int Size_v;
	BYTE **Pattern;
	PLL *next;
} Lattice_LL;
typedef Lattice_LL* LattPtr;

class Clattice_List
{
private:
	LattPtr Lattice_n;
	LattPtr Lattice_o;
	LattPtr Lattice_ot;
	LattPtr Lattice_nt;
	int NoOfPattern;
public:
	Clattice_List();
	~Clattice_List();
	int get_NoOfPattern();
	LattPtr Lattice_Create(BYTE **Lattice,int SqSize);
	int  Lattice_Add(LattPtr head, BYTE **Lattice);
	int LatticeGeneration(int n);
	void LatticeGrow(BYTE **Pattern, int n);
	bool CheckingNeighbors(BYTE **Pattern, int i, int j, int SqSize);
	bool CheckNewPattern(BYTE **Pattern, int SqSize);
	BYTE** Memory2D(int SqSize);
	void Lattice_Delete(LattPtr Lattice);
	void Lattice_Copy(LattPtr *Lattice_o, LattPtr Lattice_n); 
	void CalculateSize(BYTE **Lattice, int SqSize, int *size_h, int *size_v);
	bool IsPatternInList(BYTE **Lattice, int size_h1, int size_v1, BYTE **NewLatt,  int SqSize);
	bool IsSamePattern(BYTE **Lattice, BYTE **NewLatt, int size_h, int size_v, int MirroRotate);
	void Move2TopLeft(BYTE **Lattice, int SqSize);
};
#endif