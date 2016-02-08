// Edward Pan, edwardfpan@yahoo.com, Feb 1, 2016

// Rev 2, Feb 5, 2016

using namespace std;
#include <iostream>
#include <fstream>
#include "LatticePattern.h"
#define Print2File

bool PrintOut=false;
ofstream oft;

Clattice_List::Clattice_List()
{
	LattPtr Lattice_n = NULL;
	Lattice_o = NULL;
	Lattice_ot = NULL;
	Lattice_nt = NULL;
};
Clattice_List::~Clattice_List()
{
};

int Clattice_List::get_NoOfPattern()
{
	return NoOfPattern;
};

int Clattice_List::LatticeGeneration(int SquareSize)
{
	int n, i, j;
	static int SqSize_n, SqSize_o;
	BYTE **Pattern_o;

	PrintOut=false;

//	strcatfilename, (char)
#ifdef Print2File
	char filename[80], filename1[80], buff[80];
	strcpy(filename,"Lattice_");
	sprintf(buff, "%d",SquareSize);
	strcat(filename,buff);
	strcat(filename,".txt");
	
	oft.open(filename);
	if (NULL == oft)
	{
		cout << "Can't open file << filename" << endl; ;
		exit(-1);
	}
#endif

	for (n=1; n<=SquareSize; n++)
	{
		if (n == SquareSize)
			PrintOut = true;		
		
		if (n ==1)
		{
			NoOfPattern = 1;
			SqSize_n = 1;
			SqSize_o = 1;
			BYTE **NewPatt;

			NewPatt = Memory2D(SqSize_n);
			NewPatt[0][0] = 1;

			Lattice_n = Lattice_Create(NewPatt, SqSize_n);
			Lattice_Copy(&Lattice_o, Lattice_n);

			if (PrintOut) // printf out
			{
				cout << endl << "#" << endl;
#ifdef Print2File
				oft << "Lattice Pattern 1: " << endl << "#" << endl;
#endif
			}
		}
		else // n >1
		{
			cout << "Calculating" << n << "x" << n << "pattern..." << endl;
			SqSize_n = Lattice_o->SqSize + 1;
			NoOfPattern = 0;

			// Pattern memory allocation
			Pattern_o = Memory2D(Lattice_o->SqSize);
			Lattice_ot = Lattice_o;
			while ( NULL != Lattice_ot)
			{

				for (i=0; i<Lattice_o->SqSize; i++)
					for (j=0; j<Lattice_o->SqSize; j++)
					{
						Pattern_o[i][j] = Lattice_ot->Pattern[i][j];
					}
				LatticeGrow(Pattern_o, Lattice_o->SqSize);
				Lattice_ot = Lattice_ot->next;
			} 

			// free Pattern memory
			for (i=0; i<SqSize_o; i++)
			{
				delete Pattern_o[i];
			}
			delete Pattern_o;
		}  

		// delete old
		Lattice_Delete(Lattice_o);
		// old = new;
		Lattice_Copy(&Lattice_o, Lattice_n); //Size is new size
	} 

#ifdef Print2File
	oft << "\nThe total number of blocks for Square " << Lattice_o->SqSize << "x" << Lattice_o->SqSize << " is " << get_NoOfPattern() << endl;
	oft.close();
#endif
	// delete new
	Lattice_Delete(Lattice_n);
	return 0;
}

