#include "stdafx.h"
#include "pattern.h"

#define MAXOUTPUT	10

extern HWND g_hWnd;

float Input[64];
float Output[MAXOUTPUT];
float Target[MAXOUTPUT];
float Weight[64][MAXOUTPUT];

float Alpha = 0.9f;

int numTest = 0;
int numTrain = 0;

TEST_PATTERN tpattern[20];
LERNING_PATTERN lpattern[30];

float Sigmoid(float net);
void ProcessNet(void);
void PrintPattern(HDC hDC, int x, int y, int n);
void Training(int n);
void PrintOutput(int n);
void PrintTestOutput(int, int, int n);
float CalcError(void);

float Sigmoid(float net)
{
	return (float)( 1 / (1 + exp(-net)) );
}

void InitSimulation(void)
{
	FILE *fp;
	char line[256];
	int i, n;

	fp = fopen("test.txt", "r");
	if (fp == NULL)
	{
		MessageBox(g_hWnd, "test.txt File Not Found", "Error", MB_OK);
		return;
	}

	n = 0;
	while( feof(fp) == 0)
	{ 
		for (i = 0; i < 8; i++)
		{
			fgets(line, 256, fp);

			if (line[0] == '#') goto cont1;

			sscanf(line, "%f %f %f %f %f %f %f %f", 
			&tpattern[n].Number[i * 8 + 0],
			&tpattern[n].Number[i * 8 + 1],
			&tpattern[n].Number[i * 8 + 2],
			&tpattern[n].Number[i * 8 + 3],
			&tpattern[n].Number[i * 8 + 4],
			&tpattern[n].Number[i * 8 + 5],
			&tpattern[n].Number[i * 8 + 6],
			&tpattern[n].Number[i * 8 + 7]);
		}
		n++;
cont1: ;
	}
	numTest = n;

	fclose(fp);

	//////////////////////////////////////////////////////////////////

	fp = fopen("lern.txt", "r");
	if (fp == NULL)
	{
		MessageBox(g_hWnd, "lern.txt File Not Found", "Error", MB_OK);
		return;
	}

	n = 0;
	while(!feof(fp))
	{ 
		for (i = 0; i < 8; i++)
		{
			fgets(line, 256, fp);

			if (line[0] == '#') goto cont2;
			sscanf(line, "%f %f %f %f %f %f %f %f", 
				&lpattern[n].Number[i * 8 + 0],
				&lpattern[n].Number[i * 8 + 1],
				&lpattern[n].Number[i * 8 + 2],
				&lpattern[n].Number[i * 8 + 3],
				&lpattern[n].Number[i * 8 + 4],
				&lpattern[n].Number[i * 8 + 5],
				&lpattern[n].Number[i * 8 + 6],
				&lpattern[n].Number[i * 8 + 7]);

		}

		fgets(line, 256, fp);
		sscanf(line, "%f %f %f %f %f %f %f %f %f %f", 
			&lpattern[n].Target[0],
			&lpattern[n].Target[1],
			&lpattern[n].Target[2],
			&lpattern[n].Target[3],
			&lpattern[n].Target[4],
			&lpattern[n].Target[5],
			&lpattern[n].Target[6],
			&lpattern[n].Target[7],
			&lpattern[n].Target[8],
			&lpattern[n].Target[9]);

		n++;
cont2: ;
	}

	numTrain = n;

	char string[256];
	sprintf(string, "numTrain = %d", numTrain);
	//MessageBox(g_hWnd, string, "numTrain", MB_OK); 
	fclose(fp);
}

void ProcessNet(void)
{	
	float net;

	for (int j = 0; j < MAXOUTPUT; j++)
	{
		net = 0.0f;

		for (int i = 0; i < 64; i++)
		{
			net += Weight[i][j] * Input[i];
		}

		Output[j] = Sigmoid(net);
	}
}

void PrintPattern(HDC hDC, int x, int y, int n)
{
	for ( int i = 0; i < 8; i++)
	{
		for ( int j = 0; j < 8; j++)
		{
			if ( lpattern[n].Number[i * 8 + j] == 1.0f)
				SetPixel(hDC, x + j, y + i, 0);
		}
	}
}

void PrintTestPattern(HDC hDC, int x, int y, int n)
{
	for ( int i = 0; i < 8; i++)
	{
		for ( int j = 0; j < 8; j++)
		{
			if ( tpattern[n].Number[i * 8 + j] == 1.0f)
				SetPixel(hDC, x + j, y + i, 0);
		}
	}
}

void PrintOutput(int n)
{
	char string[256];

	HDC hDC = GetDC(g_hWnd);

	sprintf(string, "%f  %f  %f  %f  %f  %f  %f  %f  %f  %f    %f",
		Output[0],
		Output[1], 
		Output[2], 
		Output[3], 
		Output[4], 
		Output[5],
		Output[6], 
		Output[7], 
		Output[8], 
		Output[9],
		CalcError()); 

	TextOut(hDC, 20, 20 + n * 15, string, strlen(string)); 

	ReleaseDC(g_hWnd, hDC);
}

void PrintTestOutput(int x, int y, int n)
{
	char string[256];

	HDC hDC = GetDC(g_hWnd);

	sprintf(string, "%f  %f  %f  %f  %f  %f  %f  %f  %f  %f",
		Output[0], 
		Output[1], 
		Output[2], 
		Output[3], 
		Output[4], 
		Output[5], 
		Output[6], 
		Output[7], 
		Output[8], 
		Output[9]); 

	TextOut(hDC, x, y, string, strlen(string)); 

	ReleaseDC(g_hWnd, hDC);
}

void SetInputData(int n)
{
	for (int i = 0; i < 64; i++)
	{
		Input[i] = lpattern[n].Number[i];
	}
}

void SetTestInputData(int n)
{
	for (int i = 0; i < 64; i++)
	{
		Input[i] = tpattern[n].Number[i];
	}
}

void SetTargetData(int n)
{
	for (int i = 0; i < MAXOUTPUT; i++)
	{
		Target[i] = lpattern[n].Target[i];
	}
}

void AdjustWeight(void)
{
	float Error;

	for (int j = 0; j < MAXOUTPUT; j++)
	{
		Error = Target[j] - Output[j];
		for (int i = 0; i < 64; i++)
		{
			Weight[i][j] = Weight[i][j] + Alpha * Error * Input[i];
		}
	}
}

float CalcError(void)
{
	float Error;
	float Sum = 0;

	for (int j = 0; j < MAXOUTPUT; j++)
	{
		Error =  Output[j] - Target[j];
		Sum += Error * Error;
	}

	return (float)sqrt(Sum);
}

void Training(int n)
{
	SetInputData(n);
	ProcessNet();
	SetTargetData(n);
	AdjustWeight();
}

void Test(int n)
{
	SetTestInputData(n);
	ProcessNet();
	//SetTargetData(n);
}