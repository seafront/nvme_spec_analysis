#ifndef NVME_ABORT_H
#define NVME_ABORT_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 144)
//=============================================================================

/**
 * Abort - Command Dword 10
 * Figure 144
 */
union AbortCDW10 {
    struct {
        uint32_t SQID : 16;     // Bits 15:00 - Submission Queue Identifier
        uint32_t CID : 16;      // Bits 31:16 - Command Identifier
    } bits;
    uint32_t raw;
};

//=============================================================================
// Completion Queue Entry Structures (Figure 145)
//=============================================================================

/**
 * Abort - Completion Queue Entry Dword 0
 * Figure 145
 */
union AbortCQEDW0 {
    struct {
        uint32_t IANP : 1;      // Bit 00 - Immediate Abort Not Performed
        uint32_t Reserved : 31; // Bits 31:01 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 146)
//=============================================================================

/**
 * Abort - Command Specific Status Values
 * Figure 146 (SCT field set to 1h)
 */
enum class AbortStatusCode : uint8_t {
    AbortCommandLimitExceeded = 0x03,  // Abort Command Limit Exceeded
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(AbortCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(AbortCQEDW0) == sizeof(uint32_t), "CQEDW0 size mismatch");

} // namespace nvme

#endif // NVME_ABORT_H
