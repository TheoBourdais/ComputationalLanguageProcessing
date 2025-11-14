The code implements QAP generator described in the paper: 
Palubeckis G. "An algorithm for construction of test cases for the 
quadratic assignment problem", Informatica, Vol. 11, No. 3 (2000), pp. 281-296. 
The input data are supplied through the (input) file in 
the following order:
 - n_objects  -  the size of QAP instance to be generated. 
 - x_dimension  -  the number of columns of the grid used by the generator. 
 - y_dimension  -  the number of rows of the grid used by the generator. 
   A necessary condition is that x_dimension * y_dimension >= n_objects. 
   Recommended: x_dimension = y_dimension and x_dimension * y_dimension equals 
   approximately 2*n_objects. For example, if n_objects = 70, then 
   x_dimension = y_dimension = 12. 
 - n_graphs  -  the number of graphs used by the generator. 
   Recommended: n_graphs = n_objects/2. 
 - size_upper_bound  -  the maximum possible order of graphs used by the 
   generator. A necessary condition is that size_upper_bound should be 
   an odd number from the interval [3, n_objects]. 
 - size_lower_bound  -  the minimum possible order of graphs used by the 
   generator. A necessary condition is that size_lower_bound should be 
   an odd number from the interval [3, size_upper_bound]. 
   Recommended: if n_objects is even, then size_upper_bound = size_lower_bound = n_objects - 1; 
   if n_objects is odd, then size_upper_bound = size_lower_bound = n_objects - 2. 
 - graph_weight_bound  -  upper bound on the weight of edges of graphs 
   used by the generator. 
   Recommended: graph_weight_bound = 10 for smaller values of n_objects, 
   say, n_objects<=150, and graph_weight_bound = 5 for n_objects>150. 
 - seed  -  seed for random number generator. 
 - n_instances  -  the number of QAP instances to be generated (in the current 
   configuration of the code, always n_instances = 1). 
 - file_name_out  -  the name of the output file containing QAP instance 
   generated along with the optimal value attached. 

Example of the data stored in the input file, named input_example.dat, is given below 
(it is assumed that disk D contains (empty) directory "instances"): 
70 12 12 35 69 69 10 15000 1 
D:\instances\qap70.txt 

Example of invocation of the generator is as follows: 
gen.exe input_example.dat 
