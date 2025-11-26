// Rename your file to be   StudentID_FamilyName_vVersion.cpp
// And put that information in the file header
//TODO  StudentID = 11409165
//TODO  FamilyName = HAO

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <random> // For std::uniform_real_distribution, std::mt19937, and std::random_device
#include <condition_variable>
#include <array>
//#include <barrier> // Available in C++ >11 
#include "barrier.hpp" // Our version of barrier if you are using C++11
#include "cs_helper_DoNotModify.hpp"
#include<atomic>
using namespace std;
// global variable
std::mutex print_mtx;  //create a global variable for print_mutex
const int NUM_TEAMS = 4;     // number of teams in the race
const int NUM_MEMBERS = 4;    // number of athletes in the team

// Data for team/athelete initialisation. The Women’s 4x100 meter relay at the Tokyo 2020 Olympics. The teams took between 41 and 42 seconds.
std::array< string, 4> astrTeams = { "Jamaica", "United States", "Great Britain", "Switzerland" };
std::array< std::array<std::string, 4>, 4> aastrCompetitors = {{
    { "Williams", "Thompson-Herah", "Fraser-Pryce", "Jackson" },
    { "Oliver", "Daniels", "Prandini", "Thomas" },
    { "Philip", "Lansiquot", "Asher-Smith", "Neita" },
    { "Del-Ponte", "Kambundji", "Kora", "Dietsche" }
}};


class RandomTwister{
public:
    RandomTwister(float min, float max) : distribution(min, max) {} // Initialises uniform_real_distribution to initialize to a specific range of numbers
    float generate()  // Returns a random float within the specified range
    {
        //Part 1.1 Make the Random number generator thread-safe by adding a simple std::mutex, and rembember the unlock!  Instantiate the mutex into private: area of this class, below
        std::lock_guard<std::mutex> lock(mtx_);
        return distribution(engine);
    }
private:
    // std::random_device creates a seed value for the mt19937 instance creation. It creates a seed value for the “mt” random number generator
    std::mt19937 engine{std::random_device{}()};           // Mersenne Twister random number generator engine, with a seed from random_device() - static
    std::uniform_real_distribution<float> distribution;    // This uniform_real_distribution transforms the engine output into the required (min, max) range and data type.
    std::mutex mtx_;
};


//Part 1.2 Make thrd_print thread safe.  Instantiate a mutex here (global as it is shared between threads) and use it to protect the function using a std::lock_guard<std::mutex>
void thrd_print(const std::string& str) {  // Thread safe print
    std::lock_guard<std::mutex> lock(print_mtx);
    cout << str;
}

barrier barrier_allthreads_started(1+(NUM_TEAMS * NUM_MEMBERS)); // Need all the thread to reach here before the start can continue.
//Part 1.3 Create another barrier array and name it "barrier_go" which you will use to make all threads wait until the race official starts the race
barrier barrier_go(1+(NUM_TEAMS *NUM_MEMBERS));
//Part 2.1  Create a std::atomic variable of type bool, initalised to false and name it "winner". You will use it to ensure just the winning thread claims to have won the race.
std::atomic<bool> winner(false);
//Task 3: Track which teams are disqualified due to baton drop
std::array<std::atomic<bool>, NUM_TEAMS> teamDisqualified;

