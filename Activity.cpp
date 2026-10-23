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

Activity::Activity() {
    name = "";
    min_time = 0;
    real_time = 0;
    min_quality = 0;
    quality = 0;
    priority = 0;
    total_worth = 0.0;
}

Activity::Activity(const string name, int min_time, int min_quality, int priority) {
    this->name = name;
    this->min_time = min_time;
    this->real_time = 0;
    this->min_quality = min_quality;
    this->quality = 0;
    this->priority = priority;
    this->total_worth = 0.0;
}

void Activity::print_info() {
    cout << "Detailed informations for " << name << endl;
    cout << "Total worth: " << total_worth << endl;
}


// Setters

void Activity::set_real_time(int real_time) {
    this->real_time = real_time;
}

void Activity::set_quality(int quality) {
    this->quality = quality;
}

void Activity::set_total_worth(float total_worth) {
    this->total_worth = total_worth;
}



// Getters

const string Activity::get_name() {
    return name;
}

const int Activity::get_min_time() {
    return min_time;
}

const int Activity::get_real_time() {
    return real_time;
}

const int Activity::get_min_quality() {
    return min_quality;
}

const int Activity::get_quality() {
    return quality;
}

const int Activity::get_priority() {
    return priority;
}

const float Activity::get_total_worth() {
    return total_worth;
}


// Security functions
// For integer numbers
int get_valid_int(const string& prompt, int min_val, int max_val) {
    int value;
    while (true) {
        cout << prompt;
        // cin >> value returns false if user enters letters instead of numbers
        if (cin >> value && value >= min_val && value <= max_val) {
            // Clear buffer from random characters (like "12abc")
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "[ERROR] Invalid input. Please enter a number between " 
                 << min_val << " and " << max_val << ".\n";
            cin.clear(); // Remove fail state of cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Empty buffer from trash
        }
    }
}

// For "SD" and "TO"
string get_valid_sd_to(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        // Convert to capital letters
        for (char &c : input) {
            c = toupper(static_cast<unsigned char>(c));
        }

        if (input == "SD" || input == "TO") {
            return input;
        } else {
            cout << "[ERROR] Please type exactly 'SD' or 'TO'.\n";
        }
    }
}



float rate_day(Activity activities[], int num_activities) {
    float day_score = 0.0;
    float max_possible_score = 0.0;
    float act_importance = 0.0; // Or priority
    string critical_thinking = "";
    float justify_management = 0.0;
    float completion_ratio = 0.0;
    float quality_ratio = 0.0;
    float total_ratio = 0.0;

    for (int i = 0; i < num_activities; i++) {
        act_importance = num_activities - activities[i].get_priority();  // Setting the worth of every activity based on the given priority
        max_possible_score += 1 * act_importance;   // If I managed to score the minimum time of the activity
    }


    for (int i = 0; i < num_activities; i++) {
        act_importance = num_activities - activities[i].get_priority();  // Setting the worth of every activity based on the given priority
        justify_management = 0.0;

        // Calculation of time completion for the activity
        completion_ratio = (float)activities[i].get_real_time() / activities[i].get_min_time();    // Indicator of the completion percentage of the activity

        // If activity did not got studied at all the quality is set automatically to 0
        if (activities[i].get_real_time() == 0) {
            quality_ratio = 0;
        }
        else {
            quality_ratio = (float)activities[i].get_quality() / activities[i].get_min_quality();   // How good was the quality of the avtivity's studying
        }

        // Calculation of total value for the activity
        total_ratio = completion_ratio *quality_ratio;

        // Set total_worth of the activity as a score out of 100
        activities[i].set_total_worth(((total_ratio * act_importance)/max_possible_score)*100.0f);

        // Checking for bad time and quality scores
        bool missed_time = activities[i].get_real_time() < activities[i].get_min_time();
        bool missed_quality = activities[i].get_quality() < activities[i].get_min_quality();

        if (missed_time || missed_quality) {
            if (missed_time) {
                // If the quantity was not enough just highlight that more work is needed (there is NO excuse)
                cout << "*** You did not work enough TIME in: " << activities[i].get_name() << endl;
            }

            if (missed_quality) {
                // If the quality was not enough just highlight that more work is needed (there is NO excuse)
                cout << "*** You did not work with enough QUALITY in: " << activities[i].get_name() << endl;
            }
            
            // If however the quality was that good to save the score for this activity, ask user if he oversight the quantity of the activity on purpose
            if ((activities[i].get_real_time() < activities[i].get_min_time()) && (activities[i].get_quality() > activities[i].get_min_quality())) {
                critical_thinking = get_valid_sd_to("I want you to consider seriously if it was a strategic decision (SD) or just a tragic oversight... (TO) (" + activities[i].get_name() + "): ");

                for (char &c : critical_thinking) {
                    c = toupper(static_cast<unsigned char>(c));
                }

                if (critical_thinking == "SD") {
                    justify_management = 0.10f * max_possible_score;
                    cout << "You did the right thing as at least the quality was on point!" << endl;
                    cout << "Just keep in mind that you should prioritize " << activities[i].get_name() << " more in the future." << endl;
                    // Setting total_worth as (total_ratio + justify_management)
                    activities[i].set_total_worth((((total_ratio * act_importance)+justify_management)/max_possible_score)*100.0f);
                }
            }
        }


        if (total_ratio > 2) {
            cout << "You must know that for your today score " << activities[i].get_name() << " played undeniably a great role." << endl;
        }

        day_score += total_ratio * act_importance + justify_management;
    }
    

    return (day_score / max_possible_score) * 100.0f;
}


