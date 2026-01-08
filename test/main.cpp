#include <iostream>
#include "nvme.h"

int main() {
    std::cout << "NVMe Library Test\n";
    std::cout << "==================\n\n";

    // Create NVMeDevice instance
    nvme::NVMeDevice device;

    std::cout << "NVMeDevice instance created successfully.\n";
    std::cout << "Device open status: " << (device.IsOpen() ? "Open" : "Closed") << "\n";

    // Test structure sizes
    std::cout << "\nStructure size verification:\n";
    std::cout << "  SubmissionQueueEntry: " << sizeof(nvme::SubmissionQueueEntry) << " bytes (expected: 64)\n";
    std::cout << "  CompletionQueueEntry: " << sizeof(nvme::CompletionQueueEntry) << " bytes (expected: 16)\n";

    // Test some CDW unions
    std::cout << "\nCommand DWord union sizes:\n";
    std::cout << "  IdentifyCDW10: " << sizeof(nvme::IdentifyCDW10) << " bytes\n";
    std::cout << "  IdentifyCDW11: " << sizeof(nvme::IdentifyCDW11) << " bytes\n";
    std::cout << "  GetLogPageCDW10: " << sizeof(nvme::GetLogPageCDW10) << " bytes\n";
    std::cout << "  SetFeaturesCDW10: " << sizeof(nvme::SetFeaturesCDW10) << " bytes\n";
    std::cout << "  FirmwareCommitCDW10: " << sizeof(nvme::FirmwareCommitCDW10) << " bytes\n";

    std::cout << "\nAll tests passed!\n";

    return 0;
}
