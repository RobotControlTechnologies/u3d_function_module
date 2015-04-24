/*
* File: messages.cpp
* Author: m79lol, iskinmike
*
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 
#define _SCL_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <vector>

#include "SimpleIni.h"
#include "../module_headers/module.h"
#include "../module_headers/function_module.h"
#include "u3d_function_module.h"
#include "messagesf.h"

// GLOBAL VARIABLES
int COUNT_U3D_FUNCTIONS=3;

u3dFunctionModule::u3dFunctionModule() {
	u3d_functions = new FunctionData*[COUNT_U3D_FUNCTIONS];
	system_value function_id = 0;

	FunctionData::ParamTypes *Params = new FunctionData::ParamTypes[7];
	Params[0] = FunctionData::FLOAT;
	Params[1] = FunctionData::FLOAT;
	Params[2] = FunctionData::FLOAT;
	Params[3] = FunctionData::FLOAT;
	Params[4] = FunctionData::FLOAT;
	Params[5] = FunctionData::FLOAT;
	Params[6] = FunctionData::STRING;

	u3d_functions[function_id] = new FunctionData(function_id + 1, 7, Params, "spawnCube");
	function_id++;

	Params = new FunctionData::ParamTypes[5];
	Params[0] = FunctionData::FLOAT;
	Params[1] = FunctionData::FLOAT;
	Params[2] = FunctionData::FLOAT;
	Params[3] = FunctionData::FLOAT;
	Params[4] = FunctionData::STRING;

	u3d_functions[function_id] = new FunctionData(function_id + 1, 5, Params, "spawnSphere");
	function_id++;

	Params = new FunctionData::ParamTypes[1];
	Params[0] = FunctionData::FLOAT;
	u3d_functions[function_id] = new FunctionData(function_id + 1, 1, Params, "deleteCreatedObject");
};

FunctionResult* u3dFunctionModule::executeFunction(system_value function_index, void **args) {
	if ((function_index < 1) || (function_index > COUNT_U3D_FUNCTIONS)) {
		return NULL;
	}
	variable_value rez = 0;
	try {
		switch (function_index) {
		case 1: {
			variable_value *input1 = (variable_value *)args[0];
			variable_value *input2 = (variable_value *)args[1];
			variable_value *input3 = (variable_value *)args[2];
			variable_value *input4 = (variable_value *)args[3];
			variable_value *input5 = (variable_value *)args[4];
			variable_value *input6 = (variable_value *)args[5];
			std::string input7((const char *)args[6]);
			rez = createCube((int)*input1, (int)*input2, (int)*input3, (int)*input4, (int)*input5, (int) *input6, input7);
			break;
		}
		case 2: {
			variable_value *input1 = (variable_value *)args[0];
			variable_value *input2 = (variable_value *)args[1];
			variable_value *input3 = (variable_value *)args[2];
			variable_value *input4 = (variable_value *)args[3];
			std::string input5((const char *)(args[4]));
			rez = createSphere((int)*input1, (int)*input2, (int)*input3, (int)*input4, input5);
			break;
		}
		case 3: {
			variable_value *input1 = (variable_value *)args[0];
			deleteCreatedObject( (int) *input1);
			break;
		}
		};
		return new FunctionResult(1, rez);
	}
	catch (...){
		return new FunctionResult(0);
	};
};

void u3dFunctionModule::prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p) {
	colorPrintf = colorPrintf_p;
}
FunctionData** u3dFunctionModule::getFunctions(unsigned int *count_functions) {
	*count_functions = COUNT_U3D_FUNCTIONS;
	return u3d_functions;
};
const char* u3dFunctionModule::getUID() {
	return "u3dfunction_dll";
};
void *u3dFunctionModule::writePC(unsigned int *buffer_length) {
	*buffer_length = 0;
	return NULL;
}
int u3dFunctionModule::startProgram(int uniq_index, void *buffer, unsigned int buffer_length) {
	return 0;
}
int u3dFunctionModule::endProgram(int uniq_index) {
	return 0;
}
void u3dFunctionModule::destroy() {
	for (unsigned int j = 0; j < COUNT_U3D_FUNCTIONS; ++j) {
		delete u3d_functions[j];
	}
	delete[] u3d_functions;
	delete this;
};

__declspec(dllexport) FunctionModule* getFunctionModuleObject() {
	return new u3dFunctionModule();
};
