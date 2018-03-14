//------------------------------------------------------------

#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"

#define delay(ms) vTaskDelay(ms) 

uint32_t millis( void ) {
    return (__get_IPSR() == 0) ? xTaskGetTickCount() : xTaskGetTickCountFromISR();
}

int c_printf(const char *fmt, ...);

//------------------------------------------------------------
// dhry.c
// https://developer.mbed.org/users/jmehring/code/Dhrystone/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define LOOPS   512

#ifdef NOSTRUCTASSIGN
#  define structassign(d, s)    memcpy(&(d), &(s), sizeof(d))
#else
#  define structassign(d, s)    d = s
#endif

typedef enum {
	Ident1, Ident2, Ident3, Ident4, Ident5
} Enumeration;

typedef int OneToThirty;
typedef int OneToFifty;
typedef char CapitalLetter;
typedef char String30[31];
typedef int Array1Dim[51];
typedef int Array2Dim[51][51];

struct Record {
	struct Record *PtrComp;
	Enumeration Discr;
	Enumeration EnumComp;
	OneToFifty IntComp;
	String30 StringComp;
};

typedef struct Record RecordType;
typedef RecordType *RecordPtr;

#ifndef NULL
#  define NULL    (void *)0
#endif

#ifndef FALSE
#  define FALSE   0
#  define TRUE    (!FALSE)
#endif

#ifndef REG
#  define REG
#endif

void Proc0(void);
void Proc1(RecordPtr PtrParIn);
void Proc2(OneToFifty *IntParIO);
void Proc3(RecordPtr *PtrParOut);
void Proc4(void);
void Proc5(void);
void Proc6(Enumeration EnumParIn, Enumeration *EnumParOut);
void Proc7(OneToFifty IntParI1, OneToFifty IntParI2, OneToFifty *IntParOut);
void Proc8(Array1Dim Array1Par, Array2Dim Array2Par, OneToFifty IntParI1,
		OneToFifty IntParI2);
Enumeration Func1(CapitalLetter CharPar1, CapitalLetter CharPar2);
bool Func2(String30 StrParI1, String30 StrParI2);
bool Func3(Enumeration EnumParIn);

int IntGlob;
int cnt, sum_flag;
bool BoolGlob;
char Char1Glob;
char Char2Glob;
Array1Dim Array1Glob;
Array2Dim Array2Glob;
RecordPtr PtrGlb;
RecordPtr PtrGlbNext;

/****************************************************************************/

void Proc0(void) {
	OneToFifty IntLoc1;
	OneToFifty IntLoc2;
	OneToFifty IntLoc3;
	char CharIndex;
	Enumeration EnumLoc;
	String30 String1Loc, String2Loc;
	unsigned long idx;

	PtrGlbNext = (RecordPtr) malloc(sizeof(RecordType));
	PtrGlb = (RecordPtr) malloc(sizeof(RecordType));
	PtrGlb->PtrComp = PtrGlbNext;
	PtrGlb->Discr = Ident1;
	PtrGlb->EnumComp = Ident3;
	PtrGlb->IntComp = 40;
	strcpy(PtrGlb->StringComp, "DHRYSTONE PROGRAM, SOME STRING");

	for (idx = 0; idx < LOOPS; idx++) {
		Proc5();
		Proc4();

		IntLoc1 = 2;
		IntLoc2 = 3;
		IntLoc3 = 0;

		strcpy(String2Loc, "DHRYSTONE PROGRAM, 2'ND STRING");

		EnumLoc = Ident2;
		BoolGlob = !Func2(String1Loc, String2Loc);

		while (IntLoc1 < IntLoc2) {
			IntLoc3 = 5 * IntLoc1 - IntLoc2;
			Proc7(IntLoc1, IntLoc2, &IntLoc3);
			++IntLoc1;
		}

		Proc8(Array1Glob, Array2Glob, IntLoc1, IntLoc3);
		Proc1(PtrGlb);

		for (CharIndex = 'A'; CharIndex <= Char2Glob; ++CharIndex)
			if (EnumLoc == Func1(CharIndex, 'C'))
				Proc6(Ident1, &EnumLoc);

		IntLoc3 = IntLoc2 * IntLoc1;
		IntLoc2 = IntLoc3 / IntLoc1;
		IntLoc2 = 7 * (IntLoc3 - IntLoc2) - IntLoc1;
		Proc2(&IntLoc1);
	}

	free(PtrGlbNext);
	free(PtrGlb);
}

void Proc1(RecordPtr PtrParIn) {
	structassign(*(PtrParIn->PtrComp), *PtrGlb);
	PtrParIn->IntComp = 5;
	PtrParIn->PtrComp->IntComp = PtrParIn->IntComp;
	PtrParIn->PtrComp->PtrComp = PtrParIn->PtrComp;
	Proc3((RecordPtr *) (PtrParIn->PtrComp->PtrComp));
	if ((PtrParIn->PtrComp)->Discr == Ident1) {
		PtrParIn->PtrComp->IntComp = 6;
		Proc6(PtrParIn->EnumComp, &(PtrParIn->PtrComp->EnumComp));
		PtrParIn->PtrComp->PtrComp = PtrGlb->PtrComp;
		Proc7(PtrParIn->PtrComp->IntComp, 10, &(PtrParIn->PtrComp->IntComp));
	} else
		structassign(*PtrParIn, *(PtrParIn->PtrComp));
}

