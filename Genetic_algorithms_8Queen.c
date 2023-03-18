#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_QUEEN 8
#define NB_ROWS 8
#define NB_COLUMNS 8
#define MAXLENGTH 100
#define MAX_POPULATION 300


typedef struct{
	int A[NB_ROWS][NB_ROWS];
	int numQueen;
}ChessBoard;


//Khoi tao ban co
void initBoard(ChessBoard* B){
	int i, j;
	for(i=0; i<NB_ROWS; i++)
		for(j=0; j<NB_COLUMNS; j++)
			B->A[i][j] = 0;
	B->numQueen = 0;		
}

void printBoard(ChessBoard B){
	int i, j;
	for(i=0; i<NB_ROWS; i++){
		printf("-------------------------------------------------\n");
		for(j=0; j<NB_COLUMNS; j++){
			if(B.A[i][j] == 0) printf("|     ");
			else printf("| %c%c%c ", 200, 178, 188);
		}
		printf("|");
		printf("\n");
	}
	printf("-------------------------------------------------\n\n");
}



typedef struct{
	int Gens[MAX_QUEEN];
}Chromosome;

typedef struct{
	Chromosome C[300];
	int size;
}Population;

void initPopulation(Population *p){
	p->size=0;
}

void addPopulation(Population *p, Chromosome c){
	p->C[p->size] = c;
	p->size++;
}

void DeleteChromosome(Population *p, int pos){
	int i, j;
	for(i=pos-1; i<p->size-1; i++)
		p->C[i] = p->C[i+1];
	p->size--;	
}



typedef struct{
	int x, y;
}Coord;

//Danh sach toa do cac queen o 1 ca the
typedef struct{
	Coord coord[MAX_QUEEN];
	int size;
} ListCoord;

void initListCoord(ListCoord *list){
	list->size=0;
}

void pushCoord(ListCoord *list, Coord coord){
	list->coord[list->size] = coord;
	list->size++;
}

Coord Element_at(ListCoord list, int i){
	return list.coord[i-1];
}


//Tra ve danh sach cac Coord theo thu tu gen giam dan
ListCoord getPositionQueen2(Chromosome c){
	int i;
	ListCoord list;
	initListCoord(&list);
	
	int flag[MAX_QUEEN];
	for(i=0; i<MAX_QUEEN; i++)
		flag[c.Gens[i]]=0; //Danh dau gia tri nao da dung roi
	
	while(list.size < MAX_QUEEN ){ //Lam den khi bo du 8 Coord vao list
		int Mincol = 10000, Minrow=10000;
		for(i=0; i<MAX_QUEEN; i++){
			if(c.Gens[i] < Minrow && flag[c.Gens[i]] == 0){ //Lan luot kiem tra gia tri tung gen va gen phai chua duoc bo vao list
				Minrow = c.Gens[i];
				Mincol = i;
			}
		}
		Coord coord;
		coord.x = Minrow;
		coord.y = Mincol;
		flag[c.Gens[Mincol]]=1;
		pushCoord(&list, coord);	
	}
	return list;			
}


int indexOf(int x, int y){
	return (NB_ROWS * x + y);
}


int Fitness3(Chromosome c){
	ListCoord L = getPositionQueen2(c);
	int values[NB_ROWS*NB_COLUMNS]; //Danh dau vi tri cua cac Queen
	int flag[NB_ROWS*NB_COLUMNS]; //Danh dau queen thu n da duoc xet tren duong cheo chua
	int i;
	for(i=0; i<NB_ROWS*NB_COLUMNS; i++)
		values[i] = 0;
		
	for(i=1; i<=L.size; i++){
		Coord pos = Element_at(L, i);
		values[indexOf(pos.x, pos.y)] = 1;
		flag[indexOf(pos.x, pos.y)]=0; 
	} 		
	
	int k, j;
	int t1=0, sum1=0;
	int t2=0, sum2=0;
	for(k=L.size; k>=1; k--){
		Coord v = Element_at(L, k);
		i=v.x;
		j=v.y;
		while(i>=0 && j<NB_COLUMNS){
			if(values[indexOf(i, j)]==1 && flag[indexOf(i, j)]==0){
				t1 += 1;
				flag[indexOf(i, j)]=1;
			}
			i--;
			j++;
		}
		if(t1 != 0) t1-=1;
		sum1 += t1;
		t1=0;
	}
	
	for(i=1; i<=L.size; i++){
		Coord pos = Element_at(L, i);
		flag[indexOf(pos.x, pos.y)]=0;
	}
	
	for(k=L.size; k>=1; k--){
		Coord v = Element_at(L, k);
		i=v.x;
		j=v.y;
		while(i>=0 && j>=0){
			if(values[indexOf(i, j)]==1 && flag[indexOf(i, j)]==0){
				t2 += 1;
				flag[indexOf(i, j)]=1;
			}
			i--;
			j--;
		}
		if(t2 != 0) t2 -= 1;
		sum2 += t2;
		t2=0;
	}
	return sum1+sum2;
	
}



