#include <gtest/gtest.h>
#include <cstring>
#include "nvme.h"

//=============================================================================
// NVM Read Command Tests
//=============================================================================

TEST(NvmReadTest, ReadFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[4096] = {0};

    EXPECT_FALSE(device.Read(1, 0, 0, buffer, sizeof(buffer)));
}

TEST(NvmReadTest, ReadFailsWithNullBuffer) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_FALSE(device.Read(1, 0, 0, nullptr, 4096));
}

TEST(NvmReadTest, ReadSucceedsWithValidParameters) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // Read 1 block (nlb=0 means 1 block) from LBA 0
    EXPECT_TRUE(device.Read(1, 0, 0, buffer, sizeof(buffer)));
}

TEST(NvmReadTest, ReadWithFUA) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // Read with Force Unit Access
    EXPECT_TRUE(device.Read(1, 0, 0, buffer, sizeof(buffer), true, false));
}

TEST(NvmReadTest, ReadWithLimitedRetry) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // Read with Limited Retry
    EXPECT_TRUE(device.Read(1, 0, 0, buffer, sizeof(buffer), false, true));
}

TEST(NvmReadTest, ReadMultipleBlocks) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[8192] = {0};

    // Read 2 blocks (nlb=1 means 2 blocks, 0's based)
    EXPECT_TRUE(device.Read(1, 0, 1, buffer, sizeof(buffer)));
}

TEST(NvmReadTest, ReadFromHighLBA) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // Read from a high LBA (tests 64-bit LBA handling)
    uint64_t highLBA = 0x100000000ULL;  // > 32-bit
    EXPECT_TRUE(device.Read(1, highLBA, 0, buffer, sizeof(buffer)));
}

//=============================================================================
// NVM Write Command Tests
//=============================================================================

TEST(NvmWriteTest, WriteFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[4096] = {0};

    EXPECT_FALSE(device.Write(1, 0, 0, buffer, sizeof(buffer)));
}

TEST(NvmWriteTest, WriteFailsWithNullBuffer) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_FALSE(device.Write(1, 0, 0, nullptr, 4096));
}

TEST(NvmWriteTest, WriteAndReadBackVerify) {
    // Given: A device is open and write buffer is prepared with test pattern
    nvme::NVMeDevice device("/dev/nvme0");
    constexpr size_t blockSize = 4096;
    uint8_t writeBuffer[blockSize];
    uint8_t readBuffer[blockSize];

    // Fill write buffer with test pattern
    for (size_t i = 0; i < blockSize; ++i) {
        writeBuffer[i] = static_cast<uint8_t>(i & 0xFF);
    }
    std::memset(readBuffer, 0, blockSize);

    // When: Write 1 block to LBA 0, then read it back
    ASSERT_TRUE(device.Write(1, 0, 0, writeBuffer, blockSize));
    ASSERT_TRUE(device.Read(1, 0, 0, readBuffer, blockSize));

    // Then: Read data shall match written data
    EXPECT_EQ(std::memcmp(writeBuffer, readBuffer, blockSize), 0);
}

TEST(NvmWriteTest, WriteAndReadBackVerifyWithFUA) {
    // Given: A device is open with FUA (Force Unit Access) enabled
    nvme::NVMeDevice device("/dev/nvme0");
    constexpr size_t blockSize = 4096;
    uint8_t writeBuffer[blockSize];
    uint8_t readBuffer[blockSize];

    // Fill write buffer with unique pattern
    for (size_t i = 0; i < blockSize; ++i) {
        writeBuffer[i] = static_cast<uint8_t>((i * 7) & 0xFF);
    }
    std::memset(readBuffer, 0, blockSize);

    // When: Write with FUA, then read back with FUA
    ASSERT_TRUE(device.Write(1, 0, 0, writeBuffer, blockSize, true, false));
    ASSERT_TRUE(device.Read(1, 0, 0, readBuffer, blockSize, true, false));

    // Then: Read data shall match written data
    EXPECT_EQ(std::memcmp(writeBuffer, readBuffer, blockSize), 0);
}

