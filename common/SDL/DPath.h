#pragma once

#ifdef _WIN32
	#define PATH_CIN std::wcin
#else
	#define PATH_CIN std::cin
#endif