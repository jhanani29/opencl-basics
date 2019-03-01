#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<CL/cl.h>

#define MAT_SIZE 15
using namespace std;

void err_check(int err, string err_code)
{
	if (err != CL_SUCCESS)
	{
		cout << "Error:" << err_code << "(" << err << ")" << endl;
	}
}
int main()
{
	cl_platform_id platform_id = NULL;
	cl_device_id device_id;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem A_value = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platform;
	cl_int err;
	cl_ulong local_size;
	cl_int cl_local_size;
	cl_event ev;
	size_t local_size2;
	int data = 10;

	err = clGetPlatformIDs(1, &platform_id, &ret_num_platform);;
	err_check(err, "clGetPlatformIDs");

	err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 1, &device_id, &ret_num_devices);
	err_check(err, "clGetDeviceIDs");

	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
	err_check(err, "clCreateContext");

	command_queue = clCreateCommandQueue(context, device_id, 0, &err);
	err_check(err, "clCreateContext");

	A_value = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &err);
	err = clEnqueueWriteBuffer(command_queue, A_value, CL_TRUE, 0, sizeof(int), &data, 0, NULL, NULL);
	cout << "Buffer is done";
	err_check(err, "clCreateContext");

	ifstream file("xclbin/krnl_async.sw_emu.xilinx_aws-vu9p-f1-04261818_dynamic_5_0.xclbin");
	string prog(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));
	const char *source_str = prog.c_str();
	size_t size = prog.size();
	cout << "kernel file read" << endl;

	program = clCreateProgramWithBinary(context, 1, &device_id, &size, (const unsigned char **)&source_str, 0, &err);
	err_check(err, "clCreateContext");

	cout << "Build_prog" << endl;

	err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	cout << "Program Build" << endl;
	err_check(err, "clBuildProgram");

	clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_size), &local_size, NULL);
	cout << "  CL_DEVICE_LOCAL_MEM_SIZE =" << (unsigned long long) local_size << endl;
	err_check(err, "clGetDeviceInfo");

	cl_local_size = local_size / 2;
	kernel = clCreateKernel(program, "async_work1", &err);
	err_check(err, "clCreateKernel");
	cout << "opencl Kernel Created";

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&A_value);
	err = clSetKernelArg(kernel, 1, sizeof(int), NULL);
	err = clSetKernelArg(kernel, 2, sizeof(cl_local_size), &cl_local_size);
	cout << "Argument is set";

	size_t global_item_size = 1;
	size_t local_item_size = 1;
	clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	cout << "opencl kernel executed";


	clEnqueueReadBuffer(command_queue, A_value, CL_TRUE, 0, sizeof(int), &data, 0, NULL, NULL);
	cout << "Data" << data << endl;
	cout << "opencl mem fetch" << endl;


	clFlush(command_queue);
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(A_value);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
	cout << "Freeeeeeeeeeeeeee";
	return 0;
}