void Proc2(OneToFifty *IntParIO) {
	OneToFifty IntLoc;
	Enumeration EnumLoc;

	IntLoc = *IntParIO + 10;

	for (;;) {
		if (Char1Glob == 'A') {
			IntLoc -= 1;
			*IntParIO = IntLoc - IntGlob;
			EnumLoc = Ident1;
		}

		if (EnumLoc == Ident1)
			break;
	}
}

void Proc3(RecordPtr *PtrParOut) {
	if (PtrGlb != NULL)
		*PtrParOut = PtrGlb->PtrComp;
	else
		IntGlob = 100;

	Proc7(10, IntGlob, &PtrGlb->IntComp);
}

void Proc4(void) {
	bool BoolLoc;

	BoolLoc = Char1Glob == 'A';
	BoolLoc |= BoolGlob;
	Char2Glob = 'B';
}

void Proc5(void) {
	Char1Glob = 'A';
	BoolGlob = FALSE;
}

void Proc6(Enumeration EnumParIn, Enumeration *EnumParOut) {
	*EnumParOut = EnumParIn;

	if (!Func3(EnumParIn))
		*EnumParOut = Ident4;

	switch (EnumParIn) {
	case Ident1:
		*EnumParOut = Ident1;
		break;
	case Ident2:
		*EnumParOut = (IntGlob > 100) ? Ident1 : Ident4;
		break;
	case Ident3:
		*EnumParOut = Ident2;
		break;
	case Ident4:
		break;
	case Ident5:
		*EnumParOut = Ident3;
		break;
	}
}

void Proc7(OneToFifty IntParI1, OneToFifty IntParI2, OneToFifty *IntParOut) {
	OneToFifty IntLoc;

	IntLoc = IntParI1 + 2;
	*IntParOut = IntParI2 + IntLoc;
}

void Proc8(Array1Dim Array1Par, Array2Dim Array2Par, OneToFifty IntParI1,
		OneToFifty IntParI2) {
	OneToFifty IntLoc;
	OneToFifty IntIndex;

	IntLoc = IntParI1 + 5;
	Array1Par[IntLoc] = IntParI2;
	Array1Par[IntLoc + 1] = Array1Par[IntLoc];
	Array1Par[IntLoc + 30] = IntLoc;

	for (IntIndex = IntLoc; IntIndex <= (IntLoc + 1); ++IntIndex)
		Array2Par[IntLoc][IntIndex] = IntLoc;

	++Array2Par[IntLoc][IntLoc - 1];
	Array2Par[IntLoc + 20][IntLoc] = Array1Par[IntLoc];
	IntGlob = 5;
}

Enumeration Func1(CapitalLetter CharPar1, CapitalLetter CharPar2) {
	CapitalLetter CharLoc1;
	CapitalLetter CharLoc2;

	CharLoc1 = CharPar1;
	CharLoc2 = CharLoc1;

	return ((CharLoc2 != CharPar2) ? Ident1 : Ident2);
}

bool Func2(String30 StrParI1, String30 StrParI2) {
	OneToThirty IntLoc;
	CapitalLetter CharLoc;

	IntLoc = 1;
	while (IntLoc <= 1) {
		if (Func1(StrParI1[IntLoc], StrParI2[IntLoc + 1]) == Ident1) {
			CharLoc = 'A';
			++IntLoc;
		}
	}

	if (CharLoc >= 'W' && CharLoc <= 'Z') {
		IntLoc = 7;
	}

	if (CharLoc == 'X') {
		return (TRUE);
	} else {
		if (strcmp(StrParI1, StrParI2) > 0) {
			IntLoc += 7;
			return (TRUE);
		} else
			return (FALSE);
	}
}

bool Func3(Enumeration EnumParIn) {
	Enumeration EnumLoc;

	EnumLoc = EnumParIn;

	return (EnumLoc == Ident3);
}

//------------------------------------------------------------------------

void test(void)
{
    double benchtime, dps;
    unsigned int tt, t1;
    unsigned long long loops;
    dps = PLATFORM_CLOCK/1000000.0;
	c_printf("\r\nDhrystone Benchmark Program C/1 12/01/84\r\n");
    c_printf("CLK CPU = %lf MHz.\r\n", dps);
    c_printf("Test cycles 60 sec...\r\n");
    while(1) {
        loops = 0l;
		delay(1000);
		tt = millis();
        do {
            Proc0();
            loops += LOOPS;
  			t1 = millis() - tt;
        } while (t1 < 60000);
  		benchtime = (double)t1/1000.0;
        dps = (double)loops / benchtime;
  		
        c_printf("Dhrystone time for %llu passes = %.3f sec\r\n", loops, benchtime);
        c_printf("This machine benchmarks at %.0f dhrystones/second\r\n", dps);
    }
}


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	if(xTaskCreate( (TaskFunction_t)test, "Test", (2048), NULL, (tskIDLE_PRIORITY + 3), NULL)!= pdPASS) {
			DBG_8195A("Cannot create Test task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}
