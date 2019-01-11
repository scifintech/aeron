/*
 * Copyright 2014-2019 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AERON_DLOPEN_H
#define AERON_DLOPEN_H

#include <util/aeron_platform.h>

#if defined(AERON_COMPILER_GCC)

    #include <dlfcn.h>

#elif defined(AERON_COMPILER_MSVC) && defined(AERON_CPU_X64)

    #include <WinSock2.h> 
    #include <windows.h> 

    #define RTLD_DEFAULT -123    
    #define RTLD_NEXT -124
    #define RTLD_LAZY -125


#ifdef __cplusplus
extern "C"
{
#endif
    void* dlsym(HMODULE module, LPCSTR name);
    HMODULE dlopen(LPCSTR filename);
    char* dlerror();
#ifdef __cplusplus
}
#endif

#else
#error Unsupported platform!
#endif

#endif //AERON_DLOPEN_H
