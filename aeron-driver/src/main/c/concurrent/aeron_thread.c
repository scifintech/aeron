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

#include "concurrent/aeron_thread.h"


#if defined(AERON_COMPILER_GCC)

#elif defined(AERON_COMPILER_MSVC) && defined(AERON_CPU_X64)

void pthread_once(AERON_INIT_ONCE* s_init_once, void* callback)
{
    InitOnceExecuteOnce(s_init_once, (PINIT_ONCE_FN)callback, NULL, NULL);
}

void aeron_mutex_init(HANDLE* mutex, void* attr)
{
    *mutex = CreateMutexA(NULL, FALSE, NULL);
}

void aeron_mutex_lock(HANDLE* mutex)
{
    WaitForSingleObject(mutex, INFINITE);
}

void aeron_mutex_unlock(HANDLE* mutex)
{
    ReleaseMutex(mutex);
}

int pthread_attr_init(pthread_attr_t* attr) { return 0; }

int pthread_create(aeron_thread_t* thread, void* attr, void*(*callback)(void*), void* arg0)
{
    DWORD id;
    *thread = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        callback,       // thread function name
        arg0,          // argument to thread function 
        0,                      // use default creation flags 
        &id);   // returns the thread identifier 
    return 0;
}

void pthread_setname_np(aeron_thread_t self, const char* role_name)
{
    size_t wn = mbstowcs(NULL, role_name, 0);
    wchar_t * buf = malloc(sizeof(wchar_t) * (wn + 1));  // value-initialize to 0 (see below)
    mbstowcs(buf, role_name, wn + 1);
    SetThreadDescription(self, buf);

    free(buf);
}

aeron_thread_t pthread_self()
{
    return GetCurrentThread();
}

DWORD pthread_join(aeron_thread_t thread, void **value_ptr)
{
    WaitForSingleObject(thread, INFINITE);
    return 0;
}

int pthread_key_create(pthread_key_t *key, void(*destr_function) (void *))
{
    DWORD dkey = TlsAlloc();
    if (dkey != 0xFFFFFFFF) {
        *key = dkey;
        return 0;
    }
    else {
        return EAGAIN;
    }
}

int pthread_key_delete(pthread_key_t key)
{
    if (TlsFree(key)) {
        return 0;
    }
    else {
        return EINVAL;
    }
}

int pthread_setspecific(pthread_key_t key, const void *pointer)
{
    if (TlsSetValue(key, (LPVOID)pointer)) {
        return 0;
    }
    else {
        return EINVAL;
    }
}

void * pthread_getspecific(pthread_key_t key)
{
    return TlsGetValue(key);
}


#else
#error Unsupported platform!
#endif

 // sched

#if defined(AERON_COMPILER_GCC)

#include <sched.h>

void proc_yield()
{
    __asm__ volatile("pause\n": : : "memory");
}


#elif defined(AERON_COMPILER_MSVC) && defined(AERON_CPU_X64)

#else
#error Unsupported platform!
#endif


