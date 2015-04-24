/*
* File: u3d_function_module.h
* Author: m79lol, iskinmike
*
*/

#ifndef U3D_FUNCTION_MODULE_H
#define	U3D_FUNCTION_MODULE_H

class u3dFunctionModule : public FunctionModule {
	FunctionData **u3d_functions;

public:

	colorPrintf_t *colorPrintf;
	u3dFunctionModule();

	//init
	const char *getUID();
	void prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p);

	//compiler only
	FunctionData **getFunctions(unsigned int *count_functions);
	void *writePC(unsigned int *buffer_length);

	//intepreter - program
	FunctionResult* executeFunction(system_value function_index, void **args);
	int startProgram(int uniq_index, void *buffer, unsigned int buffer_length);
	int endProgram(int uniq_index);

	//destructor
	void destroy();
	~u3dFunctionModule() {};
};

#endif	/* U3D_FUNCTION_MODULE_H  */
