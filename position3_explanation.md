# 位置3详细解释

## 步骤1：找到队伍索引
```cpp
int teamIndex = -1;  // 初始化索引为-1（表示还没找到）
for (int i = 0; i < NUM_TEAMS; ++i) {
    if (a.getTeamName() == astrTeams[i]) {  // 比较队伍名称
        teamIndex = i;  // 找到了，保存索引
        break;  // 退出循环
    }
}
```
**作用**：通过比较队伍名称，找到这个运动员属于哪个队伍（0, 1, 2, 或 3）

**例子**：
- 如果 `a.getTeamName()` 是 "Jamaica"，那么 `teamIndex = 0`
- 如果 `a.getTeamName()` 是 "United States"，那么 `teamIndex = 1`

---

## 步骤2：检查队伍是否被取消资格
```cpp
if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
    thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot start.\n");
    return; // Exit thread, team is out
}
```
**作用**：
- `teamIndex >= 0`：确保找到了队伍索引（如果没找到，teamIndex 还是 -1）
- `teamDisqualified[teamIndex].load()`：读取这个队伍是否被取消资格
- 如果被取消资格：打印信息，然后 `return` 退出线程（不继续比赛）

---

## 步骤3：如果没被取消，正常开始
```cpp
thrd_print(a.getPerson() + " started, ");
```
**作用**：如果队伍没被取消资格，正常打印 "started" 并继续比赛

---

## 完整流程图

```
第一棒运动员准备开始
    ↓
找到队伍索引（teamIndex）
    ↓
检查 teamDisqualified[teamIndex] 是否为 true？
    ↓
    ├─ 是 → 打印"已取消资格" → return（退出，不比赛）
    └─ 否 → 打印"started" → 继续比赛
```

---

## 实际例子

**场景1：队伍正常**
```
Williams (Jamaica) ready,
找到 teamIndex = 0
检查 teamDisqualified[0] = false
打印：Williams started,
继续比赛
```

**场景2：队伍已被取消资格**
```
Williams (Jamaica) ready,
找到 teamIndex = 0
检查 teamDisqualified[0] = true（之前有人掉棒了）
打印：*** Williams (Jamaica) - Team already disqualified, cannot start.
return（退出，不比赛）
```
