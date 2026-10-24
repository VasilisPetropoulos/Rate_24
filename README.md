# 🚀 Daily Productivity Tracker

A robust, terminal-based C++ productivity tool designed for absolute accountability. It doesn't just track your time; it evaluates the *quality* of your work, scales it by priority, and visualizes your progress directly in the terminal using ANSI-colored charts.

## ✨ Core Philosophy
Most habit trackers just ask "Did you do it?". This tracker operates on a deeper level:
* **Quality meets Quantity:** Logging 2 hours of distracted work yields a lower score than 45 minutes of absolute focus.
* **The "SD vs TO" Mechanic:** Life happens. If you miss a target but the quality was exceptional, the app asks you to justify it. Was it a **Strategic Decision (SD)** to shift priorities, or a **Tragic Oversight (TO)**? Justified decisions grant a strategic bonus, keeping the system realistic and sustainable.

## 🛠️ Features
- **Bulletproof CLI:** Built-in input validation prevents crashes from typos or invalid data.
- **Weighted Scoring Algorithm:** Activities are scored dynamically based on user-defined priorities.
- **Dual Data Persistence:**
  - `performance.txt`: A clean, human-readable daily log.
  - `performance.json`: A structured database for historical tracking.
- **TUI Analytics:** Generates dynamic, colored UTF-8 bar charts in the terminal to visualize your evolution over the last 14 days.
- **Linux Automation:** Includes systemd timers and bash scripts to automatically pop up the evaluation terminal every night at 23:00.

## 🚀 Getting Started

### Prerequisites
- A C++ compiler (`g++` or `clang`)
- `nlohmann/json` library for C++
- *Optional (for automation):* Linux with `systemd` and a terminal emulator.

### Compilation
Clone the repository and compile the source code:
```bash

git clone https://github.com/VasilisPetropoulos/Rate_24.git
cd Rate_24
g++ *.cpp -o Rate_24

```

### Usage
Run the program manually:
```bash
./Rate_24
```
The program will guide you through your daily activities, evaluate your score out of 100, save the data locally, and offer to display your performance charts.

## ⚙️ Setting up Automated Daily Prompts (Linux)
Never forget to log your day. You can set the tracker to automatically open a terminal window every night.

1. Navigate to the `linux_automation/` directory.
2. Edit `tracker.sh` and `tracker.service` to match your exact file paths and preferred terminal emulator (e.g., Alacritty, Kitty, GNOME Terminal).
3. Enable the systemd timer:
```bash
mkdir -p ~/.config/systemd/user/
cp tracker.service tracker.timer ~/.config/systemd/user/
systemctl --user daemon-reload
systemctl --user enable --now tracker.timer
```

## 🤝 Contributing
Feel free to fork this project, open issues, or submit Pull Requests. Future plans include migrating the core logic to a Qt/QML Graphical User Interface.
