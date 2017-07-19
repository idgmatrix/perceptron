#ifndef PERCEPTRON_H

#define PERCEPTRON_H



extern int numTrain;

extern int numTest;



void InitSimulation(void);

void PrintPattern(HDC hDC, int x, int y, int n);

void PrintTestPattern(HDC hDC, int x, int y, int n);

void Training(int n);

void Test(int n);

void PrintOutput(int n);

void PrintTestOutput(int, int, int n);

void PrintTarget(int n);

float CalcError(void);



#endif