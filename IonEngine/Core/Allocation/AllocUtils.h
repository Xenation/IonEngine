#pragma once
#include <Windows.h>

static inline size_t __getVirtualPageSize() {
	SYSTEM_INFO sysInfo = {};
	GetSystemInfo(&sysInfo);
	return static_cast<size_t>(sysInfo.dwPageSize);
}
static const size_t virtPageSize = __getVirtualPageSize();


static inline void* reserveVirtualAddress(void* addr, size_t size) {
	return VirtualAlloc(addr, size, MEM_RESERVE, PAGE_NOACCESS);
}

static inline void* reserveVirtualAddress(size_t size) {
	return reserveVirtualAddress(nullptr, size);
}

static inline void* virtAlloc(void* addr, size_t size) {
	return VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE);
}

static inline void* virtAlloc(size_t size) {
	return virtAlloc(nullptr, size);
}

static inline void virtFree(void* addr, size_t size) {
	VirtualFree(addr, size, MEM_DECOMMIT);
}

static inline void virtFree(void* addr) {
	virtFree(addr, 0);
}

static inline void releaseVirtualAddress(void* addr) {
	VirtualFree(addr, 0, MEM_RELEASE);
}
