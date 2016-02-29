// Sample usage program for Stats class.

#include <iostream>
#include "shk_stats.h"
using namespace std;
using namespace shk;

const int N=16;

// two data sets A and B
double A[] = { 88., 12., 0. , 34., 77., 95., 12., 2. ,
               99., 6. , 88., 45., 76., 46., 3. , 12. };
double B[] = { 11., 52., 30., 61., 17., 5., 62., 12.,
               25., 16., 81., 29., 56., 46., 42., 92. };

Stats X(0.,100.,10);


int main()
{
    for (int i=0; i<N; i++)
        X.takeSample(A[i]);

    X.printStats("A",10,4,1);
    X.printHistogram("A",8,4);

    X.resetStats();

    for (int i = 0; i<N; i++)
        X.takeSample(B[i]);

    X.printStats("B", 10, 4, 1);
    X.printHistogram("B", 8, 4);

    return 0;
}












