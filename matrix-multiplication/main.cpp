#include <cstdio>
#include <cstring>
#include <CL/cl.h>
#include <iostream>
#include <fstream>


int main(int argc, char* argv[]) {

	// Объявление переменных
	cl_uint platfrom_counter = 0;
	cl_uint device_counter = 0;
	cl_int res = 0;
	cl_platform_id* platfrom_ids = NULL;
	cl_device_id * device_ids = NULL;

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

		printf("Platform%d Venfor - %s\n",i,platform_vendor);
	
		printf("--------------------------------------------------------------------------\n");

		delete [] platform_name;
		delete [] platform_vendor;
		platform_name_size = 0;
		platform_vendor_size = 0;
	}

	// Получаем графической устройство

	delete [] platfrom_ids;
	delete [] device_ids;

	return 0;
}