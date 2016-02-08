// Edward Pan, edwardfpan@yahoo.com, Feb 1, 2016
// For XXX Inc. 
// Rev 2, Feb 5, 2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LatticePattern.h"
#define Print2File
#define PrintPattern

int NoOfP_o, NoOfP_n;
static int SqSize_n, SqSize_o;
bool PrintOut=false;
Lattice_LL *Lattice_n, *Lattice_o, *Lattice_ot,  *Lattice_nt;
FILE *fp;
extern int LatticeSize;

int LatticePattern(int N)
{
	int n, i, j;

	BYTE **Buffer_o;


//	strcatfilename, (char)
#ifdef Print2File
	char filename[80], buff[80];
	strcpy(filename,"Lattice_");
	sprintf(buff, "%d",LatticeSize);
	strcat(filename,buff);
	strcat(filename,".txt");
	fp = fopen(filename, "w");
	if (NULL == fp)
	{
		printf("Can't open file %s\n",filename);
		exit(-1);
	}
#endif

	for (n=1; n<=N; n++)
	{
		if (n ==1)
		{

			NoOfP_n = 1;
			SqSize_n = 1;
			SqSize_o = 1;
			BYTE **Latt;

			Latt = Memory2D(SqSize_n);
			Latt[0][0] = 1;

			Lattice_n = Lattice_Create(Latt, SqSize_n);
			Lattice_Copy(&Lattice_o, Lattice_n, SqSize_n);

			if (n == N) // printf out
			{
//				printf("Lattice Pattern 1: \n#\n");
				printf("\n#\n");
#ifdef Print2File
				fprintf(fp,"Lattice Pattern 1: \n#\n");
#endif
			}

		}
		else // n >1
		{
			SqSize_n = Lattice_o->SqSize + 1;
			NoOfP_n = 0;

			// Buffer memory allocation

			Buffer_o = Memory2D(Lattice_o->SqSize);

			Lattice_ot = Lattice_o;

			while ( NULL != Lattice_ot)
			{
				if (n == N)
					PrintOut = true;


				for (i=0; i<Lattice_o->SqSize; i++)
					for (j=0; j<Lattice_o->SqSize; j++)
					{
						Buffer_o[i][j] = Lattice_ot->Lattice[i][j];
					}

				LatticeGrow(Buffer_o, Lattice_o->SqSize);

				Lattice_ot = Lattice_ot->next;
			} 

			// free Buffer memory
			for (i=0; i<SqSize_o; i++)
			{
				free(Buffer_o[i]);
			}
			free(Buffer_o);
		}  

		// delete old
		Lattice_Delete(Lattice_o, Lattice_o->SqSize);
		// old = new;
		Lattice_Copy(&Lattice_o, Lattice_n, Lattice_n->SqSize); //Size is new size
	} 

#ifdef Print2File
	fclose(fp);
#endif
	// delete new
	Lattice_Delete(Lattice_n, Lattice_n->SqSize);

	return 0;
}


