
__kernel void linear_search(__global float *input, __global float *output, int n)
{

	int i = get_global_id(0);
	if (input[i] == n)
	{
		output[0] = 1;
	}

}