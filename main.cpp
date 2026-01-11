#include <iostream>

int main()
{
    std::cout << "Breakpoint test started" << std::endl;

    // Test variables for inspection
    int counter = 0;
    int sum = 0;

    // Loop to test stepping through code
    for (int i = 1; i <= 5; i++) {
        counter = i;           // Set breakpoint here to watch 'i' change
        sum += i;
        std::cout << "i = " << i << ", sum = " << sum << std::endl;
    }

    // Final result
    std::cout << "Final sum: " << sum << std::endl;
    std::cout << "Breakpoint test completed" << std::endl;

    return 0;
}