TEST(NvmWriteTest, WriteAndReadBackMultipleBlocks) {
    // Given: A device is open and multi-block write buffer is prepared
    nvme::NVMeDevice device("/dev/nvme0");
    constexpr size_t blockSize = 4096;
    constexpr uint16_t numBlocks = 4;  // nlb is 0's based, so 3 means 4 blocks
    constexpr size_t totalSize = blockSize * numBlocks;
    uint8_t writeBuffer[totalSize];
    uint8_t readBuffer[totalSize];

    // Fill write buffer with block-specific pattern
    for (size_t i = 0; i < totalSize; ++i) {
        writeBuffer[i] = static_cast<uint8_t>((i / blockSize) ^ (i & 0xFF));
    }
    std::memset(readBuffer, 0, totalSize);

    // When: Write 4 blocks (nlb=3) starting from LBA 0, then read them back
    ASSERT_TRUE(device.Write(1, 0, numBlocks - 1, writeBuffer, totalSize));
    ASSERT_TRUE(device.Read(1, 0, numBlocks - 1, readBuffer, totalSize));

    // Then: All blocks read shall match written data
    EXPECT_EQ(std::memcmp(writeBuffer, readBuffer, totalSize), 0);
}

TEST(NvmWriteTest, WriteAndReadBackAtDifferentLBA) {
    // Given: A device is open
    nvme::NVMeDevice device("/dev/nvme0");
    constexpr size_t blockSize = 4096;
    uint8_t writeBuffer[blockSize];
    uint8_t readBuffer[blockSize];
    const uint64_t testLBA = 100;

    // Fill write buffer with LBA-based pattern
    for (size_t i = 0; i < blockSize; ++i) {
        writeBuffer[i] = static_cast<uint8_t>((testLBA + i) & 0xFF);
    }
    std::memset(readBuffer, 0, blockSize);

    // When: Write to specific LBA, then read from same LBA
    ASSERT_TRUE(device.Write(1, testLBA, 0, writeBuffer, blockSize));
    ASSERT_TRUE(device.Read(1, testLBA, 0, readBuffer, blockSize));

    // Then: Read data shall match written data
    EXPECT_EQ(std::memcmp(writeBuffer, readBuffer, blockSize), 0);
}

TEST(NvmWriteTest, WriteAndReadBackHighLBA) {
    // Given: A device is open with high LBA address (tests 64-bit LBA)
    nvme::NVMeDevice device("/dev/nvme0");
    constexpr size_t blockSize = 4096;
    uint8_t writeBuffer[blockSize];
    uint8_t readBuffer[blockSize];
    const uint64_t highLBA = 0x100000000ULL;  // > 32-bit

    // Fill write buffer with pattern
    for (size_t i = 0; i < blockSize; ++i) {
        writeBuffer[i] = static_cast<uint8_t>(i ^ 0xAA);
    }
    std::memset(readBuffer, 0, blockSize);

    // When: Write to high LBA, then read back
    ASSERT_TRUE(device.Write(1, highLBA, 0, writeBuffer, blockSize));
    ASSERT_TRUE(device.Read(1, highLBA, 0, readBuffer, blockSize));

    // Then: Read data shall match written data
    EXPECT_EQ(std::memcmp(writeBuffer, readBuffer, blockSize), 0);
}

TEST(NvmWriteTest, WriteZeroPattern) {
    // Given: A device is open and write buffer is all zeros
    nvme::NVMeDevice device("/dev/nvme0");
    constexpr size_t blockSize = 4096;
    uint8_t writeBuffer[blockSize];
    uint8_t readBuffer[blockSize];

    std::memset(writeBuffer, 0x00, blockSize);
    std::memset(readBuffer, 0xFF, blockSize);  // Initialize with different pattern

    // When: Write zero pattern, then read back
    ASSERT_TRUE(device.Write(1, 0, 0, writeBuffer, blockSize));
    ASSERT_TRUE(device.Read(1, 0, 0, readBuffer, blockSize));

    // Then: Read data shall be all zeros
    EXPECT_EQ(std::memcmp(writeBuffer, readBuffer, blockSize), 0);
}

