        thrd_print( a.getPerson() +" ("+ a.getTeamName() + ")" +" took the baton from " + pPrevA->getPerson() +" ("+pPrevA->getTeamName() + ")\n");

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
    }
