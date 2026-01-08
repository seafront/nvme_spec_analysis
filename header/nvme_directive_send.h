#ifndef NVME_DIRECTIVE_SEND_H
#define NVME_DIRECTIVE_SEND_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 180, 181)
//=============================================================================

/**
 * Directive Send - Command Dword 10
 * Figure 180
 */
union DirectiveSendCDW10 {
    struct {
        uint32_t NUMD : 32;     // Bits 31:00 - Number of Dwords (0's based)
    } bits;
    uint32_t raw;
};

/**
 * Directive Send - Command Dword 11
 * Figure 181
 */
union DirectiveSendCDW11 {
    struct {
        uint32_t DOPER : 8;     // Bits 07:00 - Directive Operation
        uint32_t DTYPE : 8;     // Bits 15:08 - Directive Type
        uint32_t DSPEC : 16;    // Bits 31:16 - Directive Specific
    } bits;
    uint32_t raw;
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(DirectiveSendCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(DirectiveSendCDW11) == sizeof(uint32_t), "CDW11 size mismatch");

} // namespace nvme

#endif // NVME_DIRECTIVE_SEND_H
