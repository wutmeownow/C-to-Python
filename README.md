# C-to-Python

Various examples and short exercises on bridging Python and compiled code
  
Project 2a: This function uses numpy to estimate Pi using monte carlo stone throwing. It takes around 4 s for 100M random numbers, which is roughly 4 times as long as in C or roughly 1/4 the time in Python. I used the numpy.random package to generate 100M uniform random 2D vectors in one array, and then used the linalg.norm() method from numpy to calculate the magnitudes of all these vectors in the same array. To count the number inside the circle, I created a boolean array with the condition that the magnitudes are less than 1 and then took the sum of it.