void save_txt(Activity activities[], int num_activities, float total_score){
    // Get today date
    time_t t = time(0);
    struct tm *now = localtime(&t);
    char date_buffer[80];
    // Date type is of DD-MM-YYYY (ex. 21-10-2026)
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", now);

    // Open file for appending more info
    // Use of ios::app so previous statistics do not get deleted
    ofstream outfile("performance.txt", ios::app);

    if (outfile.is_open()) {
        // Write data in file
        outfile << "========================================\n";
        outfile << "Date: " << date_buffer << "\n";
        outfile << "Total Day Score: " << fixed << setprecision(2) << total_score << " / 100\n";
        outfile << "----------------------------------------\n";
        outfile << "Activity Details:\n";
        
        for (int i = 0; i < num_activities; i++) {
            outfile << "- " << activities[i].get_name() << "\n";
            outfile << "  Quantity: " << activities[i].get_real_time() << " minutes (Goal: " << activities[i].get_min_time() << ")\n";
            outfile << "  Quality:  " << activities[i].get_quality() << " / 100 (Goal: " << activities[i].get_min_quality() << ")\n";
            outfile << "  Worth:    " << activities[i].get_total_worth() << "\n";
        }
        outfile << "========================================\n\n";

        // Close file
        outfile.close();
        cout << "\n[SUCCESS] Your daily report has been saved to 'performance.txt' file." << endl;
    } 
    else {
        cout << "\n[ERROR] Unable to open the file for saving." << endl;
    }
}


void save_json(Activity activities[], int num_activities, float total_score) {
    // Get today date
    time_t t = time(0);
    struct tm *now = localtime(&t);
    char date_buffer[80];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", now);

    // Load current JSON file (if exists)
    json all_days_history;
    ifstream infile("performance.json");
    
    if (infile.is_open()) {
        try {
            // Read data
            infile >> all_days_history; 
        } catch (json::parse_error& e) {
            // If file is empty or broken, initialize new empty array
            cout << "\n[WARNING] 'performance.json' is corrupted or empty! Starting fresh to prevent crash.\n";
            cout << "[DEBUG] Error: " << e.what() << "\n"; // Αυτό θα σου πει ΠΟΥ ακριβώς έκανες το λάθος!
            all_days_history = json::array();
        }
        infile.close();
    } else {
        // If there is no file (first execution)
        all_days_history = json::array();
    }

    // Create JSON object for TODAY date
    json today_entry;
    today_entry["date"] = date_buffer;
    today_entry["total_day_score"] = total_score;
    today_entry["activities"] = json::array(); // Initialize new array for activities

    // Enter data for every activity
    for (int i = 0; i < num_activities; i++) {
        json act_obj;
        act_obj["name"] = activities[i].get_name();
        act_obj["real_time"] = activities[i].get_real_time();
        act_obj["min_time"] = activities[i].get_min_time();
        act_obj["quality"] = activities[i].get_quality();
        act_obj["min_quality"] = activities[i].get_min_quality();
        act_obj["total_worth"] = activities[i].get_total_worth();
        act_obj["priority"] = activities[i].get_priority();

        // Add activity in the array for today date
        today_entry["activities"].push_back(act_obj);
    }

    // Check if today info is already in the total history
    bool day_found = false;
    for (auto& day : all_days_history) {
        if (day["date"] == date_buffer) {
            day = today_entry; // Αντικατάσταση της παλιάς εγγραφής με τη νέα
            day_found = true;
            break;
        }
    }

    // If not, add today info at the end
    if (!day_found) {
        all_days_history.push_back(today_entry);
    }

    // Write to the file (overwrite is default option)
    ofstream outfile("performance.json");
    if (outfile.is_open()) {
        outfile << all_days_history.dump(4);
        outfile.close();
        cout << "[SUCCESS] Your daily report has been saved to 'performance.json'." << endl;
    } else {
        cout << "[ERROR] Unable to open the JSON file for saving." << endl;
    }
}


