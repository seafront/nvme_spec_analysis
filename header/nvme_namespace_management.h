#ifndef NVME_NAMESPACE_MANAGEMENT_H
#define NVME_NAMESPACE_MANAGEMENT_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 368, 369)
//=============================================================================

/**
 * Select (SEL) Values for Namespace Management Command
 * Figure 368: Namespace Management - Command Dword 10
 */
enum class NamespaceManagementSelect : uint8_t {
    Create   = 0x0,  // Create namespace
    Delete   = 0x1,  // Delete namespace
    // 0x2 to 0xF: Reserved
};

/**
 * Namespace Management - Command Dword 10
 * Figure 368
 */
union NamespaceManagementCDW10 {
    struct {
        uint32_t SEL : 4;       // Bits 03:00 - Select (SEL)
        uint32_t Reserved : 28; // Bits 31:04 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Namespace Management - Command Dword 11
 * Figure 369
 */
union NamespaceManagementCDW11 {
    struct {
        uint32_t Reserved : 24; // Bits 23:00 - Reserved
        uint32_t CSI : 8;       // Bits 31:24 - Command Set Identifier (CSI)
    } bits;
    uint32_t raw;
};

//=============================================================================
// Completion Queue Entry Structures (Figure 372)
//=============================================================================

/**
 * Namespace Management - Completion Queue Entry Dword 0
 * Figure 372
 */
union NamespaceManagementCQEDW0 {
    struct {
        uint32_t NSID : 32;     // Bits 31:00 - Namespace Identifier (NSID)
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 371)
//=============================================================================

/**
 * Namespace Management - Command Specific Status Values
 * Figure 371 (SCT field set to 1h)
 */
enum class NamespaceManagementStatusCode : uint8_t {
    InvalidFormat                = 0x0A,  // Invalid Format
    NamespaceInsufficientCapacity = 0x15, // Namespace Insufficient Capacity
    NamespaceIdentifierUnavailable = 0x16, // Namespace Identifier Unavailable
    ThinProvisioningNotSupported = 0x1B,  // Thin Provisioning Not Supported
    ANAGroupIdentifierInvalid    = 0x24,  // ANA Group Identifier Invalid
    IOCommandSetNotSupported     = 0x29,  // I/O Command Set Not Supported
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(NamespaceManagementCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(NamespaceManagementCDW11) == sizeof(uint32_t), "CDW11 size mismatch");
static_assert(sizeof(NamespaceManagementCQEDW0) == sizeof(uint32_t), "CQEDW0 size mismatch");

} // namespace nvme

#endif // NVME_NAMESPACE_MANAGEMENT_H
