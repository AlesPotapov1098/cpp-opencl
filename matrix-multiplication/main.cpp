#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

int main(int argc, char* argv[]) {

	// ���������� ����������
	cl_uint platfrom_counter = 0;
	cl_uint device_counter = 0;
	cl_int res = 0;
	cl_platform_id* platfrom_ids = NULL;
	cl_device_id device_id = 0;
	cl_context context_gpu_device = 0;
	cl_program program_for_gpu = 0;
	cl_kernel kernel = 0;
	cl_command_queue command_queue = 0;
	std::ifstream kernel_file;
	std::string source_code;

	// �������� ������� ���������

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

	// ������ ���������� � ����������
	
	for (int i = 0; i < platfrom_counter; i++) {
	
		char* platform_name = nullptr;
		char* platform_vendor = nullptr;
		std::size_t platform_name_size = 0;
		std::size_t platform_vendor_size = 0;
	
		printf("--------------------------------------------------------------------------\n");

		// �������� ��� ���������
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
		// �����
		
		printf("Platform%d Name - %s\n",i,platform_name);

		// �������� ������������� ���������
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
		// �����

		printf("Platform%d Vendor - %s\n",i,platform_vendor);
	
		printf("--------------------------------------------------------------------------\n");

		delete [] platform_name;
		delete [] platform_vendor;
		platform_name_size = 0;
		platform_vendor_size = 0;
	}

	// �������� ����������� ����������

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

	// ������ ���������� �� ����������� ����������
	{
		printf("--------------------------------------------------------------------------\n");
		// ������ ��� ������������ ����������
		char * device_gpu_name = nullptr;
		std::size_t device_gpu_name_size = 0;
		
			// �������� �� ������� ���������� �� ����������	
			res = clGetDeviceInfo(device_id,CL_DEVICE_NAME,0,nullptr,&device_gpu_name_size);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n",res);
				return res;
			}

			if (device_gpu_name_size == 0) {
				printf("Error (mess) - device_gpu_name_size was ignore\n");
				return -1;
			}
			// �����

		device_gpu_name = new char[device_gpu_name_size];
	
			//	�������� ��� ����������
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
			// �����

		printf("Device Name - %s\n",device_gpu_name);
		delete [] device_gpu_name;
		// �����
		
		// �������� ������������� ������������ ����������
		char * device_gpu_vendor = nullptr;
		std::size_t device_gpu_vendor_size = 0;

			// �������� �� ������� ���������� �� ������������� ����������
			res = clGetDeviceInfo(device_id,CL_DEVICE_VENDOR,0,nullptr,&device_gpu_vendor_size);
			if (res != CL_SUCCESS) {
				printf("Error (code) - %d\n",res);
				return res;
			}

			if (device_gpu_vendor_size == 0) {
				printf("Error (mess) - device_gpu_vendor_size was ignore\n");
				return -1;
			}
			// �����

		device_gpu_vendor = new char[device_gpu_vendor_size];

			// �������� ������������� ����������
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
			// �����
	
		printf("Device Vendor - %s\n",device_gpu_vendor);
		delete [] device_gpu_vendor;
		// �����

		printf("--------------------------------------------------------------------------\n");
	}
	
	// ������� �������� ����������

	// TODO: �������������������� � pfn_notify()

	context_gpu_device = clCreateContext(nullptr,device_counter,&device_id,nullptr,nullptr,&res);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n",res);
		return res;
	}

	if (context_gpu_device == 0) {
		printf("Error (mess) - not created context for GPU\n");
		return -1;
	}

	kernel_file = std::ifstream("add.cl");
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

	// TODO: ����������� � device_id
	res = clBuildProgram(program_for_gpu,0,nullptr,nullptr,nullptr,nullptr);
	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n",res);

		const std::size_t size_log = 2048;
		char * build_log = new char[size_log];
		res = clGetProgramBuildInfo(program_for_gpu,device_id,CL_PROGRAM_BUILD_STATUS,sizeof(build_log)*size_log,build_log,nullptr);
		printf("%s\n",build_log);

		return res;
	}

	// ����������� ������
	// TODO: ��������  - comand queue, kernel, program, buffers etc. 
	{
		clReleaseProgram(program_for_gpu);
		clReleaseContext(context_gpu_device);
		delete [] platfrom_ids;
	}
	
	return 0;
}