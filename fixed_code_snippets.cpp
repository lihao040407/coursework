// ============================================
// 需要修复的代码片段
// ============================================

// ========== 修复1：BATON FUMBLE 信息 ==========
// 你的代码（错误）：
thrd_print("*** BATON FUMBLE! ... Delay: X seconds.\n");

// 应该改为：
thrd_print("*** BATON FUMBLE! " + a.getPerson() + " (" + a.getTeamName() + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n");


// ========== 修复2：BATON DROP 信息 ==========
// 你的代码（错误）：
thrd_print("*** BATON DROP! ... is DISQUALIFIED!\n");

// 应该改为：
thrd_print("*** BATON DROP! " + a.getPerson() + " (" + a.getTeamName() + ") dropped the baton! Team " + a.getTeamName() + " is DISQUALIFIED!\n");


// ========== 修复3：取消资格信息 ==========
// 你的代码（错误）：
thrd_print("*** ... - Team already disqualified, cannot continue.\n");

// 应该改为：
thrd_print("*** " + a.getPerson() + " (" + a.getTeamName() + ") - Team already disqualified, cannot continue.\n");
