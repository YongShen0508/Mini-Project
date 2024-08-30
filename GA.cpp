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
int rangeMax[10] = {5120, 32768, 5120, 5000, 5120, 600000, 1000, 655326, 5000, 1000};
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
	double sum1, sum2, sum3, product, term1, term2;

	//---------------------------------------------------------------------------------------------------------------------------
	// Insert Benchmark Functions
	//---------------------------------------------------------------------------------------------------------------------------
	switch (b)
	{
	case (0): // benchmark function 1 (Sphere function)
		for (int j = 0; j < dimension; j++)
		{
			fv = pow(a[j], 2);
			sumFit = sumFit + fv;
		}
		break;
	case (1): // benchmark function 2 (Ackley function)
		sum1 = 0.0;
		sum2 = 0.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += pow(a[j], 2);
			sum2 += cos(2 * M_PI * a[j]);
		}
		term1 = -20.0 * exp(-0.2 * sqrt(sum1 / dimension));
		term2 = exp(sum2 / dimension);
		sumFit = term1 - term2 + 20.0 + exp(1.0);
		break;
	case (2): // benchmark function 3 (Rastrigin function)
		for (int j = 0; j < dimension; j++)
		{
			sumFit += (a[j] * a[j] - 10 * cos(2 * M_PI * a[j]));
		}
		break;
	case (3): // benchmark function 4 (Zakharov function)
		sum1 = 0.0;
		sum2 = 0.0;
		sum3 = 0.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += pow(a[j], 2);
			sum2 += 0.5 * (j + 1) * a[j];
		}

		sum3 = pow(sum2, 2) + pow(sum2, 4);
		sumFit = sum1 + sum3;
		break;
	case (4): // benchmark function 5(Axis Parallel Hyper-Ellipsoid function)
		for (int j = 0; j < dimension; j++)
		{
			fv = (j + 1) * pow(a[j], 2);
			sumFit = sumFit + fv;
		}
		break;
	case (5): // benchmark function 6	(Griewank function)
		sum1 = 0.0;
		product = 1.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += pow(a[j], 2) / 4000.0;
			product *= cos(a[j] / sqrt(j + 1));
		}
		sumFit = sum1 - product + 1.0;
		break;

	case (6):
		// benchmark function 7 (Sum of Different Powers function)
		for (int j = 0; j < dimension; j++)
		{
			double fv = pow(fabs(a[j]), j + 1);
			sumFit += fv;
		}
		break;
	case (7): // benchmark function 8 (Rotated Hyper-Ellipsoid function)
		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < i; j++)
			{
				fv = pow(a[j], 2);
				sumFit = sumFit + fv;
			}
		}
		break;
	case (8): // benchmark function 9 (Schwefel 2.22 function)
		sum1 = 0.0;
		product = 1.0;
		for (int j = 0; j < dimension; j++)
		{
			sum1 += fabs(a[j]);
			product *= fabs(a[j]);
		}
		sumFit = sum1 + product;
		break;
	case (9): // benchmark function 10 (Exponential Function)
		for (int j = 0; j < dimension; j++)
		{
			fv = pow(a[j], 2);
			sumFit = sumFit + fv;
		}
		sumFit = -exp(-0.5 * sumFit);
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

void SelectionOperationTechnique(int i)
{
	switch (i)
	{
	case (0): // Roulette Wheel Selection
	{
		double totalFitness = 0;
		for (int j = 0; j < pSize; j++)
		{
			totalFitness += fit[j];
		}

		// Select parent1
		double random1 = ((double)rand() / RAND_MAX) * totalFitness;
		double partialSum = 0;
		for (parent1 = 0; parent1 < pSize; parent1++)
		{
			partialSum += fit[parent1];
			if (partialSum >= random1)
				break;
		}

		// Select parent2
		double random2;
		do
		{
			random2 = ((double)rand() / RAND_MAX) * totalFitness;
			partialSum = 0;
			for (parent2 = 0; parent2 < pSize; parent2++)
			{
				partialSum += fit[parent2];
				if (partialSum >= random2)
					break;
			}
		} while (parent2 == parent1);

		tfit[0] = fit[parent1];
		tfit[1] = fit[parent2];
		for (int j = 0; j < dimension; j++)
		{
			paroff[0][j] = chromosome[parent1][j];
			paroff[1][j] = chromosome[parent2][j];
		}
	}
	break;

	case (1): // Tournament Selection
	{
		int tournamentSize = 5;

		// Select parent1
		parent1 = rand() % pSize;
		for (int j = 1; j < tournamentSize; j++)
		{
			int competitor = rand() % pSize;
			if (fit[competitor] < fit[parent1])
			{
				parent1 = competitor;
			}
		}

		// Select parent2
		do
		{
			parent2 = rand() % pSize;
			for (int j = 1; j < tournamentSize; j++)
			{
				int competitor = rand() % pSize;
				if (fit[competitor] < fit[parent2])
				{
					parent2 = competitor;
				}
			}
		} while (parent2 == parent1);

		tfit[0] = fit[parent1];
		tfit[1] = fit[parent2];
		for (int j = 0; j < dimension; j++)
		{
			paroff[0][j] = chromosome[parent1][j];
			paroff[1][j] = chromosome[parent2][j];
		}
	}
	break;

	default:
		cout << "Selection errors" << endl;
		break;
	}
}

