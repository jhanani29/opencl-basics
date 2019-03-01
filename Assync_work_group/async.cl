__kernel void async_work1(__global int *a, __local int *b, int local_size)
{
	async_work_group_copy(b, a, 1, 0);

	*b = (*b) * 100;

	async_work_group_copy(a, b, 1, 0);
}