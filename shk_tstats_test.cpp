// Sample usage program for TStats class.

#include <iostream>
#include "shk_stats.h"
using namespace std;
using namespace shk;

const int N=16;

// two data sets A and B
double A[] = { 88., 12., 63., 34., 77., 95., 12., 2. ,
               99., 6. , 88., 45., 76., 46., 3. , 12. };
double T[] = { 1., 1.5, 3.3, 6., 7.2, 8.5, 9., 11.6,
               13.25, 16.1, 41., 59., 66.6, 78., 147., 192.5 };

TStats X(0.,100.,10);


int main()
{
    for (int i=0; i<N; i++)
        X.takeSample(A[i],T[i]);

    X.printTStats("A",10,4,1);
    X.printHistogram("A",8,4);

    X.resetTStats();

    for (int i = 0; i<N; i++)
        X.takeSample(A[i],T[i]);

    X.printTStats("A", 10, 4, 1);
    X.printHistogram("A", 8, 4);

    return 0;
}












