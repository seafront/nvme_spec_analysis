#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Get Features Tests
//=============================================================================

TEST(GetFeaturesTest, GetFeaturesFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint32_t result = 0;

    EXPECT_FALSE(device.GetFeatures(0x01, &result));
}

TEST(GetFeaturesTest, GetFeaturesSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint32_t result = 0;

    EXPECT_TRUE(device.GetFeatures(0x01, &result));
}

TEST(GetFeaturesTest, GetFeaturesWithNullResult) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.GetFeatures(0x01, nullptr));
}

TEST(GetFeaturesTest, GetFeaturesVariousFIDs) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint32_t result = 0;

    // Arbitration
    EXPECT_TRUE(device.GetFeatures(0x01, &result));
    // Power Management
    EXPECT_TRUE(device.GetFeatures(0x02, &result));
    // Temperature Threshold
    EXPECT_TRUE(device.GetFeatures(0x04, &result));
    // Error Recovery
    EXPECT_TRUE(device.GetFeatures(0x05, &result));
}

//=============================================================================
// Set Features Tests
//=============================================================================

TEST(SetFeaturesTest, SetFeaturesFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.SetFeatures(0x01, 0x1234));
}

TEST(SetFeaturesTest, SetFeaturesSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.SetFeatures(0x01, 0x1234));
}

TEST(SetFeaturesTest, SetFeaturesWithSave) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.SetFeatures(0x01, 0x1234, true));
    EXPECT_TRUE(device.SetFeatures(0x01, 0x1234, false));
}

TEST(SetFeaturesTest, SetFeaturesVariousFIDs) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Arbitration
    EXPECT_TRUE(device.SetFeatures(0x01, 0x00000000));
    // Power Management
    EXPECT_TRUE(device.SetFeatures(0x02, 0x00000000));
    // Number of Queues
    EXPECT_TRUE(device.SetFeatures(0x07, 0x00100010));
}
