// ============================================
// 位置5：结果显示 - 详细解释
// ============================================

/*
位置5的作用：
- 在比赛结束后，打印每个队伍的成绩
- 同时显示哪些队伍因为掉棒被取消资格
*/

// ========== 位置5在哪里？==========
int main() {
    // ... 创建线程、启动比赛、等待结束 ...
    
    // Join all threads
    for (int i = 0; i < NUM_TEAMS; ++i) {
        for (int j = 0; j < NUM_MEMBERS; ++j) {
            if(thread_competitor[i][j].joinable()){
                thread_competitor[i][j].join();
            }
        }
    }
    
    // ========== 位置5的代码在这里 ==========
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
    // ========== 位置5的代码结束 ==========
}

// ============================================
// 修改前的代码（原来的）
// ============================================
/*
原来的代码：
for (int i = 0; i < NUM_TEAMS; ++i)  aTeams[i].printTimes();
std::cout << std::endl;
return 0;
*/

// ============================================
// 修改后的代码（位置5）
// ============================================
/*
修改后：
for (int i = 0; i < NUM_TEAMS; ++i) {
    aTeams[i].printTimes();  // 打印队伍成绩
    
    // 检查是否被取消资格
    if (teamDisqualified[i].load()) {
        std::cout << "  *** Team " << astrTeams[i] << " was DISQUALIFIED due to baton drop! ***" << std::endl;
    }
}
std::cout << std::endl;
return 0;
*/

// ============================================
// 代码解释
// ============================================

/*
步骤1：打印标题
----------------------------------------
std::cout << "\n\nTEAM RESULTS" << std::endl;
输出：
TEAM RESULTS
*/

/*
步骤2：遍历所有队伍
----------------------------------------
for (int i = 0; i < NUM_TEAMS; ++i) {
    // i = 0: Jamaica
    // i = 1: United States
    // i = 2: Great Britain
    // i = 3: Switzerland
}
*/

/*
步骤3：打印每个队伍的成绩
----------------------------------------
aTeams[i].printTimes();
输出示例：
Team Jamaica = 41.5 s
Team United States = 42.1 s
...
*/

/*
步骤4：检查并显示取消资格信息
----------------------------------------
if (teamDisqualified[i].load()) {
    std::cout << "  *** Team " << astrTeams[i] << " was DISQUALIFIED due to baton drop! ***" << std::endl;
}

如果 teamDisqualified[i] 是 true，打印取消资格信息
*/

/*
步骤5：添加换行
----------------------------------------
std::cout << std::endl;
在最后添加一个空行，让输出更美观
*/

// ============================================
// 输出示例
// ============================================

/*
情况1：所有队伍都正常完成
----------------------------------------
TEAM RESULTS
Team Jamaica = 41.5 s
Team United States = 42.1 s
Team Great Britain = 41.8 s
Team Switzerland = 42.3 s

（没有取消资格信息）
*/

/*
情况2：有队伍被取消资格
----------------------------------------
TEAM RESULTS
Team Jamaica = 41.5 s
Team United States = 0 s
  *** Team United States was DISQUALIFIED due to baton drop! ***
Team Great Britain = 41.8 s
Team Switzerland = 42.3 s

（United States被取消资格，显示取消信息）
*/

/*
情况3：多个队伍被取消资格
----------------------------------------
TEAM RESULTS
Team Jamaica = 41.5 s
Team United States = 0 s
  *** Team United States was DISQUALIFIED due to baton drop! ***
Team Great Britain = 0 s
  *** Team Great Britain was DISQUALIFIED due to baton drop! ***
Team Switzerland = 42.3 s

（两个队伍被取消资格）
*/