TEST(NvmWriteTest, WriteAllOnesPattern) {
    // Given: A device is open and write buffer is all 0xFF
    nvme::NVMeDevice device("/dev/nvme0");
    constexpr size_t blockSize = 4096;
    uint8_t writeBuffer[blockSize];
    uint8_t readBuffer[blockSize];

    std::memset(writeBuffer, 0xFF, blockSize);
    std::memset(readBuffer, 0x00, blockSize);  // Initialize with different pattern

    // When: Write all-ones pattern, then read back
    ASSERT_TRUE(device.Write(1, 0, 0, writeBuffer, blockSize));
    ASSERT_TRUE(device.Read(1, 0, 0, readBuffer, blockSize));

    // Then: Read data shall be all 0xFF
    EXPECT_EQ(std::memcmp(writeBuffer, readBuffer, blockSize), 0);
}

//=============================================================================
// Dataset Management (DSM/TRIM) Command Tests
//=============================================================================

TEST(DatasetManagementTest, DSMFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    nvme::DatasetManagementRange ranges[1] = {};
    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;

    EXPECT_FALSE(device.DatasetManagement(1, ranges, 1));
}

TEST(DatasetManagementTest, DSMFailsWithNullRanges) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_FALSE(device.DatasetManagement(1, nullptr, 1));
}

TEST(DatasetManagementTest, DSMFailsWithZeroRanges) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[1] = {};

    EXPECT_FALSE(device.DatasetManagement(1, ranges, 0));
}

TEST(DatasetManagementTest, DSMSucceedsWithSingleRange) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[1] = {};
    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;
    ranges[0].ContextAttributes = 0;

    EXPECT_TRUE(device.DatasetManagement(1, ranges, 1));
}

TEST(DatasetManagementTest, DSMSucceedsWithMultipleRanges) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[4] = {};

    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;
    ranges[1].StartingLBA = 100;
    ranges[1].LengthInLBs = 16;
    ranges[2].StartingLBA = 200;
    ranges[2].LengthInLBs = 32;
    ranges[3].StartingLBA = 1000;
    ranges[3].LengthInLBs = 64;

    EXPECT_TRUE(device.DatasetManagement(1, ranges, 4));
}

TEST(DatasetManagementTest, DSMWithDeallocateFlag) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[1] = {};
    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;

    // Deallocate (TRIM) operation
    EXPECT_TRUE(device.DatasetManagement(1, ranges, 1, true, false, false));
}

TEST(DatasetManagementTest, DSMWithIntegralDatasetForRead) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[1] = {};
    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;

    // Integral Dataset for Read
    EXPECT_TRUE(device.DatasetManagement(1, ranges, 1, false, true, false));
}

TEST(DatasetManagementTest, DSMWithIntegralDatasetForWrite) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[1] = {};
    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;

    // Integral Dataset for Write
    EXPECT_TRUE(device.DatasetManagement(1, ranges, 1, false, false, true));
}

TEST(DatasetManagementTest, DSMWithAllFlags) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[1] = {};
    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;

    // All flags set
    EXPECT_TRUE(device.DatasetManagement(1, ranges, 1, true, true, true));
}

//=============================================================================
// DSM Context Attributes Tests
//=============================================================================

TEST(DSMContextAttributesTest, ContextAttributesBitfields) {
    nvme::DSMContextAttributes attr{};
    attr.raw = 0;

    // Test Access Frequency field (bits 3:0)
    attr.bits.AF = static_cast<uint32_t>(nvme::DSMAccessFrequency::FrequentWrites);
    EXPECT_EQ(attr.bits.AF, 5);

    // Test Access Latency field (bits 5:4)
    attr.bits.AL = static_cast<uint32_t>(nvme::DSMAccessLatency::Low);
    EXPECT_EQ(attr.bits.AL, 3);

    // Test Sequential Read Range (bit 8)
    attr.bits.SR = 1;
    EXPECT_EQ(attr.bits.SR, 1);

    // Test Sequential Write Range (bit 9)
    attr.bits.SW = 1;
    EXPECT_EQ(attr.bits.SW, 1);

    // Test Write Prepare (bit 10)
    attr.bits.WP = 1;
    EXPECT_EQ(attr.bits.WP, 1);
}

