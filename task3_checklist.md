# Task 3 要求检查清单

## ✅ 位置1：全局变量声明
```cpp
std::array<std::atomic<bool>, NUM_TEAMS> teamDisqualified;
```
✅ 正确

## ✅ 位置2：初始化数组
```cpp
for (int i = 0; i < NUM_TEAMS; ++i) {
    teamDisqualified[i].store(false);
}
```
✅ 正确

## ✅ 位置3：第一棒检查
```cpp
if ( pPrevA == NULL) {
    // 检查取消资格
    // ...
    thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot start.\n");
    return;
}
```
✅ 正确

## ✅ 位置4：接棒后检查

### 4.1 检查是否已被取消资格
```cpp
if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
    thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot continue.\n");
    return;
}
```
✅ 正确

### 4.2 BATON DROP (0-5%)
```cpp
if (batonChance < 5.0f) {
    teamDisqualified[teamIndex].store(true);
    thrd_print("*** BATON DROP! " + a.getPerson() + " (" + a.getTeamName() + ") dropped the baton! Team " + a.getTeamName() + " is DISQUALIFIED!\n");
    return;
}
```
✅ 正确

### 4.3 BATON FUMBLE (5-20%)
```cpp
else if (batonChance < 20.0f) {
    float delaySeconds = batonChance * 0.1f;
    int delay_ms = static_cast<int>(delaySeconds * 1000.0f);
    thrd_print("*** BATON FUMBLE! " + a.getPerson() + " (" + a.getTeamName() + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}
```
✅ 正确！现在包含了完整的运动员信息和延迟时间

## ✅ 位置5：结果显示
```cpp
for (int i = 0; i < NUM_TEAMS; ++i) {
    aTeams[i].printTimes();
    if (teamDisqualified[i].load()) {
        std::cout << "  *** Team " << astrTeams[i] << " was DISQUALIFIED due to baton drop! ***" << std::endl;
    }
}
```
✅ 正确

## Task 3 要求对照

| 要求 | 状态 | 说明 |
|------|------|------|
| 3.1 Baton Fumble (5-20%) | ✅ | 延迟计算正确，信息完整 |
| 3.2 Baton Drop (0-5%) | ✅ | 队伍取消资格，信息完整 |
| 3.3 清楚显示信息 | ✅ | 所有信息都完整显示 |
