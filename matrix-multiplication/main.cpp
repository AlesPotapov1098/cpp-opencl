#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

int main(int argc, char* argv[]) {

	// Объявление переменных
	cl_uint platfrom_counter = 0;
	cl_uint device_counter = 0;
	cl_int res = 0;
	cl_platform_id* platfrom_ids = NULL;
	cl_device_id device_id = 0;
	cl_context context_gpu_device = 0;
	cl_program program_for_gpu = 0;
	cl_kernel kernel = 0;
	cl_command_queue command_queue = 0;
	cl_mem buffer_a;
	cl_mem buffer_b;
	cl_mem buffer_c;
	std::ifstream kernel_file;
	std::string source_code;

	// Получаем текущие платформы

	res = clGetPlatformIDs(0, nullptr, &platfrom_counter);

	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n", res);
		return res;
	}

	if (platfrom_counter == 0) {
		printf("Error (mess) - not such OpenCL platfroms\n");
		return -1;
	}

	platfrom_ids = new cl_platform_id[platfrom_counter];

	if (!platfrom_ids) {
		printf("Error (mess) - not alloc memory for OpenCL platfrom ids\n");
		return -1;
	}

	res = clGetPlatformIDs(platfrom_counter, platfrom_ids, nullptr);

	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n", res);
		return res;
	}

	if (platfrom_ids == NULL) {
		printf("Error (mess) - platfrom_ids was ignore\n");
		return -1;
	}

	// Выводи информацию о платформах
	
	for (int i = 0; i < platfrom_counter; i++) {
	
		char* platform_name = nullptr;
		char* platform_vendor = nullptr;
		std::size_t platform_name_size = 0;
		std::size_t platform_vendor_size = 0;
	
		printf("--------------------------------------------------------------------------\n");

		// Получаем имя платформы
			res = clGetPlatformInfo(platfrom_ids[i],CL_PLATFORM_NAME,0,nullptr,&platform_name_size);
			if (res != CL_SUCCESS || platform_name_size == 0) {
				printf("Error (code) - %d\n",res);
				return res;
			}
			
			platform_name = new char[platform_name_size];
			res = clGetPlatformInfo(platfrom_ids[i],CL_PLATFORM_NAME,platform_name_size,(void*)platform_name,nullptr);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n",res);
				return res;
			}

			if (platform_name == nullptr) {
				printf("Error (mess) - platfrom_name was ignore\n");
				return -1;
			}
		// Конец
		
		printf("Platform%d Name - %s\n",i,platform_name);

		// Получаем производителя платформы
			res = clGetPlatformInfo(platfrom_ids[i],CL_PLATFORM_VENDOR,0,nullptr,&platform_vendor_size);
			if (res != CL_SUCCESS || platform_vendor_size == 0) {
				printf("Error (code) - %d\n",res);
				return res;
			}
	
			platform_vendor = new char[platform_vendor_size];
			res = clGetPlatformInfo(platfrom_ids[i],CL_PLATFORM_VENDOR,platform_vendor_size,(void*)platform_vendor,nullptr);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n",res);
				return res;
			}
	
			if (platform_vendor == nullptr) {
				printf("Error (mess) - platfroms_vendor was ignore\n");
				return -1;	
			}
		// Конец

		printf("Platform%d Vendor - %s\n",i,platform_vendor);
	
		printf("--------------------------------------------------------------------------\n");

		delete [] platform_name;
		delete [] platform_vendor;
		platform_name_size = 0;
		platform_vendor_size = 0;
	}

	// Получаем графической устройство

	res = clGetDeviceIDs(platfrom_ids[1],CL_DEVICE_TYPE_GPU,0,nullptr,&device_counter);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n",res);
		return res;
	}

	if (device_counter == 0) {
		printf("Error (mess) - not such GPU device\n");
		return -1;	
	}

	res = clGetDeviceIDs(platfrom_ids[1],CL_DEVICE_TYPE_GPU,1,&device_id,nullptr);
	if(res != CL_SUCCESS){
		printf("Error (code) - %d\n",res);		
		return res;
	}
	
	if (device_id == 0) {
		printf("Error (mess) - device_id is 0\n");
		return -1;
	}	

	// Выводи информацию об графическом устройстве
	{
		printf("--------------------------------------------------------------------------\n");
		// Выводи имя графического устройства
		char * device_gpu_name = nullptr;
		std::size_t device_gpu_name_size = 0;
		
			// Проверка на наличие информации об устройстве	
			res = clGetDeviceInfo(device_id,CL_DEVICE_NAME,0,nullptr,&device_gpu_name_size);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n",res);
				return res;
			}

			if (device_gpu_name_size == 0) {
				printf("Error (mess) - device_gpu_name_size was ignore\n");
				return -1;
			}
			// Конец

		device_gpu_name = new char[device_gpu_name_size];
	
			//	Получаем имя устройства
			res = clGetDeviceInfo(device_id,CL_DEVICE_NAME,device_gpu_name_size,(void*)device_gpu_name,nullptr);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n", res);
				delete [] device_gpu_name;
				return res;
			}

			if (!device_gpu_name) {
				printf("Error (mess) - device_gpu_name was ignore\n");
				delete [] device_gpu_name;
				return -1;
			}
			// Конец

		printf("Device Name - %s\n",device_gpu_name);
		delete [] device_gpu_name;
		// Конец
		
		// Получаем производителя графического устройства
		char * device_gpu_vendor = nullptr;
		std::size_t device_gpu_vendor_size = 0;

			// Проверка на наличие информации об производителе устройства
			res = clGetDeviceInfo(device_id,CL_DEVICE_VENDOR,0,nullptr,&device_gpu_vendor_size);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n",res);
				return res;
			}

			if (device_gpu_vendor_size == 0) {
				printf("Error (mess) - device_gpu_vendor_size was ignore\n");
				return -1;
			}
			// Конец

		device_gpu_vendor = new char[device_gpu_vendor_size];

			// Получаем производителя устройства
			res = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, device_gpu_vendor_size, (void*)device_gpu_vendor, nullptr);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n", res);
				delete [] device_gpu_vendor;
				return res;
			}

			if (!device_gpu_vendor) {
				printf("Error (mess) - device_gpu_vendor was ignore\n");
				delete [] device_gpu_vendor;
				return -1;
			}
			// Конец
	
		printf("Device Vendor - %s\n",device_gpu_vendor);
		delete [] device_gpu_vendor;
		// Конец

		printf("--------------------------------------------------------------------------\n");
	}
	
	// Создаем контекст устройства

	// TODO: поэкспериментировать с pfn_notify()

	context_gpu_device = clCreateContext(nullptr,device_counter,&device_id,nullptr,nullptr,&res);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n",res);
		return res;
	}

	if (context_gpu_device == 0) {
		printf("Error (mess) - not created context for GPU\n");
		return -1;
	}

	kernel_file = std::ifstream("multymatrix.cl");
	if (!kernel_file.is_open()) {
		printf("Error (mess) - can not open file\n");
		return -1;
	}
	source_code = std::string(std::istreambuf_iterator<char>(kernel_file),(std::istreambuf_iterator<char>()));
	const char * src = source_code.c_str();
	std::size_t size = source_code.length() + 1;

	program_for_gpu = clCreateProgramWithSource(context_gpu_device,1,&src,&size,&res);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n");
		return res;
	}

	if (program_for_gpu == 0) {
		printf("Error (mess) - not created progrma\n");
		return -1;
	}	

	// TODO: разобраться с device_id
	res = clBuildProgram(program_for_gpu,0,nullptr,nullptr,nullptr,nullptr);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n",res);

		std::size_t size_log = 0;
		char * build_log;
		res = clGetProgramBuildInfo(program_for_gpu,device_id,CL_PROGRAM_BUILD_LOG,0,nullptr,&size_log);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n",res);
			return res;
		}

		if (size_log == 0) {
			printf("Error (mess) - not such size_log\n");
			return -1;
		}

		build_log = new char[size_log];
		res = clGetProgramBuildInfo(program_for_gpu,device_id,CL_PROGRAM_BUILD_LOG,size_log,build_log,nullptr);
		if (res != CL_SUCCESS) {
			printf("Error (code) - %d\n",res);
			return res;
		}
		printf("%s\n",build_log);
		delete [] build_log;
		return res;
	}

	// Создаем kernel
	kernel = clCreateKernel(program_for_gpu,"simple_multy_matrix",&res);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n");
		return res;
	}

	command_queue = clCreateCommandQueue(context_gpu_device,device_id,CL_QUEUE_PROFILING_ENABLE,&res);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n");
		return res;
	}

	cl_int w1 = 4;
	cl_int h1 = 2;
	cl_int w2 = 6;
	cl_int h2 = 4;

	cl_int input1[2*4] = {1,1,1,1,
						  1,1,1,1};
	cl_int input2[6*4] = {2,2,2,2,
						  2,2,2,2,
						  2,2,2,2,
						  2,2,2,2,
						  2,2,2,2,
						  2,2,2,2 };
	cl_int output[6*2] = { 0 };

	buffer_a = clCreateBuffer(context_gpu_device,CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,sizeof(cl_int)*w1*h1,input1,&res);
	buffer_b = clCreateBuffer(context_gpu_device,CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,sizeof(cl_int)*w2*h2,input2,&res);
	buffer_c = clCreateBuffer(context_gpu_device,CL_MEM_WRITE_ONLY,sizeof(int)*w2*h1,0,&res);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n");
		return res;
	}
	
	res  = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&buffer_c);
	res |= clSetKernelArg(kernel,1,sizeof(cl_int),(void*)&w1);
	res |= clSetKernelArg(kernel,2,sizeof(cl_int),(void*)&h1);
	res |= clSetKernelArg(kernel,3,sizeof(cl_int),(void*)&w2);
	res |= clSetKernelArg(kernel,4,sizeof(cl_int),(void*)&h2);
	res |= clSetKernelArg(kernel,5,sizeof(cl_mem),(void*)&buffer_a);
	res |= clSetKernelArg(kernel,6,sizeof(cl_mem),(void*)&buffer_b);
	
	const std::size_t global_size[2] = {6,4};
	const std::size_t local_size[2] = {2,2};
	res = clEnqueueNDRangeKernel(command_queue,kernel,2,0,global_size,local_size,0,nullptr,nullptr);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n",res);
		return res;
	}

	res = clEnqueueReadBuffer(command_queue,buffer_c,CL_TRUE,0,sizeof(cl_int)*w2*h1,output,0,nullptr,nullptr);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n",res);
		return res;
	}

	for (int i = 0; i < w2*h1; i++) {
		printf("c[%d] = %d\n",i,output[i]);
	}

	// Освобождаем память
	{
		clReleaseMemObject(buffer_c);
		clReleaseMemObject(buffer_b);
		clReleaseMemObject(buffer_a);
		clReleaseCommandQueue(command_queue);
		clReleaseKernel(kernel);
		clReleaseProgram(program_for_gpu);
		clReleaseContext(context_gpu_device);
		delete [] platfrom_ids;
	}
	
	return 0;
}