TEST(DSMContextAttributesTest, DSMWithContextAttributes) {
    nvme::NVMeDevice device("/dev/nvme0");
    nvme::DatasetManagementRange ranges[1] = {};

    // Set up context attributes
    nvme::DSMContextAttributes attr{};
    attr.bits.AF = static_cast<uint32_t>(nvme::DSMAccessFrequency::Typical);
    attr.bits.AL = static_cast<uint32_t>(nvme::DSMAccessLatency::Normal);
    attr.bits.SR = 1;

    ranges[0].ContextAttributes = attr.raw;
    ranges[0].StartingLBA = 0;
    ranges[0].LengthInLBs = 8;

    EXPECT_TRUE(device.DatasetManagement(1, ranges, 1, true));
}

//=============================================================================
// NVM Command CDW Structure Tests
//=============================================================================

TEST(NvmCDWTest, ReadCDW12Bitfields) {
    nvme::ReadCDW12 cdw12{};
    cdw12.raw = 0;

    // Test NLB field (bits 15:0)
    cdw12.bits.NLB = 255;
    EXPECT_EQ(cdw12.bits.NLB, 255);

    // Test FUA field (bit 30)
    cdw12.bits.FUA = 1;
    EXPECT_EQ(cdw12.bits.FUA, 1);

    // Test LR field (bit 31)
    cdw12.bits.LR = 1;
    EXPECT_EQ(cdw12.bits.LR, 1);
}

TEST(NvmCDWTest, WriteCDW12Bitfields) {
    nvme::WriteCDW12 cdw12{};
    cdw12.raw = 0;

    // Test NLB field (bits 15:0)
    cdw12.bits.NLB = 511;
    EXPECT_EQ(cdw12.bits.NLB, 511);

    // Test FUA field (bit 30)
    cdw12.bits.FUA = 1;
    EXPECT_EQ(cdw12.bits.FUA, 1);

    // Test LR field (bit 31)
    cdw12.bits.LR = 1;
    EXPECT_EQ(cdw12.bits.LR, 1);
}

TEST(NvmCDWTest, DatasetManagementCDW10Bitfields) {
    nvme::DatasetManagementCDW10 cdw10{};
    cdw10.raw = 0;

    // Test NR field (bits 7:0) - 0's based number of ranges
    cdw10.bits.NR = 255;  // Max ranges (256 - 1)
    EXPECT_EQ(cdw10.bits.NR, 255);
}

TEST(NvmCDWTest, DatasetManagementCDW11Bitfields) {
    nvme::DatasetManagementCDW11 cdw11{};
    cdw11.raw = 0;

    // Test IDR field (bit 0)
    cdw11.bits.IDR = 1;
    EXPECT_EQ(cdw11.bits.IDR, 1);

    // Test IDW field (bit 1)
    cdw11.bits.IDW = 1;
    EXPECT_EQ(cdw11.bits.IDW, 1);

    // Test AD field (bit 2) - Deallocate
    cdw11.bits.AD = 1;
    EXPECT_EQ(cdw11.bits.AD, 1);
}

//=============================================================================
// NVM Opcode Enum Tests
//=============================================================================

TEST(NvmOpcodeTest, OpcodeValues) {
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::Flush), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::Write), 0x01);
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::Read), 0x02);
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::WriteUncorrectable), 0x04);
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::Compare), 0x05);
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::WriteZeroes), 0x08);
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::DatasetManagement), 0x09);
    EXPECT_EQ(static_cast<uint8_t>(nvme::NvmOpcode::Verify), 0x0C);
}
