// ============================================
// 位置3：第一棒检查 - 详细对比
// ============================================

// ========== 修改前（原来的代码）==========
void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    // ... 前面的代码 ...
    
    // 原来的代码：只有一行
    if ( pPrevA == NULL)  thrd_print(a.getPerson() + " started, ");
    
    else {
        // 非第一棒的代码
        // ...
    }
}

// ========== 修改后（位置3）==========
void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    // ... 前面的代码 ...
    
    // 修改后：变成代码块，添加了检查
    if ( pPrevA == NULL) {
        // ========== 步骤1：找到队伍索引 ==========
        int teamIndex = -1;  // 先设为-1，表示还没找到
        for (int i = 0; i < NUM_TEAMS; ++i) {
            // 遍历所有队伍名称，找到匹配的
            if (a.getTeamName() == astrTeams[i]) {
                teamIndex = i;  // 找到了！保存索引
                break;  // 找到了就退出循环
            }
        }
        
        // ========== 步骤2：检查是否被取消资格 ==========
        if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
            // teamIndex >= 0：确保找到了索引
            // teamDisqualified[teamIndex].load()：读取是否被取消资格
            // 如果两个条件都满足，说明队伍被取消了
            thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot start.\n");
            return;  // 退出函数，不继续比赛
        }
        
        // ========== 步骤3：如果没被取消，正常开始 ==========
        thrd_print(a.getPerson() + " started, ");
    }
    
    else {
        // 非第一棒的代码
        // ...
    }
}

// ============================================
// 为什么要这样做？
// ============================================
/*
问题：如果队伍之前有人掉棒了，第一棒不应该开始比赛

例子：
- 第2棒运动员掉棒了 → teamDisqualified[0] = true
- 第3棒、第4棒看到这个标志，都退出了
- 但是第1棒（如果还没开始）也应该检查，不应该开始

所以位置3的作用：
1. 第一棒在开始前检查队伍状态
2. 如果队伍已被取消，立即退出
3. 如果队伍正常，继续比赛
*/
