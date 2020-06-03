#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFFER 256

struct games{
	char gamename[MAX_BUFFER],platfrom[MAX_BUFFER];
	int gamescore,year;
};struct games n1[19000];

struct storegame {
	int score, year;
	char name[MAX_BUFFER], platform[MAX_BUFFER];
};

void swap(int* a,char s1[MAX_BUFFER], int* b, char s2[MAX_BUFFER]) 
{  int t = *a; 
	*a = *b; 
	*b = t; 
	char temp[MAX_BUFFER];
	strcpy(temp,s1);
	strcpy(s1,s2);
	strcpy(s2,temp);	
} 

int partition (struct games arr[19000], int low, int high) 
{    int j ;
	int pivot = n1[high].gamescore; 					
	int i = (low - 1); 						
	for (j = low; j <= high- 1; j++) 
	{ 
		if (n1[j].gamescore < pivot) 
		{ 
			i++; 							
			swap(&n1[i].gamescore,n1[i].gamename, &n1[j].gamescore,n1[j].gamename); 
		} 
	} 
	swap(&n1[i + 1].gamescore,n1[i+1].gamename, &n1[high].gamescore,n1[i+1].gamename); 
		return (i + 1); 	
} 

void quick_sort(struct games arr[19000], int low , int high) 
{ 
	if (low < high) 
	{   
		int pi = partition(n1,low , high); 
		quick_sort(n1, low, pi - 1); 
		quick_sort(n1, pi + 1, high); 
	} 
} 

void printArray(struct games arr[], int size) 
{ 
	int i; 
	for (i=(size-11); i < (size-1); i++) 
	printf("\n%d %s\n", n1[i].gamescore,n1[i].gamename); 
} 

int next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	for(;;) {	
		buf[i] = fgetc(f);
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		if(buf[i]==',' && !quoted) { break; }
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		if( i<max-1 ) { ++i; } 
	}
	buf[i] = 0; 
	return end; 
}

void fetch_game ( FILE *csv, struct storegame *p ) {
	char buf[MAX_BUFFER];
	next_field( csv,p->name,MAX_BUFFER );                          
	next_field( csv, p->platform, MAX_BUFFER );  
	next_field( csv, buf, MAX_BUFFER );
	p->score = atoi(buf);         
	next_field( csv, buf, MAX_BUFFER ); 
	p->year = atoi(buf);                  
}

int z=0;
void recordtoarray( struct storegame *p ) {	
  	strcpy(n1[z].gamename,p->name) ;
	n1[z].gamescore=p->score;
	z++;
}

int main () {
	FILE *f;		
	struct storegame p;
	f = fopen("ign.csv", "r");
	if(!f) { 
		printf("unable to open %s\n", "ign.csv");  
		return EXIT_FAILURE; 
	}
	//fetch_game( f, &p ); 
	while(!feof(f)) {                   
		fetch_game( f, &p );
		recordtoarray( &p );		
	}
	int n = sizeof(n1)/sizeof(n1[0]); 
	quick_sort(n1, 0, n-1); 
	printf("Most popular games: \n"); 
	printArray(n1, n); 
	fclose(f);
	return EXIT_SUCCESS;
}


