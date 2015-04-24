/*
* File: messages.cpp
* Author: m79lol, iskinmike
*
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#define _SCL_SECURE_NO_WARNINGS

#include <string>
#include "messagesf.h"
#include <WinSock2.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <map>
#include <vector>
#pragma comment(lib, "ws2_32") //link to dll

#include <boost\interprocess\managed_shared_memory.hpp>
#include <boost\interprocess\shared_memory_object.hpp>


SOCKET SaR;

CRITICAL_SECTION G_CS_MES;
CRITICAL_SECTION *G_CS_FromMemory;

bool Is_ReadFromSharedMemory = false;

typedef std::pair<std::vector<std::pair<HANDLE*, std::string> *> *, CRITICAL_SECTION *> PairForSharedMemory;
PairForSharedMemory G_DataFromSharedMemory;
PairForSharedMemory *G_Data;

HANDLE hPostman;
bool Postman_Thread_Exist = true; // Global Variable to Colose Postman Thread

HANDLE hSharedPostmansMem = NULL;
LPVOID pTempBuf; // Проверим просто с указателем
PVOID pBuf;

struct VectorAndCS{
	std::vector<std::pair<HANDLE*, std::string> *>* Vector;
	CRITICAL_SECTION *CS;
};

VectorAndCS *VACSFromSharedMemory;

std::vector<std::pair<HANDLE*, std::string> *> *BoxOfMessages;

int extractString(std::string str, char first, char second){

	std::string temp("");

	int beg = 0;
	int end = 0;
	int i = 1;
	if (first == '%') { i++; }
	beg = str.find(first) + i;
	end = str.find(second);

	temp.assign(str, beg, end - beg);

	return std::stoi(temp);
};
int extractObj_id(std::string str){
	return extractString(str, ':', '&');
};

std::string extractMessage(std::string str){
	std::string temp("");

	char first = '+';
	char second = '&';

	unsigned int beg = 0;
	unsigned int end = 0;

	beg = str.find(first)+1;
	end = str.find(second)+1; // чтобы сохранить амперсанд и не надо было снова функции переписывать

	temp.assign(str, beg, end - beg);
	return temp;
};

int extractUniq_Id(std::string str){
	return extractString(str,'%','+');
};


void testStringSuccess(std::string str){
	if (str.find("fail") != std::string::npos) {
		throw std::exception();
	}
};


void initFunctionsDll(){

	boost::interprocess::shared_memory_object shm_obj(boost::interprocess::open_only, "PostmansSharedMemory", boost::interprocess::read_write);
	boost::interprocess::mapped_region region(shm_obj, boost::interprocess::read_only);

	VectorAndCS **VACSFSM = static_cast<VectorAndCS**>(region.get_address());
	VACSFromSharedMemory = *VACSFSM;

	VectorAndCS tempShMetm = **VACSFSM;

	G_CS_FromMemory = VACSFromSharedMemory->CS;
	BoxOfMessages = VACSFromSharedMemory->Vector;
};

std::string createMessage(std::string params){
	HANDLE WaitRecivedMessage;
	WaitRecivedMessage = CreateEvent(NULL, true, false, NULL);

	std::pair<HANDLE*, std::string> pairParams(&WaitRecivedMessage, params);
	if (!Is_ReadFromSharedMemory){
		initFunctionsDll();
		Is_ReadFromSharedMemory = true;
	}
	EnterCriticalSection(G_CS_FromMemory); //CRITICAL_SECTION
	(*BoxOfMessages).push_back(&pairParams); //push_back
	LeaveCriticalSection(G_CS_FromMemory);
	if (params != "destroy") {
		WaitForSingleObject(WaitRecivedMessage, INFINITE);
	}
	CloseHandle(WaitRecivedMessage);

	testStringSuccess(pairParams.second);
	return pairParams.second;
};


// for DELETECREATEDOBJECT
void deleteCreatedObject(int obj_id){
	std::string params("delete:");

	params.append(std::to_string((int)obj_id));

	createMessage(params);
};


// for CREATECUBE
int createCube(int x, int y, int z, int d_x, int d_y, int d_z, std::string color){
	std::string params("obj:cube,");

	params.append(std::to_string(x));
	params.append(",");
	params.append(std::to_string(y));
	params.append(",");
	params.append(std::to_string(z));
	params.append(",");
	params.append(std::to_string(d_x));
	params.append(",");
	params.append(std::to_string(d_y));
	params.append(",");
	params.append(std::to_string(d_z));
	params.append(",");
	params.append(color);

	std::string temp;
	temp = createMessage(params);

	int d = extractObj_id(temp);
	return d;
};

// for CREATESPHERE
int createSphere(int x, int y, int z, int Radius, std::string color){
	std::string params("obj:sphere,");

	params.append(std::to_string(x));
	params.append(",");
	params.append(std::to_string(y));
	params.append(",");
	params.append(std::to_string(z));
	params.append(",");
	params.append(std::to_string(Radius));
	params.append(",");
	params.append(color);

	std::string temp;
	temp = createMessage(params);

	int d = extractObj_id(temp);
	return d;
};