void thd_runner_16x100m(Competitor& a, RandomTwister& generator) {
    thrd_print(a.getPerson() + " ready, ");
    // Wait for all *17* threads to ensure that all threads have started (all athletes are on their starting blocks!)
    //Part 1.4 Apply the barrier_allthreads_started using arrive_and_wait().
                // Wait at the barrier until all threads are running (including the main thread this is 16+1)
    // Wait for the starter gun to fire (all threads will be waiting while the random countdown for the starting pistol). Then the main thread also waits making 17 threads and this allows them all to started running
    barrier_allthreads_started.arrive_and_wait();
    //Part 1.5 Apply the barrier_go here
    barrier_go.arrive_and_wait();
    thrd_print(a.getPerson() + " started, ");     // This is an individual race, all the competitors are starting at the same time.
    float fSprintDuration_seconds = generator.generate(); // Thread sleep for the random time period (between 10 s and 12 s.
    //Part 1.6 Add a this_thread sleep_for to sleep for the SprintDuration. Convert the float seconds to milliseconds, and then to integer and then use std::chrono::milliseconds() to convert it to a time unit for sleep_for
    

     int sprint_ms = static_cast<int>(fSprintDuration_seconds * 1000.0f); //change float to int
     
     std::this_thread::sleep_for(std::chrono::milliseconds(sprint_ms));

    a.setTime(fSprintDuration_seconds); // Update the competitor and team information
    thrd_print( a.getPerson() + " took " + std::to_string(fSprintDuration_seconds) + " seconds. ("+ a.getTeamName() + ")\n");
}

