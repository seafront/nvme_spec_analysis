#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Device Self-Test Tests
//=============================================================================

TEST(DeviceSelfTestTest, DeviceSelfTestFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.DeviceSelfTest(nvme::DeviceSelfTestCode::ShortDeviceSelfTest));
}

TEST(DeviceSelfTestTest, ShortDeviceSelfTestSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.DeviceSelfTest(nvme::DeviceSelfTestCode::ShortDeviceSelfTest));
}

TEST(DeviceSelfTestTest, ExtendedDeviceSelfTestSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.DeviceSelfTest(nvme::DeviceSelfTestCode::ExtendedDeviceSelfTest));
}

TEST(DeviceSelfTestTest, AbortDeviceSelfTestSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.DeviceSelfTest(nvme::DeviceSelfTestCode::AbortDeviceSelfTest));
}

//=============================================================================
// Security Send/Receive Tests
//=============================================================================

TEST(SecurityTest, SecuritySendFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.SecuritySend(0x00, 0x0000, buffer, sizeof(buffer)));
}

TEST(SecurityTest, SecuritySendSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.SecuritySend(0x00, 0x0000, buffer, sizeof(buffer)));
}

TEST(SecurityTest, SecuritySendVariousProtocols) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    // Security Protocol 00h (Security Protocol Information)
    EXPECT_TRUE(device.SecuritySend(0x00, 0x0000, buffer, sizeof(buffer)));
    // TCG (Opal)
    EXPECT_TRUE(device.SecuritySend(0x01, 0x0001, buffer, sizeof(buffer)));
    // TCG (Enterprise)
    EXPECT_TRUE(device.SecuritySend(0x02, 0x0001, buffer, sizeof(buffer)));
}

TEST(SecurityTest, SecurityReceiveFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.SecurityReceive(0x00, 0x0000, buffer, sizeof(buffer)));
}

TEST(SecurityTest, SecurityReceiveSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.SecurityReceive(0x00, 0x0000, buffer, sizeof(buffer)));
}

TEST(SecurityTest, SecurityReceiveVariousProtocols) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.SecurityReceive(0x00, 0x0000, buffer, sizeof(buffer)));
    EXPECT_TRUE(device.SecurityReceive(0x01, 0x0001, buffer, sizeof(buffer)));
}

//=============================================================================
// Keep Alive Tests
//=============================================================================

TEST(KeepAliveTest, KeepAliveFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.KeepAlive());
}

TEST(KeepAliveTest, KeepAliveSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.KeepAlive());
}

TEST(KeepAliveTest, MultipleKeepAlives) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.KeepAlive());
    EXPECT_TRUE(device.KeepAlive());
    EXPECT_TRUE(device.KeepAlive());
}

//=============================================================================
// Abort Tests
//=============================================================================

TEST(AbortTest, AbortFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.Abort(0, 0));
}

TEST(AbortTest, AbortSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.Abort(0, 0));
}

TEST(AbortTest, AbortVariousSQIDsAndCIDs) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.Abort(0, 0));
    EXPECT_TRUE(device.Abort(1, 100));
    EXPECT_TRUE(device.Abort(65535, 65535));
}
