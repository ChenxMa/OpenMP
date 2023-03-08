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

int cal(int a)
{
    clock_t start, end;

    int n = a;
    int threads;
    double r_ij[2], f_ij[2], F_i[2], a_i[2];

    srand(time(NULL)); // initialize random seed

    // Create a vector of n bodies with mass set to 1.0, random position, and initial velocity (0,0)
    vector<Body> bodies(n);
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
    for (int k = 0; k < 4; k++) //Numbers of iteration
    {
        threads = omp_get_num_threads();
        for (int i = 0; i < n; i++)//Calculate F_i
        {
            F_i[0] = 0.0;
            F_i[1] = 0.0;
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                {
                    r_ij[0] = bodies[j].r[0] - bodies[i].r[0];
                    r_ij[1] = bodies[j].r[1] - bodies[i].r[1];
                    f_ij[0] = (G * bodies[i].mass * bodies[j].mass * r_ij[0]) / pow(sqrt(pow(r_ij[0], 2) + pow(r_ij[1], 2)), 3);
                    f_ij[1] = (G * bodies[i].mass * bodies[j].mass * r_ij[1]) / pow(sqrt(pow(r_ij[0], 2) + pow(r_ij[1], 2)), 3);
                    F_i[0] += f_ij[0];
                    F_i[1] += f_ij[1];
                }

            }
            a_i[0] = F_i[0] / bodies[i].mass;
            a_i[1] = F_i[1] / bodies[i].mass;
            bodies[i].v[0] += deta_t * a_i[0];
            bodies[i].v[1] += deta_t * a_i[1];
            bodies[i].r[0] += deta_t * bodies[i].v[0];//Update movement
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
    double IPS = (4 * n * n) / (double(end - start) / CLOCKS_PER_SEC);//Calculate performance
    //ofstream outfile;
    //outfile.open("demo_seq.txt", ios::out | ios::app);
    cout << "Threads : " << threads << ",Bodies:" << n << ",Performance:" << IPS << endl;
    //cout << (double(end - start) / CLOCKS_PER_SEC) << endl;
    //outfile.close();
    return 0;
}

int main() {


    int a[] = { 200, 500, 1000, 2000, 5000, 10000 }; // number of bodies

    for (int m = 0; m < sizeof(a) / sizeof(a[0]); m++)
    {
        int n = a[m];

        if (n < 5000)
        {
            for (int i = 0; i < 5; i++)
            {
                cal(a[m]);
            }

        }
        else
        {
            cal(a[m]);
        }

    }
    return 0;
}
