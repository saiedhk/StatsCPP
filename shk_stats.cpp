// This file implements functions defined in Stats class.

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
using namespace std;

#include "shk_stats.h"
namespace shk 
{


/*---------------------------------------------------------------
Stats Functions
---------------------------------------------------------------*/

// class constructor (default, without histogram)
Stats::Stats(void)
{
    histo = false;
    resetStats();
}


// class constructor (with histogram)
Stats::Stats(double low, double high, int bins)
{
    if (!((low<high) && (bins>0))) // input check
    {
        cerr<< "fatal error: Stats::Stats() => bad parameters to construct Stats!\n";
        exit(1);
    }

    histo = true;
    lo = low;
    hi = high;
    nbin = bins;
    bin = (hi - lo) / nbin;
    histogram = new unsigned[nbin+2];
    resetStats();
}


// class destructor
Stats::~Stats(void)
{
    if (histo) delete [] histogram;;
}


// reset statistics
void Stats::resetStats(void)
{
    count = 0;
    sum = 0.;
    sumsq = 0.;
    min = DBL_MAX;
    max = -DBL_MAX;
    if (histo)
        for (int i=0; i < nbin+2; i++)
            histogram[i] = 0;
}


// take one data sample
void Stats::takeSample(double x)
{
    count++ ;
    sum += x;
    sumsq += (x * x);
    if (x < min) min = x;
    if (x > max) max = x;

    if (histo)
    {
        if ( x < lo ) { histogram[0]++; return; }
        if ( x > hi ) { histogram[nbin+1]++; return; }

        int i = ( (int) ((x - lo) / bin )) + 1;
        histogram[i]++;
    }
    return;
}


// compute mean of samples
double Stats::calcMean(void)
{
    if (count < 1)
    {
        cerr<< "fatal error: Stats::calcMean() => samples < 1 !\n";
        exit(1);
    }
    return (sum/count);
}


// compute unbiased variance of samples
double Stats::calcVariance(void)
{
    if (count < 2)
    {
        cerr<< "fatal error: Stats::calcVariance() => samples < 2 !\n";
        exit(1);
    }
    return (sumsq - (sum*sum)/count)/(count-1);
}


// compute unbiased standard deviation of samples
double Stats::calcStDev(void)
{
    return sqrt(calcVariance());
}


// print statistics
void Stats::printStats(char* varname, int width, int precision, int verbose)
{
    if (count < 2)
    {
        cerr<< "fatal error: Stats::printStats() => samples < 2 !\n";
        exit(1);
    }

    cout << setiosflags(ios::fixed|ios::showpoint);
    cout << setprecision(precision);

    if (verbose)
    {
        cout << "\n----------------------------------------\n";
        cout << "Stats: " << varname << "\n";
        cout << "Sample Count        : " << setw(width) << count << "\n";
        cout << "Sample Mean         : " << setw(width) << calcMean() << "\n";
        cout << "Sample Standard Dev : " << setw(width) << calcStDev() << "\n";
        cout << "Sample Min          : " << setw(width) << calcMin() << "\n";
        cout << "Sample Max          : " << setw(width) << calcMax();
        cout << "\n----------------------------------------\n";
    }
    else
    {
        cout << varname << " : ";
        cout << setw(width) << count << " ";
        cout << setw(width) << calcMean() << " ";
        cout << setw(width) << calcStDev() << " ";
        cout << setw(width) << calcMin() << " ";
        cout << setw(width) << calcMax() << " ";
    }
}


// print histogram
void Stats::printHistogram(char* varname, int width, int precision)
{
    if (count < 1)
    {
        cerr<< "fatal error: Stats::printHistogram() => samples < 1 !\n";
        exit(1);
    }

    if (!histo)
    {
        cerr << "warning: Stats::printHistogram(): no histogram to print!\n\n";
        return;
    }

    double y = lo;
    cout << setiosflags(ios::fixed|ios::showpoint);
    cout << setprecision(precision);
    cout << "\n----------------------------------------\n";
    cout << "HISTOGRAM: " << varname << "\n";
    cout << "(" << setw(width) << "-INF" << "," << setw(width) << lo << ") : ";
    cout << setw(width) << ((double) histogram[0])/count << "\n";

    for (int i=1; i<=nbin; y+=bin, i++ )
    {
        cout << "[" <<  setw(width) << y << "," << setw(width) << y+bin << ") : ";
        cout << setw(width) << ((double) histogram[i])/count << "\n";
    }

    cout << "[" << setw(width) << hi << "," << setw(width) << "+INF" << ") : ";
    cout << setw(width) << ((double) histogram[nbin+1])/count;
    cout << "\n----------------------------------------\n";
    return;
}



/*---------------------------------------------------------------
TStats Functions
---------------------------------------------------------------*/

// class constructor (default, without histogram)
TStats::TStats(void)
{
    histo = false;
    resetTStats();
}


// class constructor (with histogram)
TStats::TStats(double low, double high, int bins)
{
    if (!((low<high) && (bins>0))) // input check
    {
        cerr<< "fatal error: TStats::TStats() => bad parameters to construct TStats!\n";
        exit(1);
    }

    histo = true;
    lo = low;
    hi = high;
    nbin = bins;
    bin = (hi - lo) / nbin;
    histogram = new double[nbin+2];
    resetTStats();
}


// class destructor
TStats::~TStats(void)
{
    if (histo) delete [] histogram;;
}


// reset statistics
void TStats::resetTStats()
{
    tnow = 0.;
    sum = 0.;
    sumsq = 0.;
    min = DBL_MAX;
    max = -DBL_MAX;
    if (histo)
        for (int i=0; i < nbin+2; i++)
            histogram[i] = 0.;
}


// take one data sample x, along with the time of sample tx
void TStats::takeSample(double x, double tx)
{
    double tdiff = tx - tnow;
    if (tdiff<=0.) { cerr <<"fatal: TStats::takeSample(): negative time advance!\n"; exit(1); }
    tnow = tx;
    sum += (x * tdiff);
    sumsq += ( x * x * tdiff);

    if (x < min) min = x;
    if (x > max) max = x;

    if (histo)
    {
        if ( x < lo ) { histogram[0] += tdiff; return; }
        if ( x > hi ) { histogram[nbin+1] += tdiff; return; }

        int i = ( (int) ((x - lo) / bin )) + 1;
        histogram[i] += tdiff;
    }
    return;
}


// compute mean of samples
double TStats::calcMean(void)
{
    if (tnow <= 0.) { cerr<< "fatal error: TStats::calcMean() => no samples!\n"; exit(1); }
    return (sum/tnow);
}


// compute unbiased standard deviation of samples
double TStats::calcStDev(void)
{
    if (tnow <= 0.) { cerr<< "fatal error: TStats::calcStDev() => no samples!\n"; exit(1); }
    double ave = (sum / tnow);
    double var = ((sumsq/tnow) - (ave * ave));
    return sqrt(var);
}


// print time statistics
void TStats::printTStats(char* varname, int width, int precision, int verbose)
{
    if (tnow <= 0.) { cerr<< "fatal error: TStats::printStats() => no samples!\n"; exit(1); }

    cout << setiosflags(ios::fixed|ios::showpoint);
    cout << setprecision(precision);

    if (verbose)
    {
        cout << "\n----------------------------------------\n";
        cout << "TStats: " << varname << "\n";
        cout << "Elapsed Time   : " << setw(width) << tnow << "\n";
        cout << "Average        : " << setw(width) << calcMean() << "\n";
        cout << "Standard Dev   : " << setw(width) << calcStDev() << "\n";
        cout << "Min            : " << setw(width) << calcMin() << "\n";
        cout << "Max            : " << setw(width) << calcMax();
        cout << "\n----------------------------------------\n";
    }
    else
    {
        cout << varname << " : ";
        cout << setw(width) << tnow << " ";
        cout << setw(width) << calcMean() << " ";
        cout << setw(width) << calcStDev() << " ";
        cout << setw(width) << calcMin() << " ";
        cout << setw(width) << calcMax() << " ";
    }
}


// print time histogram
void TStats::printHistogram(char* varname, int width, int precision)
{
    if (tnow <= 0.) { cerr<< "fatal error: TStats::printHistogram() => no samples!\n"; exit(1); }

    if (!histo)
    {
        cerr << "warning: TStats::printHistogram(): no histogram to print!\n\n";
        return;
    }

    double y = lo;
    cout << setiosflags(ios::fixed|ios::showpoint);
    cout << setprecision(precision);
    cout << "\n----------------------------------------\n";
    cout << "Time HISTOGRAM: " << varname << "\n";
    cout << "(" << setw(width) << "-INF" << "," << setw(width) << lo << ") : ";
    cout << setw(width) << (histogram[0]/tnow) << "\n";

    for (int i=1; i<=nbin; y+=bin, i++ )
    {
        cout << "[" <<  setw(width) << y << "," << setw(width) << y+bin << ") : ";
        cout << setw(width) << (histogram[i]/tnow) << "\n";
    }

    cout << "[" << setw(width) << hi << "," << setw(width) << "+INF" << ") : ";
    cout << setw(width) << (histogram[nbin+1]/tnow);
    cout << "\n----------------------------------------\n";
    return;
}


/*---------------------------------------------------------------
Useful Non-class Functions
---------------------------------------------------------------*/

/*
The following function is used to compute the "confidence interval" for the mean of sample means.
Suppose you run a simulation n times and each time you compute an average quantity, X_i, 
for some quantity of interest in your simulated system. Then you compute the mean of X_i's, 
denote X_bar. In theory, X_bar is a good estimate for the mean of quantity of interest. 
But if you want to compute a "margin of error" for X_bar, the following function can used 
to return the margin of error. It needs to know n and the standard deviation of X_i's.

After you have computed the margin of error, you can compute a "confidence interval"
for X_bar:
    X_bar - margin_of error < mean < X_bar + margin_of error

For this function to return a reliable value, the number of samples n should be large (e.g. > 100).
*/
double calcErrorMargin(
    double stdev,            // standard deviation of X_i's
    int count,               // number of X_i's
    float confidence_level   // confidence_level (larger than 0.5, smaller than 1.0)
)
{
    const float zmax = 4.0;  // based on the ZTable array
    const int steps = 400;   // based on the ZTable array
    int i;

    if ( ! ((confidence_level>0.1) && (confidence_level<1.0)) )
    {
        cerr << "fatal error: calcErrorMargin() => unacceptable confidence level!\n";
        exit(1);
    }

    for (i = 1; i < steps; i++)
    {
        if ((ZTable[i] - 0.5) > (confidence_level/2.0)) break;
    }

    float z = i * (zmax/steps);
    return (z * stdev/sqrt(count));
}


} // namespace shk