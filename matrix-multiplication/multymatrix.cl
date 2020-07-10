__kernel void simple_multy_matrix(
	__global int * output,
	int width1,
	int height1,
	int width2,
	int height2,
	__global int * input1,
	__global int * input2)
{
	int row = get_global_id(1);
	int col = get_global_id(0);
	int sum = 0;
	for(int i = 0; i < width1; i++){
		sum += input1[row*width1+i] * input2[i*width2+col];
	}
	output[row*width2+col] = sum;
}