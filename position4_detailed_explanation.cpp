// ============================================
// 位置4：非第一棒接棒后检查 - 详细解释
// ============================================

/*
位置4的作用：
- 非第一棒运动员接棒后，检查是否发生掉棒或失误
- 如果掉棒（0-5%概率）：队伍被取消资格
- 如果失误（5-20%概率）：延迟一段时间后继续
- 如果正常（20%以上概率）：继续比赛
*/

// ========== 位置4在哪里？==========
void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    // ... 前面的代码 ...
    
    if ( pPrevA == NULL) {
        // 第一棒的代码（位置3）
        // ...
    }
    else {  // ← 非第一棒进入这里
        // 等待接棒
        pPrevA->baton.wait(...);
        
        // 打印接棒信息
        thrd_print(...took the baton...);
        
        // ========== 位置4的代码从这里开始 ==========
        // 必须在 else 块内部！
        // ========== 位置4的代码到这里结束 ==========
    }
}

// ============================================
// 位置4的完整代码
// ============================================

        // Task 3: Check for baton fumble or drop (only for non-first runners)
        // 步骤1：找到队伍索引
        int teamIndex = -1;
        for (int i = 0; i < NUM_TEAMS; ++i) {
            if (a.getTeamName() == astrTeams[i]) {
                teamIndex = i;
                break;
            }
        }
        
        // 步骤2：检查队伍是否已经被取消资格
        if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
            thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot continue.\n");
            return; // Exit thread, team is out
        }
        
        // 步骤3：生成随机数，判断是否掉棒/失误
        RandomTwister randGen_baton(0.0f, 100.0f);
        float batonChance = randGen_baton.generate();  // 生成 0-100 的随机数
        
        // 步骤4：根据随机数判断结果
        if (batonChance < 5.0f) {
            // 情况1：掉棒（0-5%概率）
            if (teamIndex >= 0) {
                teamDisqualified[teamIndex].store(true);  // 标记队伍被取消资格
                thrd_print("*** BATON DROP! " + a.getPerson() + " (" + a.getTeamName() + ") dropped the baton! Team " + a.getTeamName() + " is DISQUALIFIED!\n");
            }
            return; // Exit thread, team is out
        } else if (batonChance < 20.0f) {
            // 情况2：失误（5-20%概率）
            float delaySeconds = batonChance * 0.1f;  // 延迟 = 百分比 × 0.1
            int delay_ms = static_cast<int>(delaySeconds * 1000.0f);
            thrd_print("*** BATON FUMBLE! " + a.getPerson() + " (" + a.getTeamName() + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));  // 延迟
        }
        // 情况3：正常（20%以上概率）- 什么都不做，继续比赛

// ============================================
// 详细步骤解释
// ============================================

/*
步骤1：找到队伍索引
----------------------------------------
和位置3一样，需要找到这个运动员属于哪个队伍
因为需要知道检查 teamDisqualified[哪个索引]

例子：
- 如果运动员是 "Thompson-Herah" (Jamaica)
- 找到 teamIndex = 0
*/

/*
步骤2：检查队伍是否已经被取消资格
----------------------------------------
如果之前有人掉棒了，队伍就被取消了
这时候应该立即退出，不继续比赛

例子：
- 如果第2棒掉棒了 → teamDisqualified[0] = true
- 第3棒接棒后，检查发现是 true
- 打印 "Team already disqualified, cannot continue"
- return（退出，不比赛）
*/

/*
步骤3：生成随机数
----------------------------------------
生成一个 0-100 的随机数，用来判断是否掉棒/失误

例子：
- batonChance = 3.5  → 3.5%概率，会掉棒
- batonChance = 12.0 → 12%概率，会失误
- batonChance = 45.0 → 45%概率，正常
*/

/*
步骤4：根据随机数判断
----------------------------------------
- 0-5%：掉棒 → 队伍取消资格，退出
- 5-20%：失误 → 延迟后继续
- 20%以上：正常 → 继续比赛
*/
