// ============================================
// Task 3 完整代码插入指南
// ============================================

// ========== 位置1：全局变量 ==========
// 在 std::atomic<bool> winner(false); 之后添加：
std::atomic<bool> winner(false);
//Task 3: Track which teams are disqualified due to baton drop
std::array<std::atomic<bool>, NUM_TEAMS> teamDisqualified;


// ========== 位置2：main函数初始化 ==========
// 在 RandomTwister randGen_sprint_time(10.0f, 12.0f); 之后添加：
RandomTwister randGen_sprint_time(10.0f, 12.0f);
// Task 3: Initialize teamDisqualified array
for (int i = 0; i < NUM_TEAMS; ++i) {
    teamDisqualified[i].store(false);
}


// ========== 位置3：第一棒检查 ==========
// 修改 if ( pPrevA == NULL) 部分：
void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    // ... 前面的代码 ...
    
    if ( pPrevA == NULL) {
        // Task 3: Check if team is already disqualified (for first runner)
        int teamIndex = -1;
        for (int i = 0; i < NUM_TEAMS; ++i) {
            if (a.getTeamName() == astrTeams[i]) {
                teamIndex = i;
                break;
            }
        }
        if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
            thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot start.\n");
            return; // Exit thread, team is out
        }
        thrd_print(a.getPerson() + " started, ");
    }
    else {
        // ... 接棒代码 ...
        thrd_print( a.getPerson() +" ("+ a.getTeamName() + ")" +" took the baton from " + pPrevA->getPerson() +" ("+pPrevA->getTeamName() + ")\n");
        
        // ========== 位置4：接棒后检查（必须在 else 块内部）==========
        // Task 3: Check for baton fumble or drop (only for non-first runners)
        // Find team index by comparing team names
        int teamIndex = -1;
        for (int i = 0; i < NUM_TEAMS; ++i) {
            if (a.getTeamName() == astrTeams[i]) {
                teamIndex = i;
                break;
            }
        }
        
        // Check if team is already disqualified
        if (teamIndex >= 0 && teamDisqualified[teamIndex].load()) {
            thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot continue.\n");
            return; // Exit thread, team is out
        }
        
        // Task 3: Generate random percentage (0-100) to check for baton issues
        RandomTwister randGen_baton(0.0f, 100.0f);
        float batonChance = randGen_baton.generate();
        
        if (batonChance < 5.0f) {
            // Task 3.2: Baton Drop (0-5%) - Team disqualified
            if (teamIndex >= 0) {
                teamDisqualified[teamIndex].store(true);
                thrd_print("*** BATON DROP! " + a.getPerson() + " (" + a.getTeamName() + ") dropped the baton! Team " + a.getTeamName() + " is DISQUALIFIED!\n");
            }
            return; // Exit thread, team is out
        } else if (batonChance < 20.0f) {
            // Task 3.1: Baton Fumble (5-20%) - Delay = percentage * 10
            float delaySeconds = batonChance * 0.1f; // percentage * 10 / 100 = percentage * 0.1
            int delay_ms = static_cast<int>(delaySeconds * 1000.0f);
            thrd_print("*** BATON FUMBLE! " + a.getPerson() + " (" + a.getTeamName() + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
    }  // else 块结束
    
    // ... 继续执行跑步代码 ...
}


// ========== 位置5：结果显示 ==========
// 在 main 函数中，修改结果打印部分：
// Print the results for each team
std::cout << "\n\nTEAM RESULTS" << std::endl;
for (int i = 0; i < NUM_TEAMS; ++i) {
    aTeams[i].printTimes();
    // Task 3: Display disqualification status
    if (teamDisqualified[i].load()) {
        std::cout << "  *** Team " << astrTeams[i] << " was DISQUALIFIED due to baton drop! ***" << std::endl;
    }
}
std::cout << std::endl;
return 0;
