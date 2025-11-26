void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    thrd_print(a.getPerson() + " ready, ");
    barrier_allthreads_started.arrive_and_wait();
    barrier_go.arrive_and_wait();

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
            return;
        }
        thrd_print(a.getPerson() + " started, ");
    }
    else { 
        { // Brackets to reduce mutex scope
            std::unique_lock<std::mutex> lock(pPrevA->mtx);
            pPrevA->baton.wait(lock, [pPrevA]{ return pPrevA->bFinished; });
        }
        thrd_print( a.getPerson() +" ("+ a.getTeamName() + ")" +" took the baton from " + pPrevA->getPerson() +" ("+pPrevA->getTeamName() + ")\n");

        // ✅ 位置4：所有代码必须在 else 块内部！
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
    }  // ✅ else 块在这里结束
    
    //Part 2.5 继续执行跑步代码（第一棒和非第一棒都会执行这里）
    float fSprintDuration_seconds = generator.generate();
    int sprint_ms = static_cast<int>(fSprintDuration_seconds * 1000.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(sprint_ms));

    a.setTime(fSprintDuration_seconds);
    thrd_print( "Leg "+ std::to_string(a.numBatonExchanges()) + ": "+a.getPerson() + " ran in " + std::to_string(fSprintDuration_seconds) + " seconds. ("+ a.getTeamName() + ")\n");
    if ( a.numBatonExchanges() == NUM_MEMBERS) {
        if (!winner.exchange(true)) {
            std::cout << "\n Team " << a.getTeamName() << " is the WINNER!" << std::endl;
        }
    }
}

// main 函数中的结果打印部分：
    std::cout << "\n\nTEAM RESULTS" << std::endl;
    for (int i = 0; i < NUM_TEAMS; ++i) {
        aTeams[i].printTimes();
        // Task 3: Display disqualification status
        if (teamDisqualified[i].load()) {
            std::cout << "  *** Team " << astrTeams[i] << " was DISQUALIFIED due to baton drop! ***" << std::endl;
        }
    }
    std::cout << std::endl;  // ✅ 添加这行
    return 0;