void LatticeGrow(BYTE **Buffer_o, int SqSize_o)
{
	int ii, jj;
	int i,j;

	int SqSize_n = SqSize_o +1;
	BYTE **NewLatt;
	BYTE **Buffer_n;
	NewLatt=Memory2D(SqSize_n);
	Buffer_n=Memory2D(SqSize_n);


	// grow at bottom right
	for (i=0; i<SqSize_n; i++)
		for (j=0; j<SqSize_n; j++)
		{
			Buffer_n[i][j] = 0;
		}
	for (i=0; i<SqSize_o; i++)
		for (j=0; j<SqSize_o; j++)
		{
			Buffer_n[i][j] = Buffer_o[i][j];
		}

	for (ii=0; ii<SqSize_n; ii++)
		for (jj=0; jj<SqSize_n; jj++)
		{
			// reset NewLatt
			for (int i1=0; i1<SqSize_n; i1++)
			{
				for (int j1=0; j1<SqSize_n; j1++)
				{
					NewLatt[i1][j1] = Buffer_n[i1][j1];
				}
			}

			if (NewLatt[ii][jj] == 0)
			{
				if (CheckingNeighbors(NewLatt, ii, jj, SqSize_n))
				{
					NewLatt[ii][jj] = 1;
					if (!CheckNewLatt(NewLatt, SqSize_n))
					{
						continue;
						// delete NewLatt;
					}
				}
			}
		}

	// grow at top left
	for (i=0; i<SqSize_n; i++)
		for (j=0; j<SqSize_n; j++)
		{
			Buffer_n[i][j] = 0;
		}
	for (i=0; i<SqSize_o; i++)
		for (j=0; j<SqSize_o; j++)
		{
			Buffer_n[i+1][j+1] = Buffer_o[i][j];  // old to be at bottom
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
					NewLatt[i1][j1] = Buffer_n[i1][j1];
				}
			}

			if (NewLatt[ii][jj] == 0)
			{
				if (CheckingNeighbors(NewLatt, ii, jj, SqSize_n))
				{
					NewLatt[ii][jj] = 1;

					// This time, need to move the pattern to top left if possible.
					Move2TopLeft(NewLatt, SqSize_n);
					/////////////////////////////////////////////////////////////
					if (!CheckNewLatt(NewLatt, SqSize_n))
					{
						continue;
						// delete NewLatt;
					}
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
					NewLatt[i1][j1] = Buffer_n[i1][j1];
				}
			}

			if (NewLatt[ii][jj] == 0)
			{
				if (CheckingNeighbors(NewLatt, ii, jj, SqSize_n))
				{
					NewLatt[ii][jj] = 1;

					// This time, need to move the pattern to top left if possible.
					Move2TopLeft(NewLatt, SqSize_n);

					if (!CheckNewLatt(NewLatt, SqSize_n))
					{
						continue;
						// delete NewLatt;
					}
				}
			}
		}

		for (int m=0; m<SqSize_n; m++)
		{
			free(Buffer_n[m]);
			free(NewLatt[m]);
		}
		free(Buffer_n);
		free(NewLatt);
}


bool CheckNewLatt(BYTE **Buffer, int SqSize)
{
	int i, j;
	Lattice_LL *Lattice_nt;
	bool success = false;
	bool addition = false;

	if (NoOfP_n==0)
	{
		// Adding new Lattice
		if (NULL !=Lattice_n)
		{
			Lattice_Delete(Lattice_n, Lattice_n->SqSize);
			Lattice_n = NULL;
		}

		BYTE **Latt_NEW;
		Latt_NEW = Memory2D(SqSize);
		for (i=0; i<SqSize; i++)
		{
			for (j=0; j<SqSize; j++)
			{
				Latt_NEW[i][j]=Buffer[i][j];
			}
		}

		Lattice_n = Lattice_Create(Latt_NEW, SqSize);	


		if (PrintOut) 
		{
#ifdef PrintPattern
#ifdef Print2File
			fprintf(fp, "Lattice Pattern %d: \n", NoOfP_n+1);
#endif
//			printf("Lattice Pattern %d: \n", NoOfP_n+1);
			for (i=0; i<SqSize; i++)
			{
				for (j=0; j<SqSize; j++)
				{
					if (Lattice_n->Lattice[i][j]==1)
					{
						printf("#");
#ifdef Print2File
						fprintf(fp,"#");
#endif
					}
					else
					{
						printf(" ");
					}
				}
				printf("\n");
#ifdef Print2File
				fprintf(fp, "\n");
#endif
			}
#else
			for (i=0; i<SqSize; i++)
			{
				for (j=0; j<SqSize; j++)
				{
						printf("%d ", Lattice_n->Lattice[i][j]);
				}
				printf("\n");
			}
#endif
		}

		NoOfP_n++;
		success = true;
	}
	else // NoOfP_n > 0
	{
		Lattice_nt = Lattice_n;

		// Check if the Lattice is already there
		int Node_cnt = 0;
		addition = true;
		while (NULL !=Lattice_nt) // parsing all the nodes in the list
		{
			if (LatticeCompare(Lattice_nt->Lattice, Lattice_nt->Size_h, Lattice_nt->Size_v, Buffer, SqSize))
			{
				addition = false;
			}

			Lattice_nt = Lattice_nt->next;
			Node_cnt++;
		}

		if (addition)
		{
			// Adding new Lattice
			BYTE **Latt_NEW;
			Latt_NEW = Memory2D(SqSize);
			for (i=0; i<SqSize; i++)
			{
				for (j=0; j<SqSize; j++)
				{
					Latt_NEW[i][j]=Buffer[i][j];
				}
			}
			Lattice_Add(Lattice_n, SqSize, Latt_NEW);	
			if (PrintOut)
			{
#ifdef PrintPattern

//				printf("Lattice Pattern %d: \n", NoOfP_n+1);
#ifdef Print2File
				fprintf(fp,"Lattice Pattern %d: \n", NoOfP_n+1);
#endif
				for (i=0; i<SqSize; i++)
				{
					for (j=0; j<SqSize; j++)
					{
						if (Latt_NEW[i][j]==1)
						{
							printf("#");
#ifdef Print2File
							fprintf(fp,"#");
#endif
						}
						else
						{
							printf(" ");
#ifdef Print2File
							fprintf(fp," ");
#endif
						}
					}
					printf("\n");
#ifdef Print2File
					fprintf(fp,"\n");
#endif
				}
#else
				for (i=0; i<SqSize; i++)
				{
					for (j=0; j<SqSize; j++)
					{
							printf("%d ", Latt_NEW[i][j]);
					}
					printf("\n");
				}
#endif

			}

			NoOfP_n++;
			success = true;
		}

	}

	return success;
};


