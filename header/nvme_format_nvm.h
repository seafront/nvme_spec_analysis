#ifndef NVME_FORMAT_NVM_H
#define NVME_FORMAT_NVM_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 190)
//=============================================================================

/**
 * Secure Erase Settings Values
 * Figure 190
 */
enum class FormatNVMSecureErase : uint8_t {
    NoSecureErase     = 0b000, // No secure erase operation requested
    UserDataErase     = 0b001, // User Data Erase
    CryptographicErase = 0b010, // Cryptographic Erase
    // 0b011 to 0b111: Reserved
};

/**
 * Format NVM - Command Dword 10
 * Figure 190
 */
union FormatNVMCDW10 {
    struct {
        uint32_t LBAFL : 4;     // Bits 03:00 - LBA Format Lower
        uint32_t MSET : 1;      // Bit 04 - Metadata Settings
        uint32_t PI : 3;        // Bits 07:05 - Protection Information
        uint32_t PIL : 1;       // Bit 08 - Protection Information Location
        uint32_t SES : 3;       // Bits 11:09 - Secure Erase Settings
        uint32_t LBAFU : 2;     // Bits 13:12 - LBA Format Upper
        uint32_t Reserved : 18; // Bits 31:14 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 191)
//=============================================================================

/**
 * Format NVM - Command Specific Status Values
 * Figure 191 (SCT field set to 1h)
 */
enum class FormatNVMStatusCode : uint8_t {
    InvalidFormat = 0x0A, // Invalid Format
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(FormatNVMCDW10) == sizeof(uint32_t), "CDW10 size mismatch");

} // namespace nvme

#endif // NVME_FORMAT_NVM_H
