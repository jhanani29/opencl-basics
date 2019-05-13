#include <CL/cl.hpp>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>


int main() {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	// Select the default platform and create a context using this platform and the GPU
	cl_context_properties cps[3] = {
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)(platforms[0])(),
		0
	};
	cl::Context context(CL_DEVICE_TYPE_GPU, cps);

	// Get a list of devices on this platform
	std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

	// Create a command queue and use the first device
	cl::CommandQueue queue = cl::CommandQueue(context, devices[0]);

	// Read source file
	std::ifstream sourceFile("simple_add.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));

	// Make program of the source code in the context
	cl::Program program = cl::Program(context, source);

	// Build program for these specific devices
	program.build(devices);

	// Make kernel
	cl::Kernel kernel(program, "Prefix_add");

	// create buffers on the device
	cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 16);
	cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
	cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 16);

	int n = 10;
	int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6 };

	//create queue to which we will push commands for the device.


	//write arrays A and B to the device
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * 16, A);
	queue.enqueueWriteBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * 10, NULL);

	//alternative way to run the kernel
	cl::Kernel kernel_add=cl::Kernel(program,"Prefix_add");
	kernel_add.setArg(0,buffer_A);
	kernel_add.setArg(1,buffer_C);
	queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(16), cl::NDRange(8));
	queue.finish();

	int C[10];
	//read result C from the device to array C
	queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * 16, C);

	std::cout << " result: \n";
//	for (int i = 0; i < 16; i++) {
		std::cout << C[0]+C[8] << " ";
//	}
	getchar();
	return 0;
}