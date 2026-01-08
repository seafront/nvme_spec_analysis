#ifndef NVME_SET_FEATURES_H
#define NVME_SET_FEATURES_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 384, 385)
//=============================================================================

/**
 * Set Features - Command Dword 10
 * Figure 384
 */
union SetFeaturesCDW10 {
    struct {
        uint32_t FID : 8;       // Bits 07:00 - Feature Identifier
        uint32_t Reserved : 23; // Bits 30:08 - Reserved
        uint32_t SV : 1;        // Bit 31 - Save
    } bits;
    uint32_t raw;
};

/**
 * Set Features - Command Dword 14
 * Figure 385
 */
union SetFeaturesCDW14 {
    struct {
        uint32_t UIDX : 7;      // Bits 06:00 - UUID Index
        uint32_t Reserved : 25; // Bits 31:07 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(SetFeaturesCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(SetFeaturesCDW14) == sizeof(uint32_t), "CDW14 size mismatch");

} // namespace nvme

#endif // NVME_SET_FEATURES_H
