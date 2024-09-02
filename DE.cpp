#include <iostream>
#include <conio.h>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iomanip>
#include <math.h>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>
#include <vector>
#define getrandom(min, max) (static_cast<long long>(rand()) * (max - min + 1) / RAND_MAX) + min
#define gen 2000	 // number of iterations (number of generations)
#define pSize 40	 // number of chromosomes (population size)
#define dimension 30 // number of bits (dimension size)
#define M_PI 3.14159265358979323846
using namespace std;

double chromosome[pSize][dimension]; // chromosome
double paroff[4][dimension];		 // parent and offspring
double fit[pSize];					 // fitness value for each chromosome
double r = 0, dcp = 0.7, dmp = 0.01, gcp = 0, gmp = 0;
int crb = 0, mb1 = 0, mb2 = 0;
int rp1 = 0, rp2 = 0;
double mb1v = 0, mb2v = 0;
double fv = 0, sumFit = 0;
double fit1 = 0, fit2 = 0;
double tfit[4];
// int rangeMin = 5120, rangeMax = 5120, rangeDiv = 1000;
int rangeMin[10] = {5120, 32768, 5120, 5000, 5120, 600000, 1000, 65536, 5000, 1000};
int rangeMax[10] = {5120, 32768, 5120, 5000, 5120, 600000, 1000, 65536, 5000, 1000};
int rangeDiv[10] = {1000, 1000, 1000, 1000, 1000, 600000, 1000, 1000, 1000, 1000};
int lFvIndex = 0;
double lFv = 0;
int parent1, parent2;
int iteration = 1;
// string outfile1 = ".\\GAFolder\\GAResult1.txt"; // all results place in single folder

//------------------------------------------------------------------------------------------------------------------------------
// Please write comment and tag your name while doing the coding
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
// Fitness Function
//------------------------------------------------------------------------------------------------------------------------------
double Fitness(double a[], int b)
{
	//---------------------------------------------------------------------------------------------------------------------------
	// Insert Benchmark Functions
	//---------------------------------------------------------------------------------------------------------------------------
	switch (b)
	{
	case (0): // benchmark function 1 (Sphere function)
	{
		for (int j = 0; j < dimension; j++)
		{
			fv = pow(a[j], 2);
			sumFit = sumFit + fv;
		}
	}

	break;
	case (1): // benchmark function 2 (Ackley function)
	{
		double sum1 = 0.0;
		double sum2 = 0.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += pow(a[j], 2);
			sum2 += cos(2 * M_PI * a[j]);
		}
		double term1 = -20.0 * exp(-0.2 * sqrt(sum1 / dimension));
		double term2 = exp(sum2 / dimension);
		sumFit = term1 - term2 + 20.0 + exp(1.0);
	}

	break;
	case (2): // benchmark function 3 (Rastrigin function)
	{
		for (int j = 0; j < dimension; j++)
		{
			sumFit += (a[j] * a[j] - 10 * cos(2 * M_PI * a[j]));
		}
		sumFit += 10 * dimension; // This is to ensure it matches the standard Rastrigin function formula
	}

	break;
	case (3): // benchmark function 4 (Zakharov function)
	{
		double sum1 = 0.0;
		double sum2 = 0.0;
		double sum3 = 0.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += pow(a[j], 2);
			sum2 += 0.5 * (j + 1) * a[j];
		}

		sum3 = pow(sum2, 2) + pow(sum2, 4);
		sumFit = sum1 + sum3;
	}

	break;
	case (4): // benchmark function 5(Axis Parallel Hyper-Ellipsoid function)
	{
		for (int j = 0; j < dimension; j++)
		{
			fv = (j + 1) * pow(a[j], 2);
			sumFit = sumFit + fv;
		}
	}

	break;
	case (5): // benchmark function 6	(Griewank function)
	{
		double sum1 = 0.0;
		double product = 1.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += pow(a[j], 2) / 4000.0;
			product *= cos(a[j] / sqrt(j + 1));
		}
		sumFit = sum1 - product + 1.0;
	}

	break;

	case (6): // benchmark function 7 (Sum of Different Powers function)
	{
		for (int j = 0; j < dimension; j++)
		{
			double fv = pow(fabs(a[j]), j + 1);
			sumFit += fv;
		}
	}
	break;
	case (7): // benchmark function 8 (Rotated Hyper-Ellipsoid function)
	{
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < i; j++)
			{
				fv = pow(a[j], 2);
				sumFit = sumFit + fv;
			}
		}
	}
	break;
	case (8): // benchmark function 9 (Schwefel 2.22 function)
	{
		double sum1 = 0.0;
		double product = 1.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += fabs(a[j]);
			product *= fabs(a[j]);
		}
		sumFit = sum1 + product;
	}

	break;
	case (9): // benchmark function 10 (Exponential Function)
	{
		double fx = 0.0;
		for (int j = 0; j < dimension; j++)
		{
			fv = pow(a[j], 2);
			sumFit = sumFit + fv;
		}
		sumFit = -exp(-0.5 * sumFit);
	}
	break;
	default:
		cout << "fitness errors" << endl;
		break;
	}
	return sumFit;
}

