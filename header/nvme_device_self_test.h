#ifndef NVME_DEVICE_SELF_TEST_H
#define NVME_DEVICE_SELF_TEST_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 172, 173)
//=============================================================================

/**
 * Self-test Code Values
 * Figure 172
 */
enum class DeviceSelfTestCode : uint8_t {
    // 0x0: Reserved
    ShortDeviceSelfTest    = 0x1, // Start a short device self-test operation
    ExtendedDeviceSelfTest = 0x2, // Start an extended device self-test operation
    HostInitiatedRefresh   = 0x3, // Start a Host-Initiated Refresh operation
    // 0x4 to 0xD: Reserved
    VendorSpecific         = 0xE, // Vendor specific
    AbortDeviceSelfTest    = 0xF, // Abort device self-test operation
};

/**
 * Device Self-test - Command Dword 10
 * Figure 172
 */
union DeviceSelfTestCDW10 {
    struct {
        uint32_t STC : 4;       // Bits 03:00 - Self-test Code
        uint32_t Reserved : 28; // Bits 31:04 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Device Self-test - Command Dword 15
 * Figure 173
 */
union DeviceSelfTestCDW15 {
    struct {
        uint32_t DSTP : 32;     // Bits 31:00 - Device Self-test Parameter (vendor specific when STC=Eh)
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 175)
//=============================================================================

/**
 * Device Self-test - Command Specific Status Values
 * Figure 175 (SCT field set to 1h)
 */
enum class DeviceSelfTestStatusCode : uint8_t {
    DeviceSelfTestInProgress = 0x1D, // Device Self-test in Progress
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(DeviceSelfTestCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(DeviceSelfTestCDW15) == sizeof(uint32_t), "CDW15 size mismatch");

} // namespace nvme

#endif // NVME_DEVICE_SELF_TEST_H
