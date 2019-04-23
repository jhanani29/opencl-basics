
pipe int p0 __attribute__((xcl_reqd_pipe_depth(32)));

__kernel void input_stage(__global int *input, int size,int inc, __global int *output)
{     
		int x;
        for(x=0; x < size;x++)
        {
				output[x] = input[x]+inc;
		        write_pipe_block(p0, &output[x]);
        }
}


__kernel void output_stage(int size, __global int *op)
{

         
	     int input_data,i;
		 for(i=0;i<size;i++)
		 {
		 	read_pipe_block(p0, &input_data);
		 	op[i] = input_data + input_data;
         }
         

}

