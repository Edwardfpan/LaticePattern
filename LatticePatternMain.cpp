// Edward Pan, edwardfpan@yahoo.com, Feb 1, 2016
// Put N blocks into a lattice of N x N such that neighbouring rows or columns should have 
// at least a pair of blocks that share an edge or a vertex. Two arrangements are considered 
// the same if they turn into each other by rotating or if they are mirror or each other. 
// This a program to print out total number of allowable arrangements for a particular 
// number N, and print out all the allowable arrangements with each block represented by a ‘#’.  
//
// Rev 2, Feb 5, 2016

using namespace std;
#include <iostream>
#include "LatticePattern.h"
int NoOfPattern;

void main(int argc, char *argv[])
{
	int LatticeSize = -1;
	char str[80];
	Clattice_List *Latt_LL = new Clattice_List();

	while (LatticeSize != 0 )
	{
		cout << "Please enter the block size (any digits, '0' to exit): ";	
		cin >> str ;
		LatticeSize = atoi(str);
		if (LatticeSize >0) 
		{
			cout << "\n";
			Latt_LL->LatticeGeneration(LatticeSize);
			cout << "\nThe total number of blocks for Square " << LatticeSize << "x" << LatticeSize << " is " << Latt_LL->get_NoOfPattern() << endl;
		}
		else if (LatticeSize < 0) 
		{
			LatticeSize = 0;
			cout << "Must be positive digits!\n\n";
		}
		else
		{
			cout << "Thank you for using the software. Bye Bye!\n\n";
		}
	}
	Latt_LL->~Clattice_List();
}