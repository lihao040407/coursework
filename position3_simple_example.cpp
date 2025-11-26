// ============================================
// 位置3：用简单例子理解
// ============================================

/*
假设有4个队伍：
- 队伍0：Jamaica
- 队伍1：United States  
- 队伍2：Great Britain
- 队伍3：Switzerland

假设队伍1（United States）的第2棒掉棒了：
- teamDisqualified[1] = true
- 队伍1的第3棒、第4棒看到这个标志，都退出了

但是队伍1的第1棒（Oliver）可能还没开始，或者正在准备开始。
位置3的作用就是：让第1棒在开始前也检查一下，如果队伍被取消了，就不开始。
*/

// ========== 例子1：正常情况 ==========
/*
运动员：Williams（Jamaica的第一棒）
pPrevA == NULL（是第一棒）

执行位置3的代码：
1. 找到 teamIndex = 0（Jamaica是第0个队伍）
2. 检查 teamDisqualified[0] = false（没被取消）
3. 打印 "Williams started, "
4. 继续比赛 ✅
*/

// ========== 例子2：队伍被取消 ==========
/*
运动员：Oliver（United States的第一棒）
pPrevA == NULL（是第一棒）

但是！队伍1的第2棒之前掉棒了，所以：
- teamDisqualified[1] = true

执行位置3的代码：
1. 找到 teamIndex = 1（United States是第1个队伍）
2. 检查 teamDisqualified[1] = true（被取消了！）
3. 打印 "*** Oliver (United States) - Team already disqualified, cannot start."
4. return（退出，不比赛）❌
*/

// ========== 代码执行流程 ==========
void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    // ... 前面的代码 ...
    
    if ( pPrevA == NULL) {  // 如果是第一棒
        // ========== 位置3的代码开始 ==========
        
        // 步骤1：找到这个运动员属于哪个队伍
        int teamIndex = -1;
        for (int i = 0; i < NUM_TEAMS; ++i) {
            if (a.getTeamName() == astrTeams[i]) {
                teamIndex = i;  // 找到了！比如 Jamaica = 0
                break;
            }
        }
        
        // 步骤2：检查这个队伍是否被取消资格
        if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
            // 如果 teamDisqualified[teamIndex] 是 true，说明队伍被取消了
            thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot start.\n");
            return;  // 退出，不继续
        }
        
        // 步骤3：如果没被取消，正常开始
        thrd_print(a.getPerson() + " started, ");
        
        // ========== 位置3的代码结束 ==========
    }
    else {
        // 非第一棒的代码...
    }
}

// ============================================
// 关键点总结
// ============================================
/*
1. 位置3只针对第一棒（pPrevA == NULL）
2. 作用：在开始前检查队伍是否被取消资格
3. 如果被取消：打印信息，退出（return）
4. 如果正常：打印 "started"，继续比赛
5. 为什么要找 teamIndex？
   - 因为需要知道是哪个队伍，才能检查 teamDisqualified[teamIndex]
*/
