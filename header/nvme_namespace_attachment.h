#ifndef NVME_NAMESPACE_ATTACHMENT_H
#define NVME_NAMESPACE_ATTACHMENT_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 365)
//=============================================================================

/**
 * Select Values for Namespace Attachment Command
 * Figure 365
 */
enum class NamespaceAttachmentSelect : uint8_t {
    ControllerAttach = 0x0, // Controller Attach
    ControllerDetach = 0x1, // Controller Detach
    // 0x2 to 0xF: Reserved
};

/**
 * Namespace Attachment - Command Dword 10
 * Figure 365
 */
union NamespaceAttachmentCDW10 {
    struct {
        uint32_t SEL : 4;       // Bits 03:00 - Select
        uint32_t Reserved : 28; // Bits 31:04 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 366)
//=============================================================================

/**
 * Namespace Attachment - Command Specific Status Values
 * Figure 366 (SCT field set to 1h)
 */
enum class NamespaceAttachmentStatusCode : uint8_t {
    NamespaceAlreadyAttached       = 0x18, // Namespace Already Attached
    NamespaceIsPrivate             = 0x19, // Namespace Is Private
    NamespaceNotAttached           = 0x1A, // Namespace Not Attached
    ControllerListInvalid          = 0x1C, // Controller List Invalid
    ANAAttachFailed                = 0x25, // ANA Attach Failed
    NamespaceAttachmentLimitExceeded = 0x27, // Namespace Attachment Limit Exceeded
    IOCommandSetNotSupported       = 0x29, // I/O Command Set Not Supported
    IOCommandSetNotEnabled         = 0x2A, // I/O Command Set Not Enabled
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(NamespaceAttachmentCDW10) == sizeof(uint32_t), "CDW10 size mismatch");

} // namespace nvme

#endif // NVME_NAMESPACE_ATTACHMENT_H
