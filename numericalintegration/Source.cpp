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
	std::ifstream sourceFile("numerical_integration.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));

	// Make program of the source code in the context
	cl::Program program = cl::Program(context, source);

	// Build program for these specific devices
	program.build(devices);

	// Make kernel
	cl::Kernel kernel(program, "trapezoidal");

	cl::Buffer sum(context, CL_MEM_READ_WRITE, sizeof(int) * 16);

	int n = 10;

	cl::Kernel kernel_add=cl::Kernel(program,"trapezoidal");
	float k = 0.0, k1 =1.0;
    n = 6;
	kernel_add.setArg(0, k);
	kernel_add.setArg(1, k1);
	kernel_add.setArg(2,sum);
	kernel_add.setArg(3, n);
	queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(8), cl::NDRange(4));
	queue.finish();

	 float sums[1];
	//read result C from the device to array C
	queue.enqueueReadBuffer(sum, CL_TRUE, 0, sizeof(float), sums);

	std::cout << " result: \n";
//	for (int i = 0; i < 10; i++) {
		std::cout <<sums[0]<<" ";
//	}
	getchar();
	return 0;
}