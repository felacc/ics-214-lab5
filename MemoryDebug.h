#pragma once

// The code in this header file is explained in this resource:
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/find-memory-leaks-using-the-crt-library?view=msvc-170&viewFallbackFrom=vs-2019
// You can use it to help identify where memory leaks are created.  You will need to:
// 1) include this file in the file containing your main() function
// 2) add this line of code to the top of your main() function:
// 	    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
// 3) include this file in any files that allocate heap memory with new
// 4) replace any occurences of the new keyword, with DBG_NEW. 

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#ifdef _DEBUG												// if we are running in debug mode
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )	// swap any occurences of DBG_NEW with the code that follows it here
#else
#define DBG_NEW new											// else replace any occurences of DBG_NEW with new 
#endif