void Clattice_List::LatticeGrow(BYTE **Pattern_o, int SqSize_o)
{
	int ii, jj;
	int i,j;
	int SqSize_n = SqSize_o +1;
	BYTE **NewPatt;
	BYTE **Pattern_n;
	NewPatt=Memory2D(SqSize_n);
	Pattern_n=Memory2D(SqSize_n);

	// grow at bottom right
	for (i=0; i<SqSize_n; i++)
		for (j=0; j<SqSize_n; j++)
		{
			Pattern_n[i][j] = 0;
		}
	for (i=0; i<SqSize_o; i++)
		for (j=0; j<SqSize_o; j++)
		{
			Pattern_n[i][j] = Pattern_o[i][j];
		}

	for (ii=0; ii<SqSize_n; ii++)
		for (jj=0; jj<SqSize_n; jj++)
		{
			// reset NewLatt
			for (int i1=0; i1<SqSize_n; i1++)
			{
				for (int j1=0; j1<SqSize_n; j1++)
				{
					NewPatt[i1][j1] = Pattern_n[i1][j1];
				}
			}
			if (NewPatt[ii][jj] == 0)
			{
				if (CheckingNeighbors(NewPatt, ii, jj, SqSize_n))
				{
					NewPatt[ii][jj] = 1;
					CheckNewPattern(NewPatt, SqSize_n);
				}
			}
		}

	// grow at top left
	for (i=0; i<SqSize_n; i++)
		for (j=0; j<SqSize_n; j++)
		{
			Pattern_n[i][j] = 0;
		}
	for (i=0; i<SqSize_o; i++)
		for (j=0; j<SqSize_o; j++)
		{
			Pattern_n[i+1][j+1] = Pattern_o[i][j];  // old to be at bottom
		}

//	for (ii=0; ii<SqSize_n; ii++)
		ii = 0;
		for (jj=0; jj<SqSize_n; jj++)
		{
			// reset NewLatt
			for (int i1=0; i1<SqSize_n; i1++)
			{
				for (int j1=0; j1<SqSize_n; j1++)
				{
					NewPatt[i1][j1] = Pattern_n[i1][j1];
				}
			}

			if (NewPatt[ii][jj] == 0)
			{
				if (CheckingNeighbors(NewPatt, ii, jj, SqSize_n))
				{
					NewPatt[ii][jj] = 1;

					// This time, need to move the pattern to top left if possible.
					Move2TopLeft(NewPatt, SqSize_n);
					CheckNewPattern(NewPatt, SqSize_n);
				}
			}
		}

//	for (ii=0; ii<SqSize_n; ii++)
		jj = 0;
		for (ii=1; ii<SqSize_n; ii++)
		{
			// reset NewLatt
			for (int i1=0; i1<SqSize_n; i1++)
			{
				for (int j1=0; j1<SqSize_n; j1++)
				{
					NewPatt[i1][j1] = Pattern_n[i1][j1];
				}
			}

			if (NewPatt[ii][jj] == 0)
			{
				if (CheckingNeighbors(NewPatt, ii, jj, SqSize_n))
				{
					NewPatt[ii][jj] = 1;

					// This time, need to move the pattern to top left if possible.
					Move2TopLeft(NewPatt, SqSize_n);
					CheckNewPattern(NewPatt, SqSize_n);
				}
			}
		}

		for (int m=0; m<SqSize_n; m++)
		{
			delete Pattern_n[m];
			delete NewPatt[m];
		}
		delete Pattern_n;
		delete NewPatt;
}


bool Clattice_List::CheckNewPattern(BYTE **Pattern, int SqSize)
{
	int i, j;
	LattPtr Lattice_nt;
	bool success = false;
	bool addition = false;

	if (NoOfPattern==0)
	{
		// Adding new Lattice
		if (NULL !=Lattice_n)
		{
			Lattice_Delete(Lattice_n);
			Lattice_n = NULL;
		}

		BYTE **NewPatt;
		NewPatt = Memory2D(SqSize);
		for (i=0; i<SqSize; i++)
		{
			for (j=0; j<SqSize; j++)
			{
				NewPatt[i][j]=Pattern[i][j];
			}
		}

		Lattice_n = Lattice_Create(NewPatt, SqSize);	
		if (PrintOut) 
		{
#ifdef Print2File
			oft << "Lattice Pattern " << NoOfPattern+1 << ": " << endl;
#endif
//			printf("Lattice Pattern %d: \n", NoOfPattern+1);
			for (i=0; i<SqSize; i++)
			{
				for (j=0; j<SqSize; j++)
				{
					if (Lattice_n->Pattern[i][j]==1)
					{
						cout << "#";
#ifdef Print2File
						oft << "#" ;
#endif
					}
					else
					{
						cout << " ";
#ifdef Print2File
						oft << "" ;
#endif
					}
				}
				cout << endl;
#ifdef Print2File
				oft << endl ;
#endif
			}

		}
		NoOfPattern++;
		success = true;
	}
	else // NoOfPattern > 0
	{
		Lattice_nt = Lattice_n;

		// Check if the Lattice is already there
		int Node_cnt = 0;
		addition = true;
		while (NULL !=Lattice_nt) // parsing all the Lattices in the list
		{
			if (IsPatternInList(Lattice_nt->Pattern, Lattice_nt->Size_h, Lattice_nt->Size_v, Pattern, SqSize))
			{
				addition = false;
			}
			Lattice_nt = Lattice_nt->next;
			Node_cnt++;
		}

		if (addition)
		{
			// Adding new Lattice
			BYTE **NewPatt;
			NewPatt = Memory2D(SqSize);
			for (i=0; i<SqSize; i++)
			{
				for (j=0; j<SqSize; j++)
				{
					NewPatt[i][j]=Pattern[i][j];
				}
			}
			Lattice_Add(Lattice_n, NewPatt);	
			if (PrintOut)
			{
#ifdef Print2File
				oft << "Lattice Pattern " << NoOfPattern+1 << ": " << endl;
#endif
				for (i=0; i<SqSize; i++)
				{
					for (j=0; j<SqSize; j++)
					{
						if (NewPatt[i][j]==1)
						{
							cout << "#";
#ifdef Print2File
							oft << "#" ;
#endif
						}
						else
						{
							cout << " ";
#ifdef Print2File
							oft << " " ;
#endif
						}
					}
					cout << "\n";
#ifdef Print2File
					oft << endl ;
#endif
				}
			}

			NoOfPattern++;
			success = true;
		}
	}

	return success;
};


