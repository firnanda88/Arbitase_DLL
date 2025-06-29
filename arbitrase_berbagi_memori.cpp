
#include <windows.h>
#include <string.h>

#define SHARED_MEMORY_NAME "ArbitraseSharedMemory"
#define BUFFER_SIZE 256

extern "C" __declspec(dllexport) void tulisData(const char* input) {
    HANDLE hMapFile;
    LPVOID pBuf;

    hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        BUFFER_SIZE,
        SHARED_MEMORY_NAME);

    if (hMapFile == NULL) return;

    pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
    if (pBuf == NULL) {
        CloseHandle(hMapFile);
        return;
    }

    memcpy(pBuf, input, strlen(input) + 1);
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}

extern "C" __declspec(dllexport) void bacaData(char* output, int length) {
    HANDLE hMapFile;
    LPCVOID pBuf;

    hMapFile = OpenFileMappingA(FILE_MAP_READ, FALSE, SHARED_MEMORY_NAME);
    if (hMapFile == NULL) return;

    pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, BUFFER_SIZE);
    if (pBuf == NULL) {
        CloseHandle(hMapFile);
        return;
    }

    strncpy(output, (const char*)pBuf, length - 1);
    output[length - 1] = '\0';

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}