void draw_chart(const string& title, const vector<string>& dates, const vector<float>& scores) {
    // Define ANSI color codes for terminal output
    const string RESET  = "\033[0m";
    const string RED    = "\033[31m";
    const string GREEN  = "\033[32m";
    const string YELLOW = "\033[33m";
    const string GRAY   = "\033[90m"; // Dark gray for the empty part of the bar

    cout << "\n==========================================================\n";
    cout << "  " << title << "\n";
    cout << "==========================================================\n";
    
    float max_val = 0.1f;
    for (float v : scores) { 
        if (v > max_val) max_val = v; 
    }
    
    int MAX_BAR_LENGTH = 35; // Slightly smaller for better terminal spacing
    
    for (size_t i = 0; i < dates.size(); i++) {
        cout << setw(12) << left << dates[i] << " | ";
        
        float percentage = scores[i] / max_val;
        int blocks = percentage * MAX_BAR_LENGTH;
        
        // Select color based on performance (relative to max value)
        string bar_color;
        if (percentage >= 0.8f) bar_color = GREEN;
        else if (percentage >= 0.5f) bar_color = YELLOW;
        else bar_color = RED;
        
        // Print the filled part with color
        cout << bar_color;
        for(int b = 0; b < blocks; b++) {
            cout << "▰"; // Alternative: "■"
        }
        
        // Print the empty part with dark gray
        cout << GRAY;
        for(int b = blocks; b < MAX_BAR_LENGTH; b++) {
            cout << "▱"; // Alternative: "·" or "-"
        }
        
        // Reset color and print the final score
        cout << RESET << " | " << fixed << setprecision(1) << scores[i] << "\n";
    }
    cout << "----------------------------------------------------------\n";
}

void show_statistics() {
    ifstream infile("performance.json");
    if (!infile.is_open()) {
        cout << "\n[INFO] No data found yet to display charts.\n";
        return;
    }
    
    json history;
    try {
        infile >> history;
    } catch (json::parse_error& e) {
        cout << "\n[ERROR] JSON file is corrupted or empty.\n";
        return;
    }
    
    // Define the time's depth in days that user will see the progress
    int total_days = history.size();
    int days_to_show = min(14, total_days); 
    
    vector<string> dates;
    vector<float> total_scores;
    map<string, bool> activity_names; // Find the names of all activities
    
    // Extract the total score of the days
    for (int i = total_days - days_to_show; i < total_days; i++) {
        auto day = history[i];
        dates.push_back(day["date"]);
        total_scores.push_back(day["total_day_score"]);
        
        for (auto& act : day["activities"]) {
            activity_names[act["name"]] = true;
        }
    }
    
    // Print 1st chart: Days progress
    draw_chart("TOTAL DAY SCORE EVOLUTION (Last " + to_string(days_to_show) + " days)", dates, total_scores);
    
    // Print the other 6 charts: Activities progress during the particular period
    for (auto const& [name, _] : activity_names) {
        vector<float> act_scores;
        
        // Searching for score of the activity for every day
        for (int i = total_days - days_to_show; i < total_days; i++) {
            float val = 0.0f;
            for (auto& act : history[i]["activities"]) {
                if (act["name"] == name) {
                    val = act["total_worth"];
                    break;
                }
            }
            act_scores.push_back(val);
        }
        
        // Design chart for the particular activity
        draw_chart("EVOLUTION: " + name, dates, act_scores);
    }
}