bool Clattice_List::CheckingNeighbors(BYTE **Pattern, int i, int j, int SqSize)
{
	// Check naightbouring block
	if(i>0)
	{
		if (j>0)
		{
			if (Pattern[i-1][j-1] == 1)
			{
				return true;
			}
		}

		if (Pattern[i-1][j]== 1)
		{
			return true;
		}

		if (j<SqSize-1)
		{
			if(Pattern[i-1][j+1] == 1)
			{
				return true;
			}
		}
	}

	if (j>0)
	{
		if(Pattern[i][j-1] == 1)
		{
			return true;
		}
	}
	if (j<SqSize-1)
	{
		if (Pattern[i][j+1] == 1)
		{
			return true;
		}
	}

	if (i<SqSize-1)
	{
		if (j>0)
		{
			if (Pattern[i+1][j-1] == 1)
			{
				return true;
			}
		}
		if (Pattern[i+1][j] == 1)
		{
			return true;
		}
		if (j<SqSize-1)
		{
			if (Pattern[i+1][j+1] == 1)
			{
				return true;
			}
		}
	}
	return false;
}


LattPtr Clattice_List::Lattice_Create(BYTE **NewPatt, int SqSize) {

	
//	LattPtr head = (LattPtr)malloc(sizeof(Lattice_LL));
	LattPtr head = new Lattice_LL;
	int size_h, size_v;
	if (head == NULL) 
	{
		printf("can't allocate memory!\n");
		return NULL;
	}
	else 
	{
		head->next = NULL;
		head->SqSize = SqSize;
		head->Pattern = NewPatt;
		// Calculate Size_h, Size_v
		CalculateSize(NewPatt, SqSize, &size_h, &size_v);
		head->Size_h = size_h;
		head->Size_v = size_v;
		return head;
	}
}

int  Clattice_List::Lattice_Add(LattPtr head, BYTE **NewPatt) {
	int size_h, size_v;

	if (head == NULL)
	{
		exit(-1);
	}

//	LattPtr NewNode = (LattPtr)malloc(sizeof(Lattice_LL));
	LattPtr NewNode = new Lattice_LL;
	if (NewNode == NULL) 
	{
		cout << "can't allocate memory!" << endl;
		return -1;
	}
	
	NewNode->next = NULL;
	NewNode->SqSize = head->SqSize;
	NewNode->Pattern = NewPatt;
	// Calculate Size_h, Size_v
	CalculateSize(NewPatt, head->SqSize, &size_h, &size_v);
	NewNode->Size_h = size_h;
	NewNode->Size_v = size_v;

	while (head->next !=NULL)
	{
		head = head->next;
	}
	head->next = NewNode; 
	return 0;
}

BYTE** Clattice_List::Memory2D(int SqSize)
{
	BYTE **Temp;
//	Temp = (BYTE **)malloc(sizeof(BYTE *)*SqSize);
	Temp = new BYTE *[SqSize];
	if (Temp == NULL)
	{
		cout << "Memory allocation error!" << endl;
		exit(1);
	}
	for (int i=0; i<SqSize; i++)
	{
//		Temp[i] = (BYTE *)malloc(sizeof(BYTE)*SqSize);
		Temp[i] = new BYTE [SqSize];
		if (Temp[i] == NULL)
		{
			cout << "Memory allocation error!" << endl;
			exit(1);
		}
	}
	return Temp;
}

void Clattice_List::Lattice_Delete(LattPtr Lattice)
{
	LattPtr tmp;

	while (NULL != Lattice)
	{
		for (int i=0;i<Lattice->SqSize; i++)
			delete Lattice->Pattern[i];
		delete Lattice->Pattern;

		tmp = Lattice->next;
		delete Lattice;
		Lattice = tmp;
	}
}