void thd_runner_4x4x100m(Competitor& a, Competitor *pPrevA, RandomTwister& generator) {
    thrd_print(a.getPerson() + " ready, ");
    //Part 2.2 Copy the code from thd_runner_16x100m for
    // barrier_allthreads_started
    // barrier_go
    barrier_allthreads_started.arrive_and_wait();
    barrier_go.arrive_and_wait();
    // If the competitor does not have a pointer to a previous competitor, then it must be the first runner of that team.
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
    else { // If they are not the first runner in that team, then they need to wait for the previous runner to give them the baton.
        { // Brackets to reduce mutex scope
            //Part 2.3 Create a std::unique_lock<std::mutex> called "lock", initialised with pPrevA->mtx mutex
            std::unique_lock<std::mutex> lock(pPrevA->mtx);
            //Part 2.4 Complete the pPrevA->baton condition_variable line below to wait on that lock. (use the pPrevA->bFinished as the check function. It is tricky to get the lambda right!)
            // pPrevA->baton.wait(lock, ... Complete this bit ... }); // Wait for the baton to arrive.
            pPrevA->baton.wait(lock, [pPrevA]{ return pPrevA->bFinished; });
        }
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
    //Part 2.5 Copy the code from thd_runner_16x100m for fSprintDuration_seconds and std::this_thread::sleep_for
    float fSprintDuration_seconds = generator.generate();
    int sprint_ms = static_cast<int>(fSprintDuration_seconds * 1000.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(sprint_ms));

    a.setTime(fSprintDuration_seconds);
    thrd_print( "Leg "+ std::to_string(a.numBatonExchanges()) + ": "+a.getPerson() + " ran in " + std::to_string(fSprintDuration_seconds) + " seconds. ("+ a.getTeamName() + ")\n");
    if ( a.numBatonExchanges() == NUM_MEMBERS) // The last athlete in the team has crossed the finish line (crossing the line counts as a baton exchage)
    {
        // Print "finished" only if this is the first thread to complete
        //Part 2.6 Use an atomic .exchange on the atomic "winner" object that you defined at the top of this code and use this in the line below
        // if (!winner. ...) // Uncomment this line
         if (!winner.exchange(true))
        {
            std::cout << "\n Team " << a.getTeamName() << " is the WINNER!" << std::endl;
        }
    }
}



int main() {
    thread     thread_competitor[NUM_TEAMS][NUM_MEMBERS];  // 2D array of threads.
    Team aTeams[NUM_TEAMS]; // Can be global
    Competitor athlete[NUM_TEAMS][NUM_MEMBERS];  // 2D array of Competitors.
    float afTeamTime_s[NUM_TEAMS];
    // Part 1.7   Change the random number generation to between 10 s and 12 s.  (you might want to do this later so you don't have to wait while you are debugging!)
    RandomTwister randGen_sprint_time(10.0f, 12.0f);
    // Task 3: Initialize teamDisqualified array
    for (int i = 0; i < NUM_TEAMS; ++i) {
        teamDisqualified[i].store(false);
    }
       std::cout << "Re-run of the women’s 4x100 meter relay at the Tokyo 2020 Olympics.\n" << std::endl;
    // Start threads in each position of the 2D array
    for (int i = 0; i < NUM_TEAMS; ++i) {
        //string strTeam = astrTeams[i];
        afTeamTime_s[i] = 0;
        aTeams[i].setTeam(astrTeams[i]);
        for (int j = 0; j < NUM_MEMBERS; ++j) {
            athlete[i][j].set(aastrCompetitors[i][j], &(aTeams[i]));  // Create the athlete information
            // Create threads with the competitor information and a reference to the standard number randGen_sprint_time
            // The next line does not use threads, it just runs the athletes one after another - delete it when you are ready to start working on Part 1
            // thd_runner_16x100m( std::ref(athlete[i][j]), std::ref(randGen_sprint_time) );
            // THIS IS FOR PART 1  //Part 1.8 Remove the sequential call line above, with the multithreaded one below
            // thread_competitor[i][j] = std::thread(thd_runner_16x100m, std::ref(athlete[i][j]), std::ref(randGen_sprint_time) );  // Run the atheletes all together (16x100m)

            // THIS IS FOR PART 2
            //Part 2.7 Start the thd_runner_4x4x100m instead. If it is the first runner of the team (j==0) then Competitor *pPrevA should be NULL, otherwise it should be the previous runner (Competitor *)&(athlete[i][j-1])
            if (j == 0) {
                // First runner in team – no previous runner
                thread_competitor[i][j] = std::thread(
                    thd_runner_4x4x100m,
                    std::ref(athlete[i][j]),
                    (Competitor*)nullptr,
                    std::ref(randGen_sprint_time)
                );
            } else {
                // Passed baton – previous runner is athlete[i][j-1]
                thread_competitor[i][j] = std::thread(
                    thd_runner_4x4x100m,
                    std::ref(athlete[i][j]),
                    &(athlete[i][j - 1]),
                    std::ref(randGen_sprint_time)
                );
            }
        }
    }

    // Wait for all threads to be running including the main thread
    //Part 1.9  Apply the barrier_allthreads_started arrive_and_wait here to wait for all threads to be created (16 threads + this main thread = 17)
    // Wait at the barrier until all threads arrive
    barrier_allthreads_started.arrive_and_wait();

    thrd_print("\n\nThe race official raises her starting pistol...\n");
    //Part 2.8 Change this starter gun time from the fixed 3.5 seconds (next line) to a random value between 3 to 5 seconds.
    // Note: Using 1-3 seconds for faster testing/debugging (change to 3.0f, 5.0f for assignment requirement)
    RandomTwister randGen_starter(1.0f, 3.0f);  // Starter gun random time (1-3s for testing, should be 3-5s per Part 2.8)
    float fStarterGun_s = randGen_starter.generate();

    int starter_ms = static_cast<int>(fStarterGun_s * 1000.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(starter_ms));
   

    // 所有人在第二个 barrier 等 GO
    barrier_go.arrive_and_wait();

    thrd_print("\nGO !\n\n");


    //Part 1.10 Sleep using std::this_thread::sleep_for function for the fStarterGun_s  (see advice on type conversion given above)
   // this_thread::sleep_for(std::chrono::seconds(2));
    //Part 1.11  Apply the final barrier_go arrive_and_wait here to start all the competitors running.
                        // Wait at the barrier until all threads arrive
  //  barrier_go.arrive_and_wait();
    //thrd_print("\nGO !\n\n");
    // Join all threads
    for (int i = 0; i < NUM_TEAMS; ++i) {
        for (int j = 0; j < NUM_MEMBERS; ++j) {
            //Part 1.12  For all thread_competitor[i][j], test the thread is joinable, and if so, join it
            if(thread_competitor[i][j].joinable()){
                thread_competitor[i][j].join();
            }
        }
    }
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
}
