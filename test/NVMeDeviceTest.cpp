#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// NVMeDevice Lifecycle Tests
//=============================================================================

TEST(NVMeDeviceTest, DefaultConstructor) {
    nvme::NVMeDevice device;
    EXPECT_FALSE(device.IsOpen());
}

TEST(NVMeDeviceTest, ConstructorWithPath) {
    nvme::NVMeDevice device("/dev/nvme0");
    EXPECT_TRUE(device.IsOpen());
}

TEST(NVMeDeviceTest, OpenAndClose) {
    nvme::NVMeDevice device;
    EXPECT_FALSE(device.IsOpen());

    EXPECT_TRUE(device.Open("/dev/nvme0"));
    EXPECT_TRUE(device.IsOpen());

    device.Close();
    EXPECT_FALSE(device.IsOpen());
}

TEST(NVMeDeviceTest, MultipleOpenClose) {
    nvme::NVMeDevice device;

    EXPECT_TRUE(device.Open("/dev/nvme0"));
    EXPECT_TRUE(device.IsOpen());
    device.Close();
    EXPECT_FALSE(device.IsOpen());

    EXPECT_TRUE(device.Open("/dev/nvme1"));
    EXPECT_TRUE(device.IsOpen());
    device.Close();
    EXPECT_FALSE(device.IsOpen());
}

//=============================================================================
// NVMeDevice Move Semantics Tests
//=============================================================================

TEST(NVMeDeviceTest, MoveConstructor) {
    nvme::NVMeDevice device1("/dev/nvme0");
    EXPECT_TRUE(device1.IsOpen());

    nvme::NVMeDevice device2(std::move(device1));
    EXPECT_TRUE(device2.IsOpen());
}

TEST(NVMeDeviceTest, MoveAssignment) {
    nvme::NVMeDevice device1("/dev/nvme0");
    nvme::NVMeDevice device2;

    EXPECT_TRUE(device1.IsOpen());
    EXPECT_FALSE(device2.IsOpen());

    device2 = std::move(device1);
    EXPECT_TRUE(device2.IsOpen());
}

//=============================================================================
// Error Handling Tests
//=============================================================================

TEST(NVMeDeviceTest, InitialErrorStateIsEmpty) {
    nvme::NVMeDevice device;

    EXPECT_TRUE(device.GetLastErrorString().empty());
    EXPECT_EQ(device.GetLastStatusCode(), 0u);
}

TEST(NVMeDeviceTest, ErrorSetWhenCommandFailsNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[4096] = {0};

    EXPECT_FALSE(device.IdentifyController(buffer, sizeof(buffer)));

    EXPECT_FALSE(device.GetLastErrorString().empty());
    EXPECT_NE(device.GetLastErrorString().find("not open"), std::string::npos);
}

TEST(NVMeDeviceTest, ErrorStringContainsDeviceNotOpen) {
    nvme::NVMeDevice device;

    device.KeepAlive();

    std::string error = device.GetLastErrorString();
    EXPECT_TRUE(error.find("not open") != std::string::npos ||
                error.find("Not open") != std::string::npos ||
                error.find("NOT OPEN") != std::string::npos);
}

TEST(NVMeDeviceTest, MultipleErrorsOverwrite) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    device.KeepAlive();
    std::string error1 = device.GetLastErrorString();

    device.GetSmartLog(buffer, sizeof(buffer));
    std::string error2 = device.GetLastErrorString();

    EXPECT_FALSE(error1.empty());
    EXPECT_FALSE(error2.empty());
}

TEST(NVMeDeviceTest, ErrorStateAfterSuccessfulCommand) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.IdentifyController(buffer, sizeof(buffer)));
    EXPECT_EQ(device.GetLastStatusCode(), 0u);
}
