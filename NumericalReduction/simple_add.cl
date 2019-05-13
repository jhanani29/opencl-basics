__kernel void Prefix_add(__global const int* A, __global int* C) {

	uint local_id = get_local_id(0);
	uint group_size = get_local_size(0);

	__local uint l_sum[5];

	l_sum[local_id] = A[get_global_id(0)];
	//printf("\n group_size:%d", group_size);
	barrier(CLK_LOCAL_MEM_FENCE);
	for (uint stride = group_size / 2; stride > 0; stride /= 2)
	{


		if (local_id < stride)
		{
			printf("\n stride:%d", stride);
			printf("\n local_value:%d", l_sum[local_id + stride]);
			l_sum[local_id] += l_sum[local_id + stride];

		}

	}


	if (local_id == 0)
		C[get_global_id(0)] = l_sum[0];
}