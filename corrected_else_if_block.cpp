        // ========== 步骤4：根据随机数判断结果 ==========
        if (batonChance < 5.0f) {
            // 情况1：掉棒（0-5%概率）
            teamDisqualified[teamIndex].store(true);  // 标记队伍被取消
            thrd_print("*** BATON DROP! " + a.getPerson() + " (" + a.getTeamName() + ") dropped the baton! Team " + a.getTeamName() + " is DISQUALIFIED!\n");
            return;  // 退出，不比赛
        } 
        else if (batonChance < 20.0f) {
            // 情况2：失误（5-20%概率）
            float delaySeconds = batonChance * 0.1f;  // 计算延迟时间
            int delay_ms = static_cast<int>(delaySeconds * 1000.0f);
            // ✅ 修复：应该是 BATON FUMBLE，不是 BATON DROP，并且要显示延迟时间
            thrd_print("*** BATON FUMBLE! " + a.getPerson() + " (" + a.getTeamName() + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));  // 延迟
            // 延迟后继续比赛
        }
        // 情况3：正常（20%以上概率）- 什么都不做，继续比赛
