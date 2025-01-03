#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <msxml.h>    
#include <atomic>
#include <mutex>
#include <process.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <TlHelp32.h>
#include <list>
#include <string>
#include <string.h>
#include <iostream>
#include <map>
#include <dxgi.h>
#include <string>
#include <D3D11.h>
#include <vector>
#include <sstream>
#include <string>
#include <thread>

////////////////////////////
using namespace std;
////////////////////////////

////////////////////////////
using namespace this_thread;
////////////////////////////

////////////////////////////
using namespace chrono;
////////////////////////////



inline string customstring(float _Val) { 
	
	float val = roundf(_Val * 10.0f) / 10.0f;

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << val;
	std::string stringval = oss.str();

	return stringval;
}

std::mutex worlddly_mtx;

char* fast_strstr(const char* haystack, const char* needle)
{
	if (!*needle)
		return (char*)haystack;

	const char needle_first = *needle;

	haystack = strchr(haystack, needle_first);
	if (!haystack)
		return NULL;

	const char* i_haystack = haystack + 1
		, * i_needle = needle + 1;

	unsigned int  sums_diff = *haystack;
	bool          identical = true;

	while (*i_haystack && *i_needle)
	{
		sums_diff += *i_haystack;
		sums_diff -= *i_needle;
		identical &= *i_haystack++ == *i_needle++;
	}

	if (*i_needle)
		return NULL;
	else if (identical)
		return (char*)haystack;

	size_t needle_len = i_needle - needle;
	size_t needle_len_1 = needle_len - 1;

	const char* sub_start;
	for (sub_start = haystack; *i_haystack; i_haystack++)
	{
		sums_diff -= *sub_start++;
		sums_diff += *i_haystack;

		if (
			sums_diff == 0
			&& needle_first == *sub_start
			&& memcmp(sub_start, needle, needle_len_1) == 0
			)
			return (char*)sub_start;
	}

	return NULL;
}

int game_context;

wchar_t* fast_wcsstr(const wchar_t* haystack, const wchar_t* needle)
{
	if (!*needle)
		return (wchar_t*)haystack;

	const wchar_t needle_first = *needle;

	haystack = wcsrchr(haystack, needle_first);
	if (!haystack)
		return NULL;

	const wchar_t* i_haystack = haystack + 1
		, * i_needle = needle + 1;

	unsigned int  sums_diff = *haystack;
	bool          identical = true;

	while (*i_haystack && *i_needle)
	{
		sums_diff += *i_haystack;
		sums_diff -= *i_needle;
		identical &= *i_haystack++ == *i_needle++;
	}

	if (*i_needle)
		return NULL;
	else if (identical)
		return (wchar_t*)haystack;

	size_t needle_len = i_needle - needle;
	size_t needle_len_1 = needle_len - 1;

	const wchar_t* sub_start;
	for (sub_start = haystack; *i_haystack; i_haystack++)
	{
		sums_diff -= *sub_start++;
		sums_diff += *i_haystack;

		if (
			sums_diff == 0
			&& needle_first == *sub_start
			&& memcmp(sub_start, needle, needle_len_1) == 0
			)
			return (wchar_t*)sub_start;
	}

	return NULL;
}
////////////////////////////
////////////////////////////

#define debug true

////////////////////////////
////////////////////////////


void printinfo(const std::string& information, int color) {
	time_t now;
	time(&now);
	struct tm ltm;
	localtime_s(&ltm, &now);
	char timeBuffer[32];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &ltm);
	std::cout << "[" << timeBuffer << "] ";

	std::cout << "-> ";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	std::cout << "[information]: ";

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

////////////////////////////
////////////////////////////

void printdbg(const std::string& information, int color) {
	time_t now;
	time(&now);
	struct tm ltm;
	localtime_s(&ltm, &now);
	char timeBuffer[32];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &ltm);
	std::cout << "[" << timeBuffer << "] ";
	std::cout << "-> ";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	std::cout << "[debugger]: ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

////////////////////////////
////////////////////////////

void printfailure(const std::string& information, int color) {
	time_t now;
	time(&now);
	struct tm ltm;
	localtime_s(&ltm, &now);
	char timeBuffer[32];
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &ltm);
	std::cout << "[" << timeBuffer << "] ";
	std::cout << "-> ";
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	std::cout << "[failure detected]: ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

////////////////////////////
////////////////////////////

