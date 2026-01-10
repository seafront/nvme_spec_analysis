#include <gtest/gtest.h>
#include "nvme.h"
#include <vector>

//=============================================================================
// Namespace Create Tests
//=============================================================================

TEST(NamespaceManagementTest, NamespaceCreateFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[4096] = {0};
    uint32_t nsid = 0;

    EXPECT_FALSE(device.NamespaceCreate(&nsid, buffer, sizeof(buffer)));
}

TEST(NamespaceManagementTest, NamespaceCreateSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};
    uint32_t nsid = 0;

    EXPECT_TRUE(device.NamespaceCreate(&nsid, buffer, sizeof(buffer)));
}

TEST(NamespaceManagementTest, NamespaceCreateWithNullNsid) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.NamespaceCreate(nullptr, buffer, sizeof(buffer)));
}

//=============================================================================
// Namespace Delete Tests
//=============================================================================

TEST(NamespaceManagementTest, NamespaceDeleteFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.NamespaceDelete(1));
}

TEST(NamespaceManagementTest, NamespaceDeleteSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.NamespaceDelete(1));
}

TEST(NamespaceManagementTest, NamespaceDeleteVariousNsids) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.NamespaceDelete(1));
    EXPECT_TRUE(device.NamespaceDelete(2));
    EXPECT_TRUE(device.NamespaceDelete(0xFFFFFFFF));
}

//=============================================================================
// Namespace Attach Tests
//=============================================================================

TEST(NamespaceManagementTest, NamespaceAttachFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    std::vector<uint16_t> controllers = {0, 1};

    EXPECT_FALSE(device.NamespaceAttach(1, controllers));
}

TEST(NamespaceManagementTest, NamespaceAttachSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    std::vector<uint16_t> controllers = {0, 1};

    EXPECT_TRUE(device.NamespaceAttach(1, controllers));
}

TEST(NamespaceManagementTest, NamespaceAttachEmptyControllerList) {
    nvme::NVMeDevice device("/dev/nvme0");
    std::vector<uint16_t> controllers;

    EXPECT_TRUE(device.NamespaceAttach(1, controllers));
}

//=============================================================================
// Namespace Detach Tests
//=============================================================================

TEST(NamespaceManagementTest, NamespaceDetachFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    std::vector<uint16_t> controllers = {0, 1};

    EXPECT_FALSE(device.NamespaceDetach(1, controllers));
}

TEST(NamespaceManagementTest, NamespaceDetachSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    std::vector<uint16_t> controllers = {0, 1};

    EXPECT_TRUE(device.NamespaceDetach(1, controllers));
}

TEST(NamespaceManagementTest, NamespaceDetachEmptyControllerList) {
    nvme::NVMeDevice device("/dev/nvme0");
    std::vector<uint16_t> controllers;

    EXPECT_TRUE(device.NamespaceDetach(1, controllers));
}
