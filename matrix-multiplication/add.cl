__kernel void add(
	__global int * c, 
	__global int * a, 
	__global int * b){

	int i = get_global_id(0);
	c[i] = a[i] + b[i];
}