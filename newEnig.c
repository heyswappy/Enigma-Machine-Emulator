/*
	AUTHOR: SWAPNIL SAXENA
	DATE: 10 DECEMBER 2018
	ENIGMA MACHINE EMULATOR WITH 5 ROTORS
	CAN USE ANY 3 OF THEM AT A TIME WITH CUSTOM OFFSETS
	JAYPEE INSTITUTE OF INFORMATION TECHNOLOGY
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>

struct rotor{
	char roll[27];
	int head;
};
struct rotor RotorA;
struct rotor RotorB;
struct rotor RotorC;
struct rotor RotorD;
struct rotor RotorE;
struct rotor Reflector;
struct rotor *rotorList[5];

struct enigma{
	struct rotor *A;
	struct rotor *B;
	struct rotor *C;
	struct rotor *R;
	int flagRotorA; // if set rotate rotor A
	int flagRotorB; // if set rotate rotor B
	int flagRotorC; // if set rotate rotor C
};

struct enigma army;

void createRotors(struct rotor *A, struct rotor *B, struct rotor *C, struct rotor *D, struct rotor *E, struct rotor *R);

void createEnigma(struct enigma *turing, struct rotor *a, struct rotor *b, struct rotor *c, struct rotor *r);

void createEnigmaFromUser(struct enigma *a);

char *inputLine();

int getCharOffest(char t);

char scrambleRToL(struct rotor *Rotor, char t, int *myFlag, int *nxFlag, int i, int check);

char scrambleLToR(struct rotor *Rotor, char t, int roolN);

char RtoLWiring(struct enigma *turing, char data);

char LtoRWiring(struct enigma *turing, char data);

void incr(struct enigma *turing);

char *perform(struct enigma *turing, char *toEncrypt);

void main(){
	createRotors(&RotorA, &RotorB, &RotorC, &RotorD, &RotorE, &Reflector);
	createEnigmaFromUser(&army);
	char *ptr = inputLine();
	printf("%s\n",ptr);
	char *p = perform(&army, ptr);
	printf("\n%s\n",p);
}

void createRotors(struct rotor *A, struct rotor *B, struct rotor *C, struct rotor *D, struct rotor *E, struct rotor *R){
	// this function creates rotors with scrambled alphabets
	char *temp;

	temp = (char*)calloc(sizeof(char),27);
	strcpy(temp,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	for(int i = 0; i<26; i++){
		// set the ith charachter of string to roll
		(A->roll)[i] = temp[i];
	}

	temp = (char*)calloc(sizeof(char),27);
	strcpy(temp,"NOPQRSTUVWXYZABCDEFGHIJKLM");
	for(int i = 0; i<26; i++){
		// set the ith charachter of string to roll
		(B->roll)[i] = temp[i];
	}

	temp = (char*)calloc(sizeof(char),27);
	strcpy(temp,"HIJKLMNOPQRSABCDEFGTUVWXYZ");
	for(int i = 0; i<26; i++){
		// set the ith charachter of string to roll
		(C->roll)[i] = temp[i];
	}

	temp = (char*)calloc(sizeof(char),27);
	strcpy(temp,"JKLBCPQRSTUVDEFGHIZMNOWXYA");
	for(int i = 0; i<26; i++){
		// set the ith charachter of string to roll
		(D->roll)[i] = temp[i];
	}

	temp = (char*)calloc(sizeof(char),27);
	strcpy(temp,"OPQRSTBCDENWIJKLMXYUVFGHAZ");
	for(int i = 0; i<26; i++){
		// set the ith charachter of string to roll
		(E->roll)[i] = temp[i];
	}

	temp = (char*)calloc(sizeof(char),27);
	strcpy(temp,"ZYXWVUTSRQPONMLKJIHGFEDCBA");
	for(int i = 0; i<26; i++){
		// set the ith charachter of string to roll
		(R->roll)[i] = temp[i];
	}
	// set rotor heads to zero
	A->head = 0;
	B->head = 0;
	C->head = 0;
	D->head = 0;
	E->head = 0;
	R->head = 0;
	//set rotorList
	rotorList[0] = A;
	rotorList[1] = B;
	rotorList[2] = C;
	rotorList[3] = D;
	rotorList[4] = E;
	return;
}

void createEnigma(struct enigma *turing, struct rotor *a, struct rotor *b, struct rotor *c, struct rotor *r){
	// assign rolls reflector and flags to the enigma machine passed as a parameter
	turing->A = a;
	turing->B = b;
	turing->C = c;
	turing->R = r;
	turing->flagRotorA = 0;
	turing->flagRotorB = 0;
	turing->flagRotorC = 0;
	return;
}

void createEnigmaFromUser(struct enigma *enigmaMachine){
	// set the rotors and the offsets
	struct rotor *arr[3];
	printf("ENTER ROTOR NUMBERS (3 NUMBERS): ");
	for(int i=0; i<3; i++){
		int t;
		scanf("%d", &t);
		t = t%5;
		arr[i] = rotorList[t];
	}
	int offset[3];
	printf("ENTER ROTOR OFFSETS (3 NUMBERS): ");
	for(int i=0; i<3; i++){
		int t;
		scanf("%d", &t);
		t = t%26;
		offset[i] = t;
	}
	arr[0]->head = offset[0];
	arr[1]->head = offset[1];
	arr[2]->head = offset[2];
	createEnigma(enigmaMachine,arr[0],arr[1],arr[2],&Reflector);
}

char *inputLine(){
	char *ptr = (char*)calloc(sizeof(char),512);
	int count = 0;
	printf("ENTER THE STRING: ");
	fflush(stdin);
	char c = '0';
	while(count<511){
		c = getchar();
		if(c!='\n'){
			ptr[count] = toupper(c);
			count++;
		}
		else{
			ptr[count] = '\0';
			break;
		}
	}
	// prevent memory violation
	ptr[511] = '\0';
	printf("\n");
	return ptr;
}

int getCharOffest(char t){
	int ascii;
	ascii = t - 'A';
	return ascii;
}

char getReflector(struct rotor *reflector, char d){
	int ind = getCharOffest(d);
	return (reflector->roll)[ind];
}

char scrambleLToR(struct rotor *Rotor, char t, int rollNum){
	// scramble letter from left to right
	// the rotors used are not functions that if y=f(x)
	// we cant generate x=f^(-1)(y)
	// so we have to brute force on f(x) for all x to know which x gives y
	// here the chack parameter is used for brute forcing so taht flags are not changed
	int ind = getCharOffest(t);
	for (int i = 0; i < 26; i++){
		if (scrambleRToL(Rotor, 'A'+i, NULL, NULL, rollNum, 0) == t){
			return 'A'+i;
		}
	}
	return '?';
}

char LtoRWiring(struct enigma *turing, char data){
	// emulator of left to right wiring
	//rintf("-%c->",data);
	data = getReflector(turing->R,data);
	//printf("-%c->",data);
	data = scrambleLToR(turing->A,data, 3);
	//printf("-%c->",data);
	data = scrambleLToR(turing->B,data, 2);
	//printf("-%c->",data);
	data = scrambleLToR(turing->C,data, 1);
	//printf("-%c->\n",data);
	return (data);
}

char scrambleRToL(struct rotor *Rotor, char s, int *myFlag, int *nxFlag, int i, int check){
	// functions that scrambles the letteres that is assists
	// in giving a charachter as an inpu and getting a charachter as an output
	// if check = 1 then flags are affected else not
	// helpful when we have to perform left to right scrambling
	// since the rotors are not exactly mathematical functions that can be inversed
	int t = getCharOffest(s);
	if(i==1){
		s = Rotor->roll[((Rotor->head)+t)%26];
		if(check){
			*myFlag = 1;
		}
		if((((Rotor->head)+1)%26)==0){
			if(check){
				*nxFlag=1;
			}
			return s;
		}
		else{
			return s;
		}
	}
	else if(i==2){
		s = Rotor->roll[((Rotor->head)+t)%26];
		if((((Rotor->head)+1)%26)==0){
			if(check){
				*nxFlag=1;
			}
			return s;
		}
		else{
			return s;
		}
	}
	else if(i==3){
		s = Rotor->roll[((Rotor->head)+t)%26];
		return s;
	}
	else{
		return '*';
	}
	return '#';
}

char RtoLWiring(struct enigma *turing, char data){
	// right to left wiring emulator
	//printf("-%c->",data);
	data = scrambleRToL(turing->C, data, &(turing->flagRotorC), &(turing->flagRotorB), 1, 1);
	//printf("-%c->",data);
	data = scrambleRToL(turing->B, data, NULL, &(turing->flagRotorA), 2, 1);
	//printf("-%c->",data);
	data = scrambleRToL(turing->A, data, NULL, NULL, 3, 1);
	//printf("-%c->",data);
	return (data);
}

void incr(struct enigma *turing){
	// increment the flag accordingly
	if (turing->flagRotorC){
		turing->flagRotorC = 0;
		turing->C->head = ((turing->C->head)+1)%26;
	}
	if (turing->flagRotorB){
		turing->flagRotorB = 0;
		turing->B->head = ((turing->B->head)+1)%26;
	}
	if (turing->flagRotorA){
		turing->flagRotorA = 0;
		turing->A->head = ((turing->A->head)+1)%26;
	}
	return;
}

char *perform(struct enigma *turing, char *toEncrypt){
	int l = strlen(toEncrypt);
	char *ans = calloc(sizeof(char),l+1);
	char t;
	for(int i = 0; i<l; i++){
		t = toEncrypt[i];
		if(t>='A' && t<='Z'){
			t = RtoLWiring(turing, t);
			t = LtoRWiring(turing, t);
			incr(turing);
		}
		ans[i] = t;
	}
	return ans;
}