static DWORD get_process_id(const wchar_t* process_name) {
	DWORD process_id = 0;

	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snap_shot == INVALID_HANDLE_VALUE)
		return process_id;

	PROCESSENTRY32W entry = {};
	entry.dwSize = sizeof(decltype(entry));

	if (Process32FirstW(snap_shot, &entry) == TRUE) {
		if (_wcsicmp(process_name, entry.szExeFile) == 0) {
			process_id = entry.th32ProcessID;
		}
		else {
			while (Process32NextW(snap_shot, &entry) == TRUE) {
				if (_wcsicmp(process_name, entry.szExeFile) == 0) {
					process_id = entry.th32ProcessID;
					break;
				}
			}
		}
	}

	CloseHandle(snap_shot);
	return process_id;
}
////////////////////////////
uintptr_t pid;
////////////////////////////

mutex mtx;
condition_variable cv;
bool thread_running = true;
bool set_highest_priority() {
	BOOL result = SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	if (result == 0) {
		return false;
	}

	return true;
}

////////////////////////////
////////////////////////////

bool custom_exit() {
	cerr << "Something Failed. Contact Support." << endl;
	this_thread::sleep_for(seconds(5));
	exit(EXIT_FAILURE);

	return 0;
}

////////////////////////////
#define smv_api inline
////////////////////////////

////////////////////////////
////////////////////////////

namespace math_utils {

	smv_api float custom_sqrtf(float _X) {
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(_X)));
	}

	static smv_api void custom_Memcpy(const void* dstp, const void* srcp, UINT len) {
		ULONG* dst = (ULONG*)dstp;
		ULONG* src = (ULONG*)srcp;
		UINT i, tail;

		for (i = 0; i < (len / sizeof(ULONG)); i++)
			*dst++ = *src++;

		tail = len & (sizeof(ULONG) - 1);
		if (tail) {
			UCHAR* dstb = (UCHAR*)dstp;
			UCHAR* srcb = (UCHAR*)srcp;

			for (i = len - tail; i < len; i++)
				dstb[i] = srcb[i];
		}
	}

	smv_api float custom_sinf(float _X) {
		return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(_X)));
	}

	smv_api float custom_cosf(float _X) {
		return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(_X)));
	}

	smv_api float custom_acosf(float _X) {
		return _mm_cvtss_f32(_mm_acos_ps(_mm_set_ss(_X)));
	}

	smv_api float custom_tanf(float _X) {
		return _mm_cvtss_f32(_mm_tan_ps(_mm_set_ss(_X)));
	}

	smv_api float custom_atan2f(float _X, float _Y) {
		return _mm_cvtss_f32(_mm_atan2_ps(_mm_set_ss(_X), _mm_set_ss(_Y)));
	}

	smv_api float custom_pow(float _X, float _Y) {
		return _mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y)));
	}

	smv_api float custom_pow(float _X) {
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X)));
	}

	smv_api float custom_asinf(float _X) {
		return _mm_cvtss_f32(_mm_asin_ps(_mm_set_ss(_X)));
	}

	smv_api float custom_fabsf(float x) {
		__m128 x_vec = _mm_set_ss(x);
		x_vec = _mm_and_ps(x_vec, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
		return _mm_cvtss_f32(x_vec);
	}

	smv_api int custom_compare(const char* X, const char* Y) {
		while (*X && *Y) {
			if (*X != *Y) {
				return 0;
			}
			X++;
			Y++;
		}

		return (*Y == '\0');
	}

	smv_api int custom_wcompare(const wchar_t* X, const wchar_t* Y) {
		while (*X && *Y) {
			if (*X != *Y) {
				return 0;
			}
			X++;
			Y++;
		}

		return (*Y == L'\0');
	}

	smv_api const wchar_t* custom_wcsstr(const wchar_t* X, const wchar_t* Y) {
		while (*X != L'\0') {
			if ((*X == *Y) && custom_wcompare(X, Y)) {
				return X;
			}
			X++;
		}

		return NULL;
	}

	smv_api const char* custom_strstr(const char* X, const char* Y) {
		while (*X != '\0') {
			if ((*X == *Y) && custom_compare(X, Y)) {
				return X;
			}
			X++;
		}

		return NULL;
	}

	smv_api int custom_strlen(const char* string) {
		int cnt = 0;
		if (string) {
			for (; *string != 0; ++string) ++cnt;
		}

		return cnt;
	}

	smv_api int custom_wcslen(const wchar_t* string) {
		int cnt = 0;
		if (string) {
			for (; *string != 0; ++string) ++cnt;
		}

		return cnt;
	}

}

////////////////////////////
using namespace math_utils;
////////////////////////////