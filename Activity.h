#ifndef ACTIVITY_H
#define ACTIVITY_H

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

#define N 6

class Activity {
    public:
        Activity();
        Activity(const string name, int min_time, int min_quality, int priority);
        void print_info();

        void set_real_time(int real_time);
        void set_quality(int quality);
        void set_total_worth(float total_worth);

        const string get_name();
        const int get_min_time();
        const int get_real_time();
        const int get_min_quality();
        const int get_quality();
        const int get_priority();
        const float get_total_worth();

    private:
        string name;
        int min_time;
        int real_time;
        int min_quality;
        int quality;
        int priority;
        float total_worth;  // out of 100
};

// Security functions
// For integer numbers
int get_valid_int(const string& prompt, int min_val, int max_val);
// For "SD" and "TO"
string get_valid_sd_to(const string& prompt);

// Independed rate function
float rate_day(Activity avtivities[], int num_activities);
// Save stats to a txt file (for human-read usage)
void save_txt(Activity activities[], int num_activities, float total_score);
// Save stats to a json file (for date editing usage)
void save_json(Activity activities[], int num_activities, float total_score);
// Function for drawing diagrams
void draw_chart(const string& title, const vector<string>& dates, const vector<float>& scores);
// Function for reading json and calling the charts
void show_statistics();
#endif