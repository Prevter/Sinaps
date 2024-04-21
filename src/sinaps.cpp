#include <sinaps.hpp>
#include <utility>

namespace sinaps {
    SINAPS_API bool patch_t::apply() {
        if (!applied && mem::write(address, patch.data(), size)) {
            applied = true;
            return true;
        }
        return false;
    }

    SINAPS_API bool patch_t::revert() {
        if (applied && mem::write(address, original.data(), size)) {
            applied = false;
            return true;
        }
        return false;
    }

    SINAPS_API bool patch_t::write(bool apply) const {
        if (apply) {
            return mem::write(address, patch.data(), size);
        } else {
            return mem::write(address, original.data(), size);
        }
    }

    static module_t defaultModule = {"", 0, 0};

    SINAPS_API void setDefaultModule(module_t module) {
        defaultModule = std::move(module);
    }

    SINAPS_API uintptr_t find(const std::string &pattern, const module_t& module) {
        return 0;
    }

    SINAPS_API uintptr_t find(const std::string &pattern) {
        return find(pattern, defaultModule);
    }

    SINAPS_API patch_t match(uintptr_t address, const std::string &mask) {
        return {};
    }

    SINAPS_API Result<patch_t> match(const std::string &pattern, const std::string &mask, const module_t& module) {
        return Result<patch_t>::fail("Not implemented");
    }

    SINAPS_API Result<patch_t> match(const std::string &pattern, const std::string &mask) {
        return match(pattern, mask, defaultModule);
    }

    SINAPS_API std::vector<patch_t> matchAll(const std::string &pattern, const std::string &mask, const module_t& module) {
        return {};
    }

    SINAPS_API std::vector<patch_t> matchAll(const std::string &pattern, const std::string &mask) {
        return matchAll(pattern, mask, defaultModule);
    }

    SINAPS_API std::vector<uintptr_t> findAll(const std::string &pattern, const std::string &mask, const module_t& module) {
        return {};
    }

    SINAPS_API std::vector<uintptr_t> findAll(const std::string &pattern, const std::string &mask) {
        return findAll(pattern, mask, defaultModule);
    }
}