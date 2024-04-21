#include <sinaps.hpp>

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

    SINAPS_API bool patch_t::write(bool apply) {
        if (apply) {
            return mem::write(address, patch.data(), size);
        } else {
            return mem::write(address, original.data(), size);
        }
    }
}