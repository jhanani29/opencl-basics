/*nclude<iostream>

float y(float x)
{
	return 1 / (1 + x * x);
}
float trapezoidal(float a, float b, float n)
{
	float h = (b - a) / n;
	float s = y(a) + y(b);
	for (int i = 1; i < n; i++)
	{
		s += 2 * y(a + i * h);
	}
	return (h / 2)*s;
}
int main()
{
	float x0 = 0;
	float xn = 1;
	int n = 6;
	std::cout << trapezoidal(x0, xn, n);
	return 0;
}
*/
float y(float x)
{
	return 1 / (1 + x * x);
}
float inner_func(float a1, float b1)
{
	float fA, fB;
	fA = y(a1);
	fB = y(b1);
	return ((fA + fB)*(b1 - a1));
}
__kernel void trapezoidal(float a,
	float b, __global float* sum, int n)
{
	__local float interim_sum[64];
	int l_id = get_local_id(0);
	float a1 = a + ((b - a) / get_global_size(0))*get_global_id(0);
	float b1 = a1 + ((b - a) / get_global_size(0));
	printf("a1:%f", a1);
	printf("b1:%f", b1);

	float h;
	if (n > 0) {
		h = (b1 - a1) / n;
	}
	else {
		h = b1 - a1;
	}
	int i;
	interim_sum[l_id] = 0;
	for (i = 0; i < n; i++)
	{
		interim_sum[l_id] += 2 * inner_func(a1 + h * i, a1 + h * (i + 1));
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if (l_id == 0)
	{
		for (i = 1; i < get_local_size(0); i++)
		{
			interim_sum[0] += interim_sum[i];
		}
		sum[get_group_id(0)] = interim_sum[0];
	}
}
