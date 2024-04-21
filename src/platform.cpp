#include <sinaps/platform.hpp>

// Windows
#ifdef _WIN32

#include <Windows.h>

namespace sinaps::mem {

    SINAPS_API bool write(void *address, const void *data, size_t size) {
        if (address == nullptr || data == nullptr || size == 0) {
            return false;
        }

        DWORD oldProtect;
        if (!VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            return false;
        }

        memcpy(address, data, size);
        return VirtualProtect(address, size, oldProtect, &oldProtect);
    }

    SINAPS_API std::vector<uint8_t> read(void *address, size_t size) {
        if (address == nullptr || size == 0) {
            return {};
        }

        std::vector<uint8_t> buffer(size);
        memcpy(buffer.data(), address, size);

        return buffer;
    }

    SINAPS_API void *getModuleBase(const char *moduleName) {
        HMODULE module = GetModuleHandleA(moduleName);
        if (module == nullptr) {
            return nullptr;
        }

        return module;
    }

}
#endif