bool CheckingNeighbors(BYTE **Buffer, int i, int j, int SqSize)
{
	// Check naightbouring block
	if(i>0)
	{
		if (j>0)
		{
			if (Buffer[i-1][j-1] == 1)
			{
				return true;
			}
		}

		if (Buffer[i-1][j]== 1)
		{
			return true;
		}

		if (j<SqSize-1)
		{
			if(Buffer[i-1][j+1] == 1)
			{
				return true;
			}
		}
	}

	if (j>0)
	{
		if(Buffer[i][j-1] == 1)
		{
			return true;
		}
	}
	if (j<SqSize-1)
	{
		if (Buffer[i][j+1] == 1)
		{
			return true;
		}
	}

	if (i<SqSize-1)
	{
		if (j>0)
		{
			if (Buffer[i+1][j-1] == 1)
			{
				return true;
			}

		}
		if (Buffer[i+1][j] == 1)
		{
			return true;
		}
		if (j<SqSize-1)
		{
			if (Buffer[i+1][j+1] == 1)
			{
				return true;
			}
		}
	}
	return false;
}


Lattice_LL* Lattice_Create(BYTE **Lattice, int SqSize) {
	Lattice_LL *head = (Lattice_LL*)malloc(sizeof(Lattice_LL));
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
		head->Lattice = Lattice;
		// Calculate Size_h, Size_v
		CalculateSize(Lattice, SqSize, &size_h, &size_v);
		head->Size_h = size_h;
		head->Size_v = size_v;

		return head;
	}
}

int  Lattice_Add(Lattice_LL *head, int SqSize, BYTE **Lattice) {
	int size_h, size_v;

	if (head == NULL)
	{
		exit(-1);
	}

	Lattice_LL *NewNode = (Lattice_LL*)malloc(sizeof(Lattice_LL));
	if (NewNode == NULL) 
	{
		printf("can't allocate memory!\n");
		return -1;
	}
	
	NewNode->next = NULL;
	NewNode->SqSize = SqSize;
	NewNode->Lattice = Lattice;
	// Calculate Size_h, Size_v
	CalculateSize(Lattice, SqSize, &size_h, &size_v);
	NewNode->Size_h = size_h;
	NewNode->Size_v = size_v;
#if 0	
	Lattice_LL * temp;
	temp = head;
	while (temp->next !=NULL)
	{
		temp = temp->next;
	}
	temp->next = NewNode;
#else  // This also works - So it does not change the head when the function exits.
	while (head->next !=NULL)
	{
		head = head->next;
	}
	head->next = NewNode; 
#endif
	return 0;
}

BYTE** Memory2D(int SqSize)
{
	BYTE **Temp;
	Temp = (BYTE **)malloc(sizeof(BYTE *)*SqSize);
	if (Temp == NULL)
	{
		printf("Memory allocation error!\n");
		exit(1);
	}
	for (int i=0; i<SqSize; i++)
	{
		Temp[i] = (BYTE *)malloc(sizeof(BYTE)*SqSize);
		if (Temp[i] == NULL)
		{
			printf("Memory allocation error!\n");
			exit(1);
		}
	}
	return Temp;
}

