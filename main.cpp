#include "Activity.h"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <nlohmann/json.hpp>
using namespace std;
using json = nlohmann::json;

/*
    This program operates together with the files below:
    tracker.sh (path: /home/Vasilis/Desktop/Vasilis/Rating_Project)
    tracker.service (path: /home/Vasilis/.config/systemd/user)
    tracker.timer (path: /home/Vasilis/.config/systemd/user)
*/

int main(int argc, char **argv) {
    Activity activities[] = {
        Activity("Cybersecurity Studying", 60, 65, 1),
        Activity("University Studying", 30, 65, 2),
        Activity("Piano Studying", 30, 65, 3),
        Activity("Book Reading", 30, 65, 2),
        Activity("Greek Studying", 15, 65, 2),
        Activity("English Studying", 15, 65, 2)
    };
    
    int num_activities = sizeof(activities) / sizeof(activities[0]);
    float total_score;
    string see_stats;

    cout << "Good night!" << endl;
    cout << "Enter your today performance informations, based on your point of view, in order to rate the total value of your day:" << endl;

    for (int i = 0; i < num_activities; i++) {
        int temp_real_time = 0;
        int temp_quality = 0;

        cout << activities[i].get_name() << endl;
        temp_real_time = get_valid_int("Type the time (minutes): ", 0, 1440);
        temp_quality = get_valid_int("Type the quality (0-100): ", 0, 100);

        activities[i].set_real_time(temp_real_time);
        activities[i].set_quality(temp_quality);
    }

    cout << endl << "----- RESULTS -----" << endl;
    
    total_score = rate_day(activities, num_activities);
    cout << endl << "====================" << "Your total day score: " << fixed << setprecision(2) << total_score << "/100" << "====================";

    cout << endl << endl << "Based on the data you entered, below there are detailed informations for each activity:" << endl << endl;
    for (int i = 0; i < num_activities; i++) {
        activities[i].print_info();
    }

    save_txt(activities, num_activities, total_score);
    save_json(activities, num_activities, total_score);

    cout << endl << "Would you like to watch the progress of the last 2 weeks? (Y/n)" << endl;
    cin >> see_stats;

    if (see_stats == "Y" || see_stats == "y") {
        show_statistics();
    }

    return 0;
}