void Clattice_List::Lattice_Copy(LattPtr *Lattice_o, LattPtr Lattice_n) // Size is new size
{

	BYTE **NewLattice;

	if (NULL != *Lattice_o)
	{
		*Lattice_o = NULL;
	}

	while (NULL != Lattice_n)
	{
		NewLattice = Memory2D(Lattice_n->SqSize);

		for (int i=0; i<Lattice_n->SqSize; i++)
		{
			memcpy(NewLattice[i], Lattice_n->Pattern[i], sizeof(BYTE)*Lattice_n->SqSize);
		}

		if (NULL == *Lattice_o)
		{
			*Lattice_o = Lattice_Create(NewLattice,Lattice_n->SqSize);
		}
		else
		{
			Lattice_Add(*Lattice_o, NewLattice);
		}
		Lattice_n = Lattice_n->next;
	}
}

void Clattice_List::CalculateSize(BYTE **Lattice, int SqSize, int *size_h, int *size_v)
{
	*size_h = 0;
	*size_v = 0;
	for (int i=0; i<SqSize; i++)
		for (int j=0; j<SqSize; j++)
		{
			if (Lattice[i][j] ==1)
			{
				if (*size_h < j)
					*size_h = j;
				if (*size_v < i)
					*size_v = i;
			}
		}
	*size_h += 1;
	*size_v += 1;
};

bool Clattice_List::IsPatternInList(BYTE **Lattice, int size_h1, int size_v1, BYTE **NewLatt,  int SqSize) // false = not equal; true = equal
{
	int i, size_h2, size_v2;

	CalculateSize(NewLatt, SqSize, &size_h2, &size_v2);

	//Check if the size are the same, including rotation
	if (!( ((size_h1 == size_h2) && (size_v1 == size_v2))			// Same size
	   ||((size_h1 == size_v2) && (size_v1 == size_h2)) ))		// Same size after rotation
	{
		return false;  // not same
	}

	if (size_h1 == size_h2) // original
	{
		for (i=0; i<4; i++)
		{
			if (IsSamePattern(Lattice, NewLatt, size_h1, size_v1, i))
			{
				return true;
			}
		}
		if (size_h1 == size_v2)  //square, check rotation
		{
			for (i=4; i<8; i++)
			{
				if (IsSamePattern(Lattice, NewLatt, size_h1, size_v1, i))
				{
					return true;
				}
			}
		}
	}
	else // rotate
	{
		for (i=4; i<8; i++)
		{
			if (IsSamePattern(Lattice, NewLatt, size_h1, size_v1, i)) // check rotation
			{
				return true;
			}
		}
	}
	return (false);
}


bool Clattice_List::IsSamePattern(BYTE **Lattice, BYTE **NewLatt, int size_h, int size_v, int MirroRotate)
{
	// Size is Lattice Lattice Size
	switch (MirroRotate)
	{
		case 0:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[i][j])
						return false;
				}
			return true;
		case 1:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[i][size_h-1-j])
						return false;
				}
			return true;
		case 2:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[size_v-1-i][j])
						return false;
				}
			return true;
		case 3:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[size_v-1-i][size_h-1-j])
						return false;
				}
			return true;
		case 4:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[j][i])
						return false;
				}
			return true;
		case 5:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[size_h-1-j][i])
						return false;
				}
			return true;
		case 6:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[j][size_v-1-i])
						return false;
				}
			return true;
		case 7:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Lattice[i][j] != NewLatt[size_h-1-j][size_v-1-i])
						return false;
				}
			return true;
		default:
			return false;
	}
};

void Clattice_List::Move2TopLeft(BYTE **Lattice, int SqSize)
{
	int Left, Top, i ,j;
	Left = 1;
	Top = 1;

	for (i=0; i<SqSize; i++)
	{
		if (Lattice[i][0] == 1)
		{
			Left = 0;
			break;
		}
	}
	for (j=0; j<SqSize; j++)
	{
		if (Lattice[0][j] == 1)
		{
			Top = 0;
			break;
		}
	}

	if (Left == 1)
	{
		for (i=0; i<SqSize; i++)
		{
			for (j=0; j<SqSize-1; j++)
			{
				Lattice[i][j] = Lattice[i][j+1];
			}
			Lattice[i][SqSize-1] = 0;
		}
	}

	if (Top == 1)
	{
		for (j=0; j<SqSize; j++)
		{
			for (i=0; i<SqSize-1; i++)
			{
				Lattice[i][j] = Lattice[i+1][j];
			}
			Lattice[SqSize-1][j] =0;
		}

	}
}