void Lattice_Delete(Lattice_LL *Lattice, int SqSize)
{
	Lattice_LL *tmp1, *tmp;
	tmp = Lattice;

	while (NULL != tmp)
	{
		for (int i=0;i<SqSize; i++)
			free(tmp->Lattice[i]);
		free(tmp->Lattice);
		tmp1 = tmp->next;
		free(tmp);
		tmp = tmp1;
	}
}

void Lattice_Copy(Lattice_LL **Lattice_o, Lattice_LL *Lattice_n,  int SqSize) // Size is new size
{

	BYTE **NewLattice;

	if (NULL != *Lattice_o)
	{
		*Lattice_o = NULL;
	}

	while (NULL != Lattice_n)
	{

		NewLattice = Memory2D(SqSize);

		for (int i=0; i<SqSize; i++)
		{
			memcpy(NewLattice[i], Lattice_n->Lattice[i], sizeof(BYTE)*SqSize);
		}

		if (NULL == *Lattice_o)
		{
			*Lattice_o = Lattice_Create(NewLattice,SqSize);
		}
		else
		{
			Lattice_Add(*Lattice_o, SqSize, NewLattice);
		}
		Lattice_n = Lattice_n->next;
	}
}

void CalculateSize(BYTE **Lattice, int SqSize, int *size_h, int *size_v)
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

bool LatticeCompare(BYTE **Pattern, int size_h1, int size_v1, BYTE **NewPat,  int SqSize) // false = not equal; true = equal
{
	int i, size_h2, size_v2;

	CalculateSize(NewPat, SqSize, &size_h2, &size_v2);

	//Check if the size are the same, including rotation
	if (!( ((size_h1 == size_h2) && (size_v1 == size_v2))			// Same size
	   ||((size_h1 == size_v2) && (size_v1 == size_h2)) ))		// Same size after rotation
	{
		return false;  // not same
	}
#if 1
	if (size_h1 == size_h2) // original
	{
		for (i=0; i<4; i++)
		{
			if (SamePatten(Pattern, NewPat, size_h1, size_v1, i))
			{
				return true;
			}
		}
		if (size_h1 == size_v2)  //square, check rotation
		{
			for (i=4; i<8; i++)
			{
				if (SamePatten(Pattern, NewPat, size_h1, size_v1, i))
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
			if (SamePatten(Pattern, NewPat, size_h1, size_v1, i)) // check rotation
			{
				return true;
			}
		}
	}
#else
	if (size_h1 == size_h2) // original
	{
		for (i=0; i<4; i++)
		{
			if (SamePatten(Pattern, NewPat, size_h1, size_v1, i))
			{
				return true;
			}
		}

	}
	else // rotate90, rotate270
	{
		for (i=4; i<8; i++)
		{
			if (SamePatten(Pattern, NewPat, size_h1, size_v1, i))
			{
				return true;
			}
		}
	}
#endif
	return (false);
}


bool SamePatten(BYTE **Pattern, BYTE **NewPat, int size_h, int size_v, int MirroRotate)
{
	// Size is Lattice Pattern Size
	switch (MirroRotate)
	{
		case 0:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[i][j])
						return false;
				}
			return true;
		case 1:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[i][size_h-1-j])
						return false;
				}
			return true;
		case 2:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[size_v-1-i][j])
						return false;
				}
			return true;
		case 3:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[size_v-1-i][size_h-1-j])
						return false;
				}
			return true;
		case 4:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[j][i])
						return false;
				}
			return true;
		case 5:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[size_h-1-j][i])
						return false;
				}
			return true;
		case 6:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[j][size_v-1-i])
						return false;
				}
			return true;
		case 7:
			for (int i=0; i<size_v; i++)
				for (int j=0; j<size_h; j++)
				{
					if (Pattern[i][j] != NewPat[size_h-1-j][size_v-1-i])
						return false;
				}
			return true;
	}
};

void Move2TopLeft(BYTE **Lattice, int SqSize)
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

