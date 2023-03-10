#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <omp.h>
#include <fstream>

using namespace std;

struct Body {
    double mass;
    double r[2];
    double v[2];
};
double deta_t = 0.001, G = 6.673 * pow(10, -11);

int cal(int a, int b)
{
    clock_t start, end;

    int n = a;
    double r_ij[2];
    int threads;
    vector<vector <double>> fx(n, vector<double>(n));
    vector<vector <double>> fy(n, vector<double>(n));
    vector<vector <double>> tempx(n, vector<double>(n));
    vector<vector <double>> tempy(n, vector<double>(n));
    vector<double> Fx(n);
    vector<double> Fy(n);
    vector<double> ax(n);
    vector<double> ay(n);

    srand(time(NULL)); // initialize random seed

    // Create a vector of n bodies with mass set to 1.0, random position, and initial velocity (0,0)
    vector<Body> bodies(n);
#pragma omp parallel for num_threads(2) 
    for (int i = 0; i < n; i++) {
        bodies[i].mass = 1.0;
        bodies[i].r[0] = (double)rand() / RAND_MAX; // random x coordinate between 0 and 1
        bodies[i].r[1] = (double)rand() / RAND_MAX; // random y coordinate between 0 and 1
        bodies[i].v[0] = 0.0;
        bodies[i].v[1] = 0.0;
    }

    // Print out the properties of each body
    //for (int i = 0; i < n; i++) {
    //    cout << "Body " << i << ":" << endl;
    //    cout << "  mass = " << bodies[i].mass << endl;
    //    cout << "  position = (" << bodies[i].r[0] << ", " << bodies[i].r[1] << ")" << endl;
    //    cout << "  velocity = (" << bodies[i].v[0] << ", " << bodies[i].v[1] << ")" << endl;
    //}
    start = clock();
    // Update the properties 
    omp_set_nested(1);//Enable nest loop

    for (int k = 0; k < 4; k++)
    {
#pragma omp parallel for num_threads(b)        
        for (int i = 0; i < n; i++)
        {
            Fx[i] = 0.0;
            Fy[i] = 0.0;
            threads = omp_get_num_threads();
#pragma omp parallel for num_threads(b)
            for (int j = 0; j < n; j++)
            {
                if (i < j)
                {
                    r_ij[0] = bodies[j].r[0] - bodies[i].r[0];
                    r_ij[1] = bodies[j].r[1] - bodies[i].r[1];
                    fx[i][j] = (G * bodies[i].mass * bodies[j].mass * r_ij[0]) / pow(sqrt(pow(r_ij[0], 2) + pow(r_ij[1], 2)), 3);
                    fy[i][j] = (G * bodies[i].mass * bodies[j].mass * r_ij[1]) / pow(sqrt(pow(r_ij[0], 2) + pow(r_ij[1], 2)), 3);
                    tempx[i][j] = fx[i][j];
                    tempy[i][j] = fy[i][j];
                }
                else if (i > j)
                {
                    fx[i][j] = -tempx[j][i];
                    fy[i][j] = -tempy[j][i];
                }
#pragma omp atomic
                Fx[i] += fx[i][j];
#pragma omp atomic
                Fy[i] += fy[i][j];

            }
            ax[i] = Fx[i] / bodies[i].mass;
            ay[i] = Fy[i] / bodies[i].mass;
#pragma omp atomic
            bodies[i].v[0] += deta_t * ax[i];
#pragma omp atomic
            bodies[i].v[1] += deta_t * ay[i];
#pragma omp atomic
            bodies[i].r[0] += deta_t * bodies[i].v[0];
#pragma omp atomic
            bodies[i].r[1] += deta_t * bodies[i].v[1];

        }
    }
    end = clock();

    // Print out the properties of each body
    //for (int i = 0; i < n; i++) {
    //    cout << "Body " << i << ":" << endl;
    //    cout << "  mass = " << bodies[i].mass << endl;
    //    cout << "  position = (" << bodies[i].r[0] << ", " << bodies[i].r[1] << ")" << endl;
    //    cout << "  velocity = (" << bodies[i].v[0] << ", " << bodies[i].v[1] << ")" << endl;
    //}
    double IPS = (4 * n * n) / (2 * (double(end - start) / CLOCKS_PER_SEC));
    //ofstream outfile;
    //outfile.open("half_para.txt", ios::out | ios::app);
    cout << "Threads : " << threads << ",Bodies:" << n << ",Performance:" << IPS << endl;
    //outfile.close();
    return 0;
}

int main() {
    

    int a[] = { 200,500,1000,2000,5000,10000,20000 }; // number of bodies
    int num[] = { 1,2,4,8,16,24 };
    for (int nu = 0; nu < sizeof(num) / sizeof(num[0]); nu++) 
    {
        for (int m = 0; m < sizeof(a) / sizeof(a[0]); m++)
        {
            int n = a[m];
 
            if (n < 5000)
            {
                for (int i = 0; i < 5; i++) 
                {
                    cal(a[m], num[nu]);
                }
                
            }
            else
            {
                cal(a[m], num[nu]);
            }

    }
    
    }

    return 0;
}

