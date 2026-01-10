#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Get Log Page Tests
//=============================================================================

TEST(GetLogPageTest, GetLogPageFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.GetLogPage(0x01, 0, buffer, sizeof(buffer)));
    EXPECT_FALSE(device.GetSmartLog(buffer, sizeof(buffer)));
    EXPECT_FALSE(device.GetErrorLog(buffer, sizeof(buffer)));
    EXPECT_FALSE(device.GetFirmwareSlotInfo(buffer, sizeof(buffer)));
}

TEST(GetLogPageTest, GetLogPageSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.GetLogPage(0x01, 0, buffer, sizeof(buffer)));
}

TEST(GetLogPageTest, GetSmartLogSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.GetSmartLog(buffer, sizeof(buffer)));
}

TEST(GetLogPageTest, GetErrorLogSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.GetErrorLog(buffer, sizeof(buffer)));
}

TEST(GetLogPageTest, GetFirmwareSlotInfoSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.GetFirmwareSlotInfo(buffer, sizeof(buffer)));
}

TEST(GetLogPageTest, GetLogPageVariousLIDs) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    // Error Information (LID 0x01)
    EXPECT_TRUE(device.GetLogPage(0x01, 0, buffer, sizeof(buffer)));
    // SMART / Health Information (LID 0x02)
    EXPECT_TRUE(device.GetLogPage(0x02, 0xFFFFFFFF, buffer, sizeof(buffer)));
    // Firmware Slot Information (LID 0x03)
    EXPECT_TRUE(device.GetLogPage(0x03, 0, buffer, sizeof(buffer)));
    // Commands Supported and Effects (LID 0x05)
    EXPECT_TRUE(device.GetLogPage(0x05, 0, buffer, sizeof(buffer)));
}

TEST(GetLogPageTest, GetLogPageWithDifferentSizes) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t smallBuffer[64] = {0};
    uint8_t largeBuffer[4096] = {0};

    EXPECT_TRUE(device.GetLogPage(0x02, 0xFFFFFFFF, smallBuffer, sizeof(smallBuffer)));
    EXPECT_TRUE(device.GetLogPage(0x02, 0xFFFFFFFF, largeBuffer, sizeof(largeBuffer)));
}