void MutationOperationTechnique(int j, int b)
{
	switch (j)
	{
	case (0): // Flipping Mutation
	{
		for (int i = 2; i < 4; i++)
		{
			for (int k = 0; k < dimension; k++)
			{
				gmp = (rand() % 1000000) / 1000000;
				if (gmp <= dmp)
				{
					// Flip the gene (invert its sign)
					paroff[i][k] = -paroff[i][k];
				}
			}
		}
	}

	break;

	case (1): // Uniform Mutation
	{
		for (int i = 2; i < 4; i++)
		{
			for (int k = 0; k < dimension; k++)
			{
				gmp = (rand() % 1000000) / 1000000;
				if (gmp <= dmp)
				{
					// Uniform mutation: replace the gene with a random value within the allowed range
					double newValue = getrandom(-rangeMin[b], rangeMax[b]) / rangeDiv[b];
					paroff[i][k] = newValue;
				}
			}
		}
	}

	break;

	default:
		cout << "Mutation errors" << endl;
		break;
	}
}
void CrossoverOperationTechnique(int k)
{
	switch (k)
	{

	case 0: // Uniform Crossover
	{
		for (int i = 0; i < dimension; ++i)
		{
			if ((rand() % 2) == 0)
			{
				paroff[2][i] = paroff[0][i];
				paroff[3][i] = paroff[1][i];
			}
			else
			{
				paroff[2][i] = paroff[1][i];
				paroff[3][i] = paroff[0][i];
			}
		}
		break;
	}
	case 1: // Two-Point Crossover
	{
		int point1 = getrandom(0, dimension - 1);
		int point2 = getrandom(0, dimension - 1);
		if (point1 > point2)
		{
			std::swap(point1, point2);
		}
		for (int i = 0; i < dimension; ++i)
		{
			if (i >= point1 && i <= point2)
			{
				paroff[2][i] = paroff[1][i];
				paroff[3][i] = paroff[0][i];
			}
			else
			{
				paroff[2][i] = paroff[0][i];
				paroff[3][i] = paroff[1][i];
			}
		}
		break;
	}

	default:
		cout << "Crossover operation technique error: invalid case" << endl;
		break;
	}
}
void ReplacementOperationTechnique(int l)
{
	switch (l)
	{
	case (0): // weak parent replacement
	{
		for (int i = 0; i < 4 - 1; i++)
		{
			for (int j = i; j < 4; j++)
			{
				if (tfit[i] > tfit[j])
				{
					swap(tfit[i], tfit[j]);
					swap(paroff[i], paroff[j]);
				}
			}
		}
		for (int k = 0; k < dimension; k++)
		{
			chromosome[parent1][k] = paroff[0][k];
		}
		fit[parent1] = tfit[0];

		for (int k = 0; k < dimension; k++)
		{
			chromosome[parent2][k] = paroff[1][k];
		}
		fit[parent2] = tfit[1];
		break;
	}
	case (1): // both parent replacement
	{
		for (int i = 0; i < dimension; i++)
		{
			chromosome[parent1][i] = paroff[2][i];
			chromosome[parent2][i] = paroff[3][i];
		}
		fit[parent1] = tfit[2];
		fit[parent2] = tfit[3];
	}
	case (2): // tournament replacement
	{
		for (int i = 2; i < 4; i++)
		{
			int posIndex[5]; // tournament size
			int lowestIndex = 0;
			bool isTrue = true;
			for (int j = 0; j < 5; j++)
			{
				do
				{
					posIndex[j] = rand() % pSize;
					for (int k = 0; k < j; k++)
					{
						if (posIndex[j] == posIndex[k])
						{
							isTrue = false;
							break;
						}
					}
				} while (!isTrue);
			}
			if (tfit[i] > fit[lowestIndex])
			{
				for (int z = 0; z < dimension; z++)
				{
					chromosome[lowestIndex][z] = paroff[i][z];
				}
				fit[lowestIndex] = tfit[i];
			}
		}
	}
	case (3): // Proposed replacement
	{		  // find average
		double totalFit = 0;
		for (int i = 0; i < pSize; i++)
		{
			totalFit += fit[i];
		}
		double avgFit = totalFit / pSize;
		cout << avgFit << endl
			 << totalFit << endl;
		int aboveAVG = 0;
		int posIndex[40]; // store chromosome index above avg
		for (int i = 0; i < pSize; i++)
		{
			if (fit[i] > avgFit)
			{
				posIndex[aboveAVG] = i;
				aboveAVG++;
			}
		}
		int targetPos[2];
		targetPos[0] = posIndex[rand() % aboveAVG];
		int secondIndex;
		do
		{
			secondIndex = posIndex[rand() % aboveAVG];
		} while (secondIndex == targetPos[0]);
		targetPos[1] = secondIndex;
		// weak parent replacement
		int tempParent1 = targetPos[0];
		int tempParent2 = targetPos[1];
		int tempFit[4] = {fit[tempParent1], fit[tempParent2], tfit[2], tfit[3]};
		double tempOff[4][4];
		for (int i = 0; i < dimension; i++)
		{
			tempOff[0][i] = chromosome[tempParent1][i]; // Copy chromosome of tempParent1
			tempOff[1][i] = chromosome[tempParent2][i]; // Copy chromosome of tempParent2
			tempOff[2][i] = paroff[2][i];				// Copy offspring from paroff[2]
			tempOff[3][i] = paroff[3][i];				// Copy offspring from paroff[3]
		}
		for (int i = 0; i < 4 - 1; i++)
		{
			for (int j = i + 1; j < 4; j++)
			{
				if (tempFit[i] > tempFit[j])
				{
					swap(tempFit[i], tempFit[j]);
					swap(tempOff[i], tempOff[j]);
				}
			}
		}
		for (int k = 0; k < dimension; k++)
		{
			chromosome[tempParent1][k] = tempOff[0][k];
		}
		fit[tempParent1] = tempFit[0];

		for (int k = 0; k < dimension; k++)
		{
			chromosome[tempParent2][k] = tempOff[1][k];
		}
		fit[tempParent2] = tempFit[1];
		break;
	}
	break;
	default:
		cout << "Replacement errors" << endl;
		break;
	}
}
int main()
{
	clock_t start, end;
	srand(time(0));
	// Each operation technique will run ten benchmark (each benchmark 10 times)
	for (int e = 0; e < 1; e++)
	{ // Selection operation technique (i)
		for (int f = 0; f < 1; f++)
		{ // CrossOver operation technique(k)
			for (int g = 0; g < 1; g++)
			{ // Mutation operation technique(j)
				for (int h = 0; h < 1; h++)
				{ // Replacement operation technique(l)

					for (int b = 0; b < 10; b++)
					{ // Benchmark function

						for (int n = 0; n < 10; n++)
						{																			   // 10 times
							string outfile1 = ".\\GAFolder\\GAResult" + to_string(iteration) + ".txt"; // all results place in single folder
							ofstream outfileo1(outfile1.c_str(), ios::trunc);
							// outfileo1 << "Selection OT " << (i + 1) << " \n\n";
							// outfileo1 << "Crossover OT " << (j + 1) << " \n\n";
							// outfileo1 << "Mutation OT " << (k + 1) << " \n\n";
							// outfileo1 << "Replacement OT " << (l + 1) << " \n\n";
							// outfileo1 << "Benchmark Funtion " << (b + 1) << " \n\n";
							// outfileo1 << "Times " << (n + 1) << " \n\n";
							//  cout << "Selection OT " << (i + 1) << " \n\n";
							//  cout << "Mutation OT " << (j + 1) << " \n\n";
							//  cout << "Crossover OT " << (k + 1) << " \n\n";
							//  cout << "Replacement OT " << (l + 1) << " \n\n";
							//  cout << "Benchmark Funtion " << (b + 1) << " \n\n";
							//  cout << "Times " << (n + 1) << " \n\n";

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
								//--------------------------------------------------------------------------------------------------------------------------
								// Selection operation technique function calling
								//--------------------------------------------------------------------------------------------------------------------------
								SelectionOperationTechnique(e);

								//--------------------------------------------------------------------------------------------------------------------------
								// Crossover operation technique function calling
								//--------------------------------------------------------------------------------------------------------------------------
								CrossoverOperationTechnique(f);

								//--------------------------------------------------------------------------------------------------------------------------
								// Mutation operation technique function calling
								//--------------------------------------------------------------------------------------------------------------------------
								MutationOperationTechnique(g, b);

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
								ReplacementOperationTechnique(h);

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
							//<< endl;

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
						}
					}
				}
			}
		}
	}
}
