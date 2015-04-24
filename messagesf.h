/*
* File: messages.h
* Author: m79lol, iskinmike
*
*/


int extractString(std::string str, char first, char second);


int extractObj_id(std::string str);


std::string extractMessage(std::string str);
int extractUniq_Id(std::string str);

void testStringSuccess(std::string str);
std::string createMessage(std::string params);

int createCube(int x, int y, int z, int d_x, int d_y, int d_z, std::string color);
int createSphere(int x, int y, int z, int Radius, std::string color);

void deleteCreatedObject(int obj_id);

void initFunctionsDll();

