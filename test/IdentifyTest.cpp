#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Identify Command Tests
//=============================================================================

TEST(IdentifyTest, IdentifyFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[4096] = {0};

    EXPECT_FALSE(device.IdentifyController(buffer, sizeof(buffer)));
    EXPECT_FALSE(device.IdentifyNamespace(1, buffer, sizeof(buffer)));
    EXPECT_FALSE(device.Identify(nvme::IdentifyCNS::IdentifyController, 0, buffer, sizeof(buffer)));
}

TEST(IdentifyTest, IdentifyControllerSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.IdentifyController(buffer, sizeof(buffer)));
}

TEST(IdentifyTest, IdentifyNamespaceSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.IdentifyNamespace(1, buffer, sizeof(buffer)));
}

TEST(IdentifyTest, IdentifyWithCNSSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.Identify(nvme::IdentifyCNS::IdentifyController, 0, buffer, sizeof(buffer)));
    EXPECT_TRUE(device.Identify(nvme::IdentifyCNS::IdentifyNamespace, 1, buffer, sizeof(buffer)));
}

TEST(IdentifyTest, IdentifyMultipleNamespaces) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.IdentifyNamespace(1, buffer, sizeof(buffer)));
    EXPECT_TRUE(device.IdentifyNamespace(2, buffer, sizeof(buffer)));
    EXPECT_TRUE(device.IdentifyNamespace(0xFFFFFFFF, buffer, sizeof(buffer)));
}
