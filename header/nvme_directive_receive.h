#ifndef NVME_DIRECTIVE_RECEIVE_H
#define NVME_DIRECTIVE_RECEIVE_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 177, 178)
//=============================================================================

/**
 * Directive Receive - Command Dword 10
 * Figure 177
 */
union DirectiveReceiveCDW10 {
    struct {
        uint32_t NUMD : 32;     // Bits 31:00 - Number of Dwords (0's based)
    } bits;
    uint32_t raw;
};

/**
 * Directive Receive - Command Dword 11
 * Figure 178
 */
union DirectiveReceiveCDW11 {
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

static_assert(sizeof(DirectiveReceiveCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(DirectiveReceiveCDW11) == sizeof(uint32_t), "CDW11 size mismatch");

} // namespace nvme

#endif // NVME_DIRECTIVE_RECEIVE_H