void GeneratePopulation(int b)
{ // get benchmark value (every benchmark has diff. rangemin, rangemax and rangediv)
	for (int i = 0; i < pSize; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			int min = rangeMin[b];
			int max = rangeMax[b];
			r = getrandom(-min, max);
			r = r / rangeDiv[b];
			chromosome[i][j] = r;
		}
		// cout<<"Chromosome "<<i+1<<endl;
		// for (int j = 0; j < dimension; j++){
		// cout << setprecision(6) << chromosome[i][j] << "\t";
		//}
		// cout << endl<< endl;
	}
}
void FitnessValue(int b)
{
	for (int i = 0; i < pSize; i++)
	{
		fit[i] = Fitness(chromosome[i], b);
		// cout << setprecision(6) << fit[i] << endl;
		sumFit = 0;
	}
}

int main()
{
	clock_t start, end;
	srand(time(0));
	// Each operation technique will run ten benchmark (each benchmark 10 times)
	for (int b = 0; b < 10; b++)
	{ // Benchmark functioN
		for (int n = 0; n < 10; n++)
		{																			   // 10 times
			string outfile1 = ".\\DEFolder\\DEResult" + to_string(iteration) + ".txt"; // all results place in single folder
			ofstream outfileo1(outfile1.c_str(), ios::trunc);

			cout << "Benchmark Funtion " << (b + 1) << " \n\n";
			cout << "Times " << (n + 1) << " \n\n";

			// CPU Time
			start = clock();
			//---------------------------------------------------------------------------------------------------------------------------
			// Generate Population
			//---------------------------------------------------------------------------------------------------------------------------
			GeneratePopulation(b);

			// getch();

			//---------------------------------------------------------------------------------------------------------------------------
			// Fitness Evaluation
			//---------------------------------------------------------------------------------------------------------------------------
			FitnessValue(b);
			//---------------------------------------------------------------------------------------------------------------------------
			// Termination Criteria (Maximum Generation)
			//---------------------------------------------------------------------------------------------------------------------------
			for (int z = 0; z < gen; z++)
			{




				//------------------------------------------------------------------------------------------------------------------------
				// Fitness Evaluation
				//------------------------------------------------------------------------------------------------------------------------
				fit1 = Fitness(paroff[2], b);
				sumFit = 0;

				fit2 = Fitness(paroff[3], b);
				sumFit = 0;

				tfit[2] = fit1;
				tfit[3] = fit2;
				//------------------------------------------------------------------------------------------------------------------------

				//--------------------------------------------------------------------------------------------------------------------------
				// Replacement operation technique function calling
				//--------------------------------------------------------------------------------------------------------------------------




				lFv = pow(999, 30);
				for (int j = 0; j < pSize; j++)
				{
					if (fit[j] < lFv)
					{
						lFv = fit[j];
						lFvIndex = j;
					}
				}

				fit1 = 0;
				fit2 = 0;
				outfileo1 << setprecision(6) << lFv << endl;
			}
			lFv = pow(999, 30);
			for (int j = 0; j < pSize; j++)
			{
				if (fit[j] < lFv)
				{
					lFv = fit[j];
					lFvIndex = j;
				}
			}

			// cout << endl
			//<< endl;
			outfileo1 << endl
					  << endl;
			// cout << setprecision(6) << lFv << " " << lFvIndex + 1 << endl
			//	<< endl;

			for (int j = 0; j < dimension; j++)
			{
				// cout << setprecision(6) << chromosome[lFvIndex][j] << "\t";
				outfileo1 << setprecision(6) << chromosome[lFvIndex][j] << "\n";
			}

			// cout << endl;
			outfileo1 << endl;
			end = clock();
			// cout << "Time required for execution: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds." << "\n\n";
			outfileo1 << (double)(end - start) / CLOCKS_PER_SEC << "\n\n";
			iteration++;

			for (int i = 0; i < pSize; i++)
			{
				for (int j = 0; j < dimension; j++)
				{
					chromosome[i][j] = 0.0f;
				}
			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < dimension; j++)
				{
					paroff[i][j] = 0.0f;
				}
			}
		}
	}
	return 0;
}