int random(int min, int max){
	return min + rand() % (max + 1 -min);
}

void RandomPopulation(Population *p){
	initPopulation(p);
	int i, j, k;
	int flag[MAX_QUEEN];
	srand((unsigned int)time(NULL)); //gia tri thoi gian 	
	for(i=0; i<MAX_POPULATION; i++){
		for(k=0; k<MAX_QUEEN; k++)
			flag[k]=0; //voi moi ca the moi tao ra thi cac NST duoc reset lai 
		j=0;
		while(j<MAX_QUEEN){
			int r=random(0, MAX_QUEEN-1);
			if(flag[r] == 0){ //NST da co trong 1 ca the roi thi random lai
				p->C[i].Gens[j] = r;
				flag[r] = 1;
			}
			else if(j == MAX_QUEEN-1){
				for(k=0; k<MAX_QUEEN; k++)
					if(flag[k] == 0) p->C[i].Gens[j]=k;
			}
			else p->C[i].Gens[j] = -1;
			
			if(p->C[i].Gens[j] != -1) j++; //Chi khi
		}
		p->size++;
	}					
}


void Swap(int *a, int *b){
	int tmp = *a;
	*a=*b;
	*b=tmp;
}

Chromosome Crossover(Chromosome dad, Chromosome mom){
	srand((unsigned int)time(NULL));
	int start = random(0, MAX_QUEEN-1); //Chon NST khoi dau
	int end = random(0, MAX_QUEEN-1); //Chon NST ket thuc
									//=> Child se ke thua bo gen giua 2 doan start va end cua Dad
	
	
	int i, flag[MAX_QUEEN], j;
	Chromosome child;
	for(i=0; i<MAX_QUEEN; i++){
		child.Gens[i]=-1;
		flag[i]=0;
	}		
	
	if(start > end) Swap(&start, &end);
	
	for(i=start; i<=end; i++){
		child.Gens[i]=dad.Gens[i]; //Ke thuc cac gen cua dad
		flag[dad.Gens[i]]=1; //danh dau gen nao da ke thua r tu bo thi khong ke thua tu me nua
	}
		 
	for(i=0; i<start; i++)	//nhung gen con thieu se ke thua tu me
		for(j=0; j<MAX_QUEEN; j++)
			if(flag[mom.Gens[j]]==0){
				child.Gens[i]=mom.Gens[j];
				flag[mom.Gens[j]]=1;
				break;
			}
				
	for(i=end+1; i<MAX_QUEEN; i++) //nhung gen con thieu se ke thua tu me
		for(j=0; j<MAX_QUEEN; j++)
			if(flag[mom.Gens[j]]==0){ //Gen nao cua mom ma chua co trong con thi ke thua
				child.Gens[i]=mom.Gens[j];
				flag[mom.Gens[j]]=1; //danh dau da ke thua roi
				break;
			}		
	return child;	
}


void Mutation(Chromosome *child, int *dotbien){
	srand((unsigned int)time(NULL));
	int a=random(0, MAX_QUEEN-1);
	int b=random(0, MAX_QUEEN-1);
	
	int F = Fitness3(*child);
	if(F == 1){
		while(1){
				int a=random(0, MAX_QUEEN-1);
				int b=random(0, MAX_QUEEN-1);
				while(b == a){
					b = random(0, MAX_QUEEN-1);
				}
				Swap(&child->Gens[a], &child->Gens[b]);
				*dotbien += 1;
				if(Fitness3(*child) < F) break;
			}	
	}
	else{
		while(b == a){
			b = random(0, MAX_QUEEN-1);
		}
		Swap(&child->Gens[a], &child->Gens[b]);
		*dotbien += 1;
	}	
}




void setQueen(ChessBoard* B, Chromosome c){
	initBoard(B);
	int i;
	for(i=0; i<MAX_QUEEN; i++){
		B->A[c.Gens[i]][i]=1;
		B->numQueen++;
	}
		
}


void SwapChromosome(Chromosome *x, Chromosome *y){
	Chromosome tmp = *x;
	*x = *y;
	*y = tmp;
}

void Selection(Population *p){
	int i, j;	
	int Fx, Fy;
	//Sap xep theo thu tu cac ca the manh duoc xep len tren
	for(i=0; i<p->size-1; i++){
		Fx = Fitness3(p->C[i]);
		for(j=i+1; j<p->size; j++){	
			Fy = Fitness3(p->C[j]);	
			if( Fx >= Fy ){
				SwapChromosome(&p->C[i], &p->C[j]);
				Fx = Fitness3(p->C[i]);
			}		
		}
	}
	
	//Loai bo cac ca the yeu
	int Fmax = Fitness3(p->C[p->size-1]);
	if(p->size != 0)
		DeleteChromosome(p, p->size);
	else printf("Can not Delete\n");
	
	for(i=p->size-1; i>=0; i--){
		int Fkill = Fitness3(p->C[i]);
		if(Fkill == Fmax && p->size != 0){
			if(p->size != 0) DeleteChromosome(p, p->size);
			else printf("Can not Delete\n");
		}
		else break;
	}	
}


void Genetic_Algorithm(Population p, int *danhgia, int *chonloc, int *lai, int *dotbien, int *soquanthe){	
	int a, b;		
	srand((unsigned int)time(NULL));
	int i, k;
	int isSelection = 0; //Quyet dinh xem co tiep tuc chon loc o cac vong lai tao sau hay khong
	int F1, F2, Fchild;
	Chromosome child[200];
	ChessBoard B;
	*danhgia=0;
	*chonloc=0;
	*lai=0;
	*dotbien=0;
	*soquanthe=0;
	while(1){
		RandomPopulation(&p);
		*soquanthe += 1;
		Selection(&p); //chon loc tu nhien
		*chonloc += 1;
		for(i=0; i<500; i++){
			*danhgia += 1;
			if(isSelection == 1){ //Dk tien hanh chon loc
				Selection(&p);
				*chonloc += 1;
				isSelection = 0; 
			}
			Chromosome x = p.C[0]; //Chon ra 2 ca the khoe nhat cua quan the
			Chromosome y = p.C[1];		
			F1 = Fitness3(x);			
			F2 = Fitness3(y);	
			Chromosome ch = Crossover(x, y);
					
			if(F1==0){
				setQueen(&B, x);
				printBoard(B);
				return;
			}
	
			
			else if(F2==0){
				setQueen(&B, y);
				printBoard(B);
				return;
			}
			
			
			for(k=0; k<=50; k++){   //Lai ra 2 con
				if(k%2==0){
					child[k] = Crossover(x, y); //x la bo, y la me
					*lai += 1;
				}	
				else{
					child[k] = Crossover(y, x); //nguoc lai
					*lai += 1;
				}
					 	
				int Fchild = Fitness3(child[k]);
				if(Fchild == 0){
					setQueen(&B, child[k]);
					printBoard(B);
					return;
				}
				else if(Fchild <= F1 && Fchild <= F2){
					addPopulation(&p, child[k]);
					isSelection=1;
				}
				
				int choose=random(1, 10);
				if(choose <= 8){
					Mutation(&child[k], dotbien); //Dot bien lan 1 voi ty le 0.8
					*dotbien += 1;
					int Fmutation1 = Fitness3(child[k]);
					if(Fmutation1 == 0){
						setQueen(&B, child[k]);
						printBoard(B);
						return;
					}
					else if(Fmutation1 <= Fchild && Fmutation1 <= F1 && Fmutation1 <= F2){
						addPopulation(&p, child[k]);
						isSelection=1;
					}
					choose=random(1, 10);
					if(choose <= 4){
						Mutation(&child[k], dotbien);
						*dotbien += 1;
						int Fmutation2 = Fitness3(child[k]);
						if(Fmutation2 == 0){
							setQueen(&B, child[k]);
							printBoard(B);
							return;
						}
						else if(Fmutation2 <= Fmutation1 && Fmutation2 <= Fchild && Fmutation2 <= F1 && Fmutation2 <= F2){
							addPopulation(&p, child[k]);
							isSelection=1;
						}
					}
				}			
			}				
		}	
	}
		
}

int main(){
	Population p;
	int lai, dotbien, chonloc, danhgia, soquanthe;	
	Genetic_Algorithm(p, &danhgia, &chonloc, &lai, &dotbien, &soquanthe);
	printf("So quan the tao ra la: %d\n", soquanthe);
	printf("So lan chon loc tu nhien la: %d\n", chonloc);
	printf("So lan danh gia la: %d\n", danhgia);
	printf("So phep lai phai thuc hien la: %d\n", lai);
	printf("So lan dot bien la: %d\n", dotbien);
	
	return 0;
}
