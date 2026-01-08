#ifndef NVME_IDENTIFY_H
#define NVME_IDENTIFY_H

#include <cstdint>

namespace nvme {

//=============================================================================
// CNS Values (Figure 311)
//=============================================================================

/**
 * Controller or Namespace Structure (CNS) Values
 * Figure 311
 */
enum class IdentifyCNS : uint8_t {
    IdentifyNamespace              = 0x00, // Identify Namespace data structure
    IdentifyController             = 0x01, // Identify Controller data structure
    ActiveNamespaceIdList          = 0x02, // Active Namespace ID list
    NamespaceIdDescriptorList      = 0x03, // Namespace Identification Descriptor list
    NVMSetList                     = 0x04, // NVM Set List
    IOCommandSetNamespace          = 0x05, // I/O Command Set specific Identify Namespace
    IOCommandSetController         = 0x06, // I/O Command Set specific Identify Controller
    IOCommandSetActiveNSIDList     = 0x07, // Active Namespace ID list for I/O Command Set
    IOCommandSetIndependentNS      = 0x08, // I/O Command Set Independent Identify Namespace
    AllocatedNamespaceIdList       = 0x10, // Allocated Namespace ID list
    IdentifyAllocatedNamespace     = 0x11, // Identify Namespace for allocated NSID
    ControllerListAttached         = 0x12, // Controller List attached to NSID
    ControllerListAll              = 0x13, // Controller List in NVM subsystem
    PrimaryControllerCapabilities  = 0x14, // Primary Controller Capabilities
    SecondaryControllerList        = 0x15, // Secondary Controller list
    NamespaceGranularityList       = 0x16, // Namespace Granularity List
    UUIDList                       = 0x17, // UUID List
    DomainList                     = 0x18, // Domain List
    EnduranceGroupList             = 0x19, // Endurance Group List
    IOCommandSetAllocatedNSIDList  = 0x1A, // I/O Command Set specific Allocated Namespace ID list
    IOCommandSetAllocatedNS        = 0x1B, // I/O Command Set specific Identify Namespace for allocated NSID
    IOCommandSetDataStructure      = 0x1C, // I/O Command Set data structure
};

//=============================================================================
// Command Dword Structures (Figure 308-310)
//=============================================================================

/**
 * Identify - Command Dword 10
 * Figure 308
 */
union IdentifyCDW10 {
    struct {
        uint32_t CNS : 8;       // Bits 07:00 - Controller or Namespace Structure
        uint32_t Reserved : 8;  // Bits 15:08 - Reserved
        uint32_t CNTID : 16;    // Bits 31:16 - Controller Identifier
    } bits;
    uint32_t raw;
};

/**
 * Identify - Command Dword 11
 * Figure 309
 */
union IdentifyCDW11 {
    struct {
        uint32_t CNSSID : 16;   // Bits 15:00 - CNS Specific Identifier
        uint32_t Reserved : 8;  // Bits 23:16 - Reserved
        uint32_t CSI : 8;       // Bits 31:24 - Command Set Identifier
    } bits;
    uint32_t raw;
};

/**
 * Identify - Command Dword 14
 * Figure 310
 */
union IdentifyCDW14 {
    struct {
        uint32_t UIDX : 7;      // Bits 06:00 - UUID Index
        uint32_t Reserved : 25; // Bits 31:07 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(IdentifyCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(IdentifyCDW11) == sizeof(uint32_t), "CDW11 size mismatch");
static_assert(sizeof(IdentifyCDW14) == sizeof(uint32_t), "CDW14 size mismatch");

} // namespace nvme

#endif // NVME_IDENTIFY_H
