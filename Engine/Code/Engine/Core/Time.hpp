#pragma once
//#include <windows.h>
#include "Engine/Core/EngineCommon.hpp"
//-----------------------------------------------------------------------------------------------
double GetCurrentTimeSeconds();
//
//double PCFreq = 0.0;
//__int64 CounterStart = 0;
//
//void StartCounter()
//{
//	LARGE_INTEGER li;
//	if (!QueryPerformanceFrequency(&li)) ERROR_AND_DIE("Gone");
//
//	PCFreq = double(li.QuadPart) / 1000.0;
//
//	QueryPerformanceCounter(&li);
//	CounterStart = li.QuadPart;
//}
//double GetCounter()
//{
//	LARGE_INTEGER li;
//	QueryPerformanceCounter(&li);
//	return double(li.QuadPart - CounterStart) / PCFreq;
//}
//