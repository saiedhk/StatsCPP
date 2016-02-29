# StatsCPP
C++ Classes for Simple Univariate Statistics

This package provides two C++ classes and utility functions for 
statistical computations. They come handy when doing discrete-event simulations.

Stats:

Stats is a C++ class for collecting statistics on a random variable.
Suppose X is the random variable of interest. You can define a 'Stats' object
that allows you to take a sequence of samples, and then calculate simple statistics
such as mean, unbiased standard deviation and histogram (a relative frequency table)
on your data. 

TStats:

TStats is a C++ class for computing simple statistics on a random process.
Suppose X(t) is the random process of interest. You can define a 'TStats' object
that allows you to take a sequence of samples x(t_1), x(t_2), ..., x(t_i),
and then calculate simple statistics such as mean, standard deviation of the X(t).
You can also compute the histogram of the process. In this case, a histogram paritions
the X-axis into many bins (intervals) and computes the percentage of the time X spends
in each bin. You can then print the histogram.
This class comes usually handy when you do discrete-event simulations.

