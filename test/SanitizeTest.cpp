#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Sanitize Tests
//=============================================================================

TEST(SanitizeTest, SanitizeFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.Sanitize(nvme::SanitizeAction::BlockErase));
}

TEST(SanitizeTest, SanitizeBlockEraseSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::BlockErase));
}

TEST(SanitizeTest, SanitizeOverwriteSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::Overwrite, 0xDEADBEEF));
}

TEST(SanitizeTest, SanitizeCryptoEraseSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::CryptoErase));
}

TEST(SanitizeTest, SanitizeExitFailureModeSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::ExitFailureMode));
}

TEST(SanitizeTest, SanitizeOverwriteWithVariousPatterns) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::Overwrite, 0x00000000));
    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::Overwrite, 0xFFFFFFFF));
    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::Overwrite, 0xAAAAAAAA));
    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::Overwrite, 0x55555555));
}

TEST(SanitizeTest, SanitizeDefaultOverwritePattern) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Default pattern is 0
    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::Overwrite));
}
