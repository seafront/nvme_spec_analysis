#ifndef NVME_CAPACITY_MANAGEMENT_H
#define NVME_CAPACITY_MANAGEMENT_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 157, 158, 159)
//=============================================================================

/**
 * Operation Values for Capacity Management Command
 * Figure 157
 */
enum class CapacityManagementOperation : uint8_t {
    SelectCapacityConfiguration = 0x0, // Select Capacity Configuration
    CreateEnduranceGroup        = 0x1, // Create Endurance Group
    DeleteEnduranceGroup        = 0x2, // Delete Endurance Group
    CreateNVMSet                = 0x3, // Create NVM Set
    DeleteNVMSet                = 0x4, // Delete NVM Set
    // 0x5 to 0xF: Reserved
};

/**
 * Capacity Management - Command Dword 10
 * Figure 157
 */
union CapacityManagementCDW10 {
    struct {
        uint32_t OPER : 4;      // Bits 03:00 - Operation
        uint32_t Reserved : 12; // Bits 15:04 - Reserved
        uint32_t ELID : 16;     // Bits 31:16 - Element Identifier
    } bits;
    uint32_t raw;
};

/**
 * Capacity Management - Command Dword 11
 * Figure 158
 */
union CapacityManagementCDW11 {
    struct {
        uint32_t CAPL : 32;     // Bits 31:00 - Capacity Lower
    } bits;
    uint32_t raw;
};

/**
 * Capacity Management - Command Dword 12
 * Figure 159
 */
union CapacityManagementCDW12 {
    struct {
        uint32_t CAPU : 32;     // Bits 31:00 - Capacity Upper
    } bits;
    uint32_t raw;
};

//=============================================================================
// Completion Queue Entry Structures (Figure 161)
//=============================================================================

/**
 * Capacity Management - Completion Queue Entry Dword 0
 * Figure 161
 */
union CapacityManagementCQEDW0 {
    struct {
        uint32_t CELID : 16;    // Bits 15:00 - Created Element Identifier
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 160)
//=============================================================================

/**
 * Capacity Management - Command Specific Status Values
 * Figure 160 (SCT field set to 1h)
 */
enum class CapacityManagementStatusCode : uint8_t {
    InsufficientCapacity  = 0x26, // Insufficient Capacity
    IdentifierUnavailable = 0x2D, // Identifier Unavailable
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(CapacityManagementCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(CapacityManagementCDW11) == sizeof(uint32_t), "CDW11 size mismatch");
static_assert(sizeof(CapacityManagementCDW12) == sizeof(uint32_t), "CDW12 size mismatch");
static_assert(sizeof(CapacityManagementCQEDW0) == sizeof(uint32_t), "CQEDW0 size mismatch");

} // namespace nvme

#endif // NVME_CAPACITY_MANAGEMENT_H
