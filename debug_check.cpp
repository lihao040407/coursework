// 检查代码的关键部分

// 你的代码第150行应该是：
thrd_print("*** BATON FUMBLE! " + a.getPerson() + " (" + a.getTeamName() + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n");

// 如果还是显示 "X seconds" 或没有显示，可能的原因：

// 1. 检查是否在 else 块内部
// 2. 检查缩进是否正确
// 3. 确保代码在 else if (batonChance < 20.0f) 块内

// 完整的正确代码应该是：
else if (batonChance < 20.0f) {
    // Task 3.1: Baton Fumble (5-20%) - Delay = percentage * 10
    float delaySeconds = batonChance * 0.1f;
    int delay_ms = static_cast<int>(delaySeconds * 1000.0f);
    thrd_print("*** BATON FUMBLE! " + a.getPerson() + " (" + a.getTeamName() + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}
