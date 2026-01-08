#ifndef NVME_FIRMWARE_IMAGE_DOWNLOAD_H
#define NVME_FIRMWARE_IMAGE_DOWNLOAD_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 186, 187)
//=============================================================================

/**
 * Firmware Image Download - Command Dword 10
 * Figure 186
 */
union FirmwareImageDownloadCDW10 {
    struct {
        uint32_t NUMD : 32;     // Bits 31:00 - Number of Dwords (0's based)
    } bits;
    uint32_t raw;
};

/**
 * Firmware Image Download - Command Dword 11
 * Figure 187
 */
union FirmwareImageDownloadCDW11 {
    struct {
        uint32_t OFST : 32;     // Bits 31:00 - Offset (in dwords)
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 188)
//=============================================================================

/**
 * Firmware Image Download - Command Specific Status Values
 * Figure 188 (SCT field set to 1h)
 */
enum class FirmwareImageDownloadStatusCode : uint8_t {
    OverlappingRange = 0x14, // Overlapping Range
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(FirmwareImageDownloadCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(FirmwareImageDownloadCDW11) == sizeof(uint32_t), "CDW11 size mismatch");

} // namespace nvme

#endif // NVME_FIRMWARE_IMAGE_DOWNLOAD_H
