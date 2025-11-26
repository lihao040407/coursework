#include <iostream>
#include <string>
int main() {
    std::string person = "Test";
    std::string team = "Team";
    float delaySeconds = 1.234f;
    std::cout << "*** BATON FUMBLE! " + person + " (" + team + ") fumbled the baton! Delay: " + std::to_string(delaySeconds) + " seconds.\n";
    return 0;
}
