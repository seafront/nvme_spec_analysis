#ifndef NVME_SECURITY_H
#define NVME_SECURITY_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Security Receive Command Structures (Figure 377, 378)
//=============================================================================

/**
 * Security Receive - Command Dword 10
 * Figure 377
 */
union SecurityReceiveCDW10 {
    struct {
        uint32_t NSSF : 8;      // Bits 07:00 - NVMe Security Specific Field
        uint32_t SPSP0 : 8;     // Bits 15:08 - SP Specific 0
        uint32_t SPSP1 : 8;     // Bits 23:16 - SP Specific 1
        uint32_t SECP : 8;      // Bits 31:24 - Security Protocol
    } bits;
    uint32_t raw;
};

/**
 * Security Receive - Command Dword 11
 * Figure 378
 */
union SecurityReceiveCDW11 {
    struct {
        uint32_t AL : 32;       // Bits 31:00 - Allocation Length
    } bits;
    uint32_t raw;
};

//=============================================================================
// Security Send Command Structures (Figure 381, 382)
//=============================================================================

/**
 * Security Send - Command Dword 10
 * Figure 381
 */
union SecuritySendCDW10 {
    struct {
        uint32_t NSSF : 8;      // Bits 07:00 - NVMe Security Specific Field
        uint32_t SPSP0 : 8;     // Bits 15:08 - SP Specific 0
        uint32_t SPSP1 : 8;     // Bits 23:16 - SP Specific 1
        uint32_t SECP : 8;      // Bits 31:24 - Security Protocol
    } bits;
    uint32_t raw;
};

/**
 * Security Send - Command Dword 11
 * Figure 382
 */
union SecuritySendCDW11 {
    struct {
        uint32_t TL : 32;       // Bits 31:00 - Transfer Length
    } bits;
    uint32_t raw;
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(SecurityReceiveCDW10) == sizeof(uint32_t), "SecurityReceive CDW10 size mismatch");
static_assert(sizeof(SecurityReceiveCDW11) == sizeof(uint32_t), "SecurityReceive CDW11 size mismatch");
static_assert(sizeof(SecuritySendCDW10) == sizeof(uint32_t), "SecuritySend CDW10 size mismatch");
static_assert(sizeof(SecuritySendCDW11) == sizeof(uint32_t), "SecuritySend CDW11 size mismatch");

} // namespace nvme

#endif // NVME_SECURITY_H
