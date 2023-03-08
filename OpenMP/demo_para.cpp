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
    int threads;
    double r_ij[2], f_ij[2], F_i[2], a_i[2];

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
    omp_set_nested(1);
#pragma omp parallel for num_threads(b)// Set number of threads
    for (int k = 0; k < 4; k++)
    {
        threads = omp_get_num_threads();
        for (int i = 0; i < n; i++)
        {
            F_i[0] = 0.0;
            F_i[1] = 0.0;   
#pragma omp parallel for num_threads(b)
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                {
                    r_ij[0] = bodies[j].r[0] - bodies[i].r[0];
                    r_ij[1] = bodies[j].r[1] - bodies[i].r[1];
                    f_ij[0] = (G * bodies[i].mass * bodies[j].mass * r_ij[0]) / pow(sqrt(pow(r_ij[0], 2) + pow(r_ij[1], 2)), 3);
                    f_ij[1] = (G * bodies[i].mass * bodies[j].mass * r_ij[1]) / pow(sqrt(pow(r_ij[0], 2) + pow(r_ij[1], 2)), 3);
#pragma omp atomic//Avoid false sharing
                    F_i[0] += f_ij[0];
#pragma omp atomic
                    F_i[1] += f_ij[1];
                }
                    
            }
            a_i[0] = F_i[0] / bodies[i].mass;
            a_i[1] = F_i[1] / bodies[i].mass;
#pragma omp atomic
            bodies[i].v[0] += deta_t * a_i[0];
#pragma omp atomic
            bodies[i].v[1] += deta_t * a_i[1];
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
    double IPS = (4 * n * n) / (double(end - start) / CLOCKS_PER_SEC);
    //ofstream outfile;
    //outfile.open("demo_para.txt", ios::out | ios::app);
    //outfile << "Threads : " << threads << ",Bodies:" << n << ",Performance:" << IPS << endl;
    //outfile.close();
    cout << "Threads : " << threads << ",Bodies:" << n << ",Performance:" << IPS << endl;
    return 0;
}

int main() {


    int a[] = { 200,500,1000, 2000,5000,10000,20000 }; // number of bodies
    int num[] = { 1, 2, 4, 8 };
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

