#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>
#include <iomanip>
#include <string>
#define gen 2000	 // number of generations
#define pSize 40	 // population size
#define dimension 30 // problem dimension
#define M_PI 3.14159265358979323846

using namespace std;

double population[pSize][dimension];
double trial[dimension];
double fit[pSize];
double F = 0.3;	 // Differential weight (F)
double CR = 0.8; // Crossover probability (CR)
double fv = 0, sumFit = 0;
int rangeMin[10] = {5120, 32768, 5120, 5000, 5120, 600000, 1000, 65536, 5000, 1000};
int rangeMax[10] = {5120, 32768, 5120, 5000, 5120, 600000, 1000, 65536, 5000, 1000};
int rangeDiv[10] = {1000, 1000, 1000, 1000, 1000, 600000, 1000, 1000, 1000, 1000};

// Function to generate a random double between min and max
double randomDouble(double min, double max)
{
	return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

// Benchmark functions (same as in the original code)
double Fitness(double a[], int b)
{
	sumFit = 0;
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

// Initialization process
void InitializePopulation(int b)
{
	for (int i = 0; i < pSize; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			double r = randomDouble(0, 1) * (rangeMax[b] - rangeMin[b]) + rangeMin[b];
			population[i][j] = r;
		}
		fit[i] = Fitness(population[i], b);
	}
}

// DE/best/1 Mutation Strategy
void Mutation(int i, int b)
{
	int bestIndex = 0;
	double bestFitness = fit[0];

	// Find the best individual
	for (int k = 1; k < pSize; k++)
	{
		if (fit[k] < bestFitness)
		{
			bestFitness = fit[k];
			bestIndex = k;
		}
	}

	int r1, r2;
	do
	{
		r1 = rand() % pSize;
		r2 = rand() % pSize;
	} while (r1 == r2 || i == r1 || i == r2);

	for (int j = 0; j < dimension; j++)
	{
		trial[j] = population[bestIndex][j] + F * (population[r1][j] - population[r2][j]);
		// Ensure trial vector is within bounds
		if (trial[j] < -rangeMax[b] / rangeDiv[b])
			trial[j] = -rangeMax[b] / rangeDiv[b];
		if (trial[j] > rangeMax[b] / rangeDiv[b])
			trial[j] = rangeMax[b] / rangeDiv[b];
	}
}

// Crossover
void Crossover(int i)
{
	for (int j = 0; j < dimension; j++)
	{
		if (randomDouble(0, 1) < CR || j == rand() % dimension)
		{
			// Keep the mutated value
		}
		else
		{
			trial[j] = population[i][j];
		}
	}
}

// Selection
void Selection(int i, int b)
{
	double trialFitness = Fitness(trial, b);
	if (trialFitness <= fit[i])
	{
		for (int j = 0; j < dimension; j++)
		{
			population[i][j] = trial[j];
		}
		fit[i] = trialFitness;
	}
}

int main()
{
	srand(time(0));
	clock_t start, end;
	int count = 1;
	for (int b = 0; b < 10; b++)
	{ // Benchmark functions
		for (int n = 0; n < 10; n++)
		{ // 10 times for each benchmark
			string outfile = "./DEFolder/DEResult_" + to_string(count) + ".txt";
			ofstream outfile_stream(outfile, ios::trunc);

			cout << "Benchmark Funtion " << (b + 1) << " \n\n";
			cout << "Times " << (n + 1) << " \n\n";

			start = clock();

			InitializePopulation(b);

			for (int g = 0; g < gen; g++)
			{
				for (int i = 0; i < pSize; i++)
				{
					Mutation(i, b);
					Crossover(i);
					Selection(i, b);
				}

				// Find best fitness in this generation
				double bestFitness = fit[0];
				int bestIndex = 0;
				for (int i = 1; i < pSize; i++)
				{
					if (fit[i] < bestFitness)
					{
						bestFitness = fit[i];
						bestIndex = i;
					}
				}

				outfile_stream << setprecision(6) << bestFitness << endl;
			}

			// Output final best solution
			double bestFitness = fit[0];
			int bestIndex = 0;
			for (int i = 1; i < pSize; i++)
			{
				if (fit[i] < bestFitness)
				{
					bestFitness = fit[i];
					bestIndex = i;
				}
			}
			outfile_stream << endl
						   << endl;

			outfile_stream << setprecision(6) << bestFitness << endl;
			for (int j = 0; j < dimension; j++)
			{
				outfile_stream << setprecision(6) << population[bestIndex][j] << endl;
			}

			// cout << endl;
			outfile_stream << endl;
			end = clock();
			// cout << "Time required for execution: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds." << "\n\n";
			outfile_stream << (double)(end - start) / CLOCKS_PER_SEC << "\n\n";

			count++;
		}
	}

	return 0;
}
