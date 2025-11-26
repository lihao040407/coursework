#include <iostream>
#include <string>
#include <iomanip>

int main() {
    // 测试 std::to_string 是否正常工作
    float delaySeconds = 1.234f;
    std::string message = "*** BATON FUMBLE! Test (Team) fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n";
    std::cout << message;
    
    // 测试不同的延迟值
    float testValues[] = {5.0f, 12.5f, 19.9f};
    for (float val : testValues) {
        float delay = val * 0.1f;
        std::cout << "Test: " << val << "% -> Delay: " << std::to_string(delay) << " seconds\n";
    }
    
    return 0;
}
