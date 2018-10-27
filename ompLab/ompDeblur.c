//OpenMP version.  Edit and submit only this file.
/* Enter your details below
 * Name : Changyu Yan
 * UCLA ID: 304-566-451
 * Email id: changyannnnnn@gmail.com
 */

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int OMP_xMax;
#define xMax OMP_xMax
int OMP_yMax;
#define yMax OMP_yMax
int OMP_zMax;
#define zMax OMP_zMax
int XMAX_YMAX; 
int id, i; 


int OMP_Index(int x, int y, int z)
{
	return (z * XMAX_YMAX + y * xMax + x);
}

// Index(x, y, z) = (z * yMax + y) * xMax + x
// Index(x, y, z) =  z * XMAX_YMAX + y * xMax + x

#define Index(x, y, z) OMP_Index(x, y, z)

double OMP_SQR(double x)
{
	return x * x;
}
#define SQR(x) OMP_SQR(x)

double* OMP_conv;
double* OMP_g;

void OMP_Initialize(int xM, int yM, int zM)
{
	xMax = xM;
	yMax = yM;
	zMax = zM;
	XMAX_YMAX = xM * zM; 
	assert(OMP_conv = (double*)malloc(sizeof(double) * xMax * yMax * zMax));
	assert(OMP_g = (double*)malloc(sizeof(double) * xMax * yMax * zMax));
}
void OMP_Finish()
{
	free(OMP_conv);
	free(OMP_g);
}
void OMP_GaussianBlur(double *u, double Ksigma, int stepCount)
{
    omp_set_num_threads(16);
    
	double lambda = (Ksigma * Ksigma) / (double)(2 * stepCount);
	double nu = (1.0 + 2.0*lambda - sqrt(1.0 + 4.0*lambda))/(2.0*lambda);
	int x, y, z, step;
	double boundryScale = 1.0 / (1.0 - nu);
	double postScale = pow(nu / lambda, (double)(3 * stepCount));


	for(step = 0; step < stepCount; step++)
	{
		for(z = 0; z < zMax; z++)
		{
            #pragma omp parallel for 
			for(y = 0; y < yMax; y++)
			{
				id = z * XMAX_YMAX + y * xMax; 

				u[id] *= boundryScale;
				
                for(x = 1; x < xMax; x++)
                {
					id += x; 
                    u[id] += u[id - 1] * nu;
                }
                for(x; x >= 0; x--)
                {
                    u[id] += u[id + 1] * nu;
                }
			}
		}
		
		// CHECKPOINT!!!!!!!!!
		
		for(z = 0; z < zMax; z++)
		{
			i = z * XMAX_YMAX; 
			#pragma omp parallel for
			for(x = 0; x < xMax; x++)
			{
				id = i + x; 
				u[id] *= boundryScale;
			}
			for(y = 1; y < yMax; y++)
			{
				// Index(x, y, z) =  z * XMAX_YMAX + y * xMax + x
				 id = i + y * xMax; 
				for(x = 0; x < xMax; x++)
				{
					
					u[id] += u[id - xMax] * nu;
					id += 1; 
				}
				u[Index(x, yMax - 1, z)] *= boundryScale;
			}
			for(y = yMax - 2; y >= 0; y--)
			{
				id = i + y * xMax; 
				for(x = 0; x < xMax; x++)
				{
					u[id] += u[id + xMax] * nu; 
					id += 1; 
				}
			}
		}
		
		
		for(y = 0; y < yMax; y++)
		{
			u[y * xMax + x] *= boundryScale;
			i = y * xMax;
			for(z = 1; z < zMax; z++)
			{
				id = z * XMAX_YMAX + i; 
				for(x = 0; x < xMax; x++)
				{
					id += 1; 
					u[id] = u[id - XMAX_YMAX] * nu;
				}
			}
			
			z -= 2;
			for(z; z >= 0; z--)
			{
				u[id - XMAX_YMAX] *= boundryScale;
				id = z * XMAX_YMAX + i; 
				for(x = 0; x < xMax; x++)
				{ 
					id += 1; 
					u[id] += u[id + XMAX_YMAX] * nu;
				}
				
			}
		}
	}
	
	for(y = 0; y < yMax; y++)
	{
		i = y * xMax; 
		for(z = 0; z < zMax; z++)
		{
			id = z * XMAX_YMAX + i; 
			for(x = 0; x < xMax; x++)
			{
				id += 1; 
				u[id] *= postScale;
			}
			
		}
	}
	
}
void OMP_Deblur(double* u, const double* f, int maxIterations, double dt, double gamma, double sigma, double Ksigma)
{
	double epsilon = 1.0e-7;
	double sigma2 = SQR(sigma);
	int x, y, z, iteration, i;
	int converged = 0;
	int lastConverged = 0;
	int fullyConverged = (xMax - 1) * (yMax - 1) * (zMax - 1);
	double* conv = OMP_conv;
	double* g = OMP_g;

	for(iteration = 0; iteration < maxIterations && converged != fullyConverged; iteration++)
	{
		for(y = 1; y < yMax - 1; y++)
		{
			i =  y * xMax; 
			for(z = 1; z < zMax - 1; z++)
			{
				id = z * XMAX_YMAX + i;
				for(x = 1; x < xMax - 1; x++)
				{
					// Index(x, y, z) =  z * XMAX_YMAX + y * xMax + x
					id += 1; 
					double val = u[id]; 
					g[id] = 1.0 / sqrt(epsilon + 
						SQR(val - u[id + 1]) + 
						SQR(val - u[id - 1]) + 
						SQR(val - u[id + xMax]) + 
						SQR(val - u[id - xMax]) + 
						SQR(val - u[id + XMAX_YMAX]) + 
						SQR(val - u[id - XMAX_YMAX]));
				}
			}
		}
		// memcpy(conv, u, sizeof(double) * XMAX_YMAX * zMax);
		// OMP_GaussianBlur(conv, Ksigma, 3);
		// for(y = 0; y < yMax; y++)
		// {
		// 	i = y * xMax; 
		// 	for(z = 0; z < zMax; z++)
		// 	{
		// 		id = z * XMAX_YMAX + i;
		// 		for(x = 0; x < xMax; x++)
		// 		{
		// 			id += 1; 
		// 			double r = conv[id] * f[id] / sigma2;
		// 			r = (r * (2.38944 + r * (0.950037 + r))) / (4.65314 + r * (2.57541 + r * (1.48937 + r)));
		// 			conv[id] -= f[id] * r;
		// 		}
		// 	}
		// }
		// OMP_GaussianBlur(conv, Ksigma, 3);
		converged = 0;
		for(y = 1; y < yMax - 1; y++)
		{
			i = y * xMax; 
			for(z = 1; z < zMax - 1; z++)
			{
				id = z * XMAX_YMAX + i ; 
				for(x = 1; x < xMax - 1; x++)
				{
					// Index(x, y, z) =  z * XMAX_YMAX + y * xMax + x

					id += 1; 
					double oldVal = u[id];
					double newVal = (u[id] + dt * ( 
						u[id-1] * g[id-1] + 
						u[id+1] * g[id+1] + 
						u[id - xMax] * g[id - xMax] + 
						u[id + xMax] * g[id + xMax] + 
						u[id - XMAX_YMAX] * g[id - XMAX_YMAX] + 
						u[id + XMAX_YMAX] * g[id + XMAX_YMAX] - gamma * conv[id])) /
						(1.0 + dt * (g[id + 1] + g[id - 1] + g[id + xMax] + g[id - xMax] + g[id + XMAX_YMAX] + g[id - XMAX_YMAX]));
					if(fabs(oldVal - newVal) < epsilon)
					{
						converged++;
					}
					u[id] = newVal;
				}
			}
		}
		if(converged > lastConverged)
		{
			printf("%d pixels have converged on iteration %d\n", converged, iteration);
			lastConverged = converged;
		}
	}
}

