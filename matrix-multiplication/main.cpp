#include <cstdio>
#include <cstring>
#include <CL/cl.h>
#include <iostream>
#include <fstream>


int main(int argc, char* argv[]) {

	// Объявление переменных
	cl_uint platfrom_counter = 0;
	cl_int res = 0;
	cl_platform_id* platfrom_ids = NULL;

	// Получаем текущие платформы

	res = clGetPlatformIDs(0, NULL, &platfrom_counter);

	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n", res);
		return res;
	}

	if (platfrom_counter == 0) {
		printf("Error (mess) - not such OpenCL platfroms\n");
		return -1;
	}

	platfrom_ids = \
		(cl_platform_id*)malloc(platfrom_counter * sizeof(cl_platform_id));

	if (!platfrom_ids) {
		printf("Error (mess) - not alloc memory for OpenCL platfrom ids\n");
		return -1;
	}

	res = clGetPlatformIDs(platfrom_counter, platfrom_ids, NULL);

	if (res != CL_SUCCESS) {
		printf("Error (code) - %d\n", res);
		return res;
	}

	if (platfrom_ids == NULL) {
		printf("Error (mess) - platfrom_ids was ignore\n");
		return -1;
	}

	// Получаем графической устройство



	return 0;
}