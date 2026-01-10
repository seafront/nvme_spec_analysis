#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Format NVM Tests
//=============================================================================

TEST(FormatNVMTest, FormatNVMFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.FormatNVM(1, 0));
}

TEST(FormatNVMTest, FormatNVMSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FormatNVM(1, 0));
}

TEST(FormatNVMTest, FormatNVMWithSecureErase) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FormatNVM(1, 0, nvme::FormatNVMSecureErase::NoSecureErase));
    EXPECT_TRUE(device.FormatNVM(1, 0, nvme::FormatNVMSecureErase::UserDataErase));
    EXPECT_TRUE(device.FormatNVM(1, 0, nvme::FormatNVMSecureErase::CryptographicErase));
}

TEST(FormatNVMTest, FormatNVMVariousLBAFormats) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FormatNVM(1, 0));
    EXPECT_TRUE(device.FormatNVM(1, 1));
    EXPECT_TRUE(device.FormatNVM(1, 2));
    EXPECT_TRUE(device.FormatNVM(1, 15));
}

TEST(FormatNVMTest, FormatNVMVariousNamespaces) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FormatNVM(1, 0));
    EXPECT_TRUE(device.FormatNVM(2, 0));
    EXPECT_TRUE(device.FormatNVM(0xFFFFFFFF, 0));  // All namespaces
}

TEST(FormatNVMTest, FormatNVMCombinations) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Various combinations of NSID, LBAF, and SES
    EXPECT_TRUE(device.FormatNVM(1, 0, nvme::FormatNVMSecureErase::NoSecureErase));
    EXPECT_TRUE(device.FormatNVM(1, 1, nvme::FormatNVMSecureErase::UserDataErase));
    EXPECT_TRUE(device.FormatNVM(2, 0, nvme::FormatNVMSecureErase::CryptographicErase));
}
