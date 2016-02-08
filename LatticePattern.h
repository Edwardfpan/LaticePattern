// Edward Pan, edwardfpan@yahoo.com, Feb 1, 2016
// For XXX Inc.
// Rev 2, Feb 5, 2016

#ifndef __LATTICEPATTERN_H
#define __LATTICEPATTERN_H

typedef unsigned char BYTE;

typedef struct LL
{
	int SqSize;
	int Size_h;
	int Size_v;
//	int *Hash_h;
//	int *Hash_v;
	BYTE **Lattice;
	LL *next;
} Lattice_LL;

Lattice_LL* Lattice_Create(BYTE **Lattice,int SqSize);
int  Lattice_Add(Lattice_LL *head, int SqSize, BYTE **Lattice);
int LatticePattern(int n);
void LatticeGrow(BYTE **Buffer, int n);
bool CheckingNeighbors(BYTE **Buffer, int i, int j, int SqSize);
bool CheckNewLatt(BYTE **Buffer, int SqSize);
BYTE** Memory2D(int SqSize);
int* Memory1D(int SqSize);
void Lattice_Delete(Lattice_LL *Lattice, int SqSize);
void Lattice_Copy(Lattice_LL **Lattice_o, Lattice_LL *Lattice_n,  int SqSize); // Size is new size
void CalculateSize(BYTE **Lattice, int SqSize, int *size_h, int *size_v);
bool LatticeCompare(BYTE **Pattern, int size_h1, int size_v1, BYTE **NewPat,  int SqSize);
bool SamePatten(BYTE **Pattern, BYTE **NewPat, int size_h, int size_v, int MirroRotate);
void Move2TopLeft(BYTE **Lattice, int SqSize);
#endif