// ============================================
// 位置4：实际例子
// ============================================

// ========== 例子1：正常情况（无掉棒/失误）==========
/*
运动员：Thompson-Herah（Jamaica的第2棒）
接棒后执行位置4：

步骤1：找到 teamIndex = 0（Jamaica）

步骤2：检查 teamDisqualified[0] = false（正常）

步骤3：生成随机数 batonChance = 45.0（45%）

步骤4：判断
- 45.0 >= 20.0 → 正常情况
- 不执行任何特殊操作
- 继续比赛 ✅

结果：正常继续比赛
*/

// ========== 例子2：失误（Baton Fumble）==========
/*
运动员：Daniels（United States的第2棒）
接棒后执行位置4：

步骤1：找到 teamIndex = 1（United States）

步骤2：检查 teamDisqualified[1] = false（正常）

步骤3：生成随机数 batonChance = 12.0（12%）

步骤4：判断
- 5.0 <= 12.0 < 20.0 → 失误！
- 计算延迟：12.0 × 0.1 = 1.2秒
- 打印："*** BATON FUMBLE! Daniels (United States) fumbled the baton! Delay: 1.2 seconds."
- sleep(1200毫秒)
- 继续比赛 ✅

结果：延迟1.2秒后继续比赛
*/

// ========== 例子3：掉棒（Baton Drop）==========
/*
运动员：Asher-Smith（Great Britain的第3棒）
接棒后执行位置4：

步骤1：找到 teamIndex = 2（Great Britain）

步骤2：检查 teamDisqualified[2] = false（正常）

步骤3：生成随机数 batonChance = 3.5（3.5%）

步骤4：判断
- 3.5 < 5.0 → 掉棒！
- teamDisqualified[2].store(true) → 标记队伍被取消
- 打印："*** BATON DROP! Asher-Smith (Great Britain) dropped the baton! Team Great Britain is DISQUALIFIED!"
- return（退出）❌

结果：队伍被取消资格，这个运动员退出，后续运动员也会退出
*/

// ========== 例子4：队伍已被取消资格 ==========
/*
运动员：Neita（Great Britain的第4棒）
但是！第3棒（Asher-Smith）之前掉棒了

接棒后执行位置4：

步骤1：找到 teamIndex = 2（Great Britain）

步骤2：检查 teamDisqualified[2] = true（已被取消！）
- 打印："*** Neita (Great Britain) - Team already disqualified, cannot continue."
- return（退出）❌

结果：不继续比赛，直接退出
*/

// ============================================
// 位置4的代码结构
// ============================================

void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    // ... 前面的代码 ...
    
    if ( pPrevA == NULL) {
        // 第一棒的代码（位置3）
    }
    else {  // ← 非第一棒
        // 等待接棒
        pPrevA->baton.wait(...);
        
        // 打印接棒信息
        thrd_print(...took the baton...);
        
        // ========== 位置4开始 ==========
        
        // 步骤1：找队伍索引
        int teamIndex = -1;
        for (int i = 0; i < NUM_TEAMS; ++i) {
            if (a.getTeamName() == astrTeams[i]) {
                teamIndex = i;
                break;
            }
        }
        
        // 步骤2：检查是否已被取消
        if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
            thrd_print("*** ... - Team already disqualified, cannot continue.\n");
            return;  // 退出
        }
        
        // 步骤3：生成随机数
        RandomTwister randGen_baton(0.0f, 100.0f);
        float batonChance = randGen_baton.generate();
        
        // 步骤4：判断结果
        if (batonChance < 5.0f) {
            // 掉棒
            teamDisqualified[teamIndex].store(true);
            thrd_print("*** BATON DROP! ...");
            return;  // 退出
        } 
        else if (batonChance < 20.0f) {
            // 失误
            float delaySeconds = batonChance * 0.1f;
            int delay_ms = static_cast<int>(delaySeconds * 1000.0f);
            thrd_print("*** BATON FUMBLE! ...");
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
        // 正常情况：什么都不做，继续
        
        // ========== 位置4结束 ==========
    }
    
    // 继续执行跑步代码...
}

// ============================================
// 延迟时间计算说明
// ============================================
/*
Task 3.1 要求：
- 如果百分比在 5%-20% 之间
- 延迟 = 百分比 × 10

例子：
- 百分比 = 5% → 延迟 = 5 × 10 = 50（但这是秒吗？不对）
- 百分比 = 5% → 延迟 = 5 × 0.1 = 0.5秒 ✅
- 百分比 = 20% → 延迟 = 20 × 0.1 = 2.0秒 ✅

代码中：
float delaySeconds = batonChance * 0.1f;
- batonChance 是 0-100 的数值
- 例如 batonChance = 12.0（表示12%）
- delaySeconds = 12.0 × 0.1 = 1.2秒
- 符合要求：5% = 0.5秒，20% = 2.0秒
*/
