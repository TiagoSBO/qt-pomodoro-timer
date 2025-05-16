# 🍎Pomodoro Timer Application

[![In Development](https://img.shields.io/badge/Status-In%20Development-yellow)](https://shields.io/)
[![Netlify Status](https://api.netlify.com/api/v1/badges/cc0be1bf-2eae-4e81-9ced-627ba616499e/deploy-status)](https://app.netlify.com/projects/qtpomodoro-timer/deploys)




## Description

Pomodoro Timer is a project based on the pomodoro technique developed by the author [Francesco Cirillo](https://www-pomodorotechnique-com.translate.goog/the-pomodoro-technique-book/?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc). The objective of this project was to design a system focused on increasing productivity and focus.

The application operates in the system tray, providing discreet access to timer controls and settings. Users can customize the length of work periods and breaks, set up sound alerts, and review their session history through detailed logs.

## Technologies used

<img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/cplusplus/cplusplus-original.svg" width="30" height="30"/> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/python/python-original.svg" width="30" height="30" /> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/qt/qt-original.svg" width="30" height="30"/> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/cmake/cmake-original.svg" width="30" height="30" /> <img src="https://cdn.jsdelivr.net/gh/devicons/devicon@latest/icons/netlify/netlify-original.svg" width="30" height="30" />
          
## Features

-   **🔗 System Tray Integration:** The application runs minimized in the system tray, keeping your workspace clean and organized.
       - Minimize to tray
       - Restore from tray
       - Quit from tray
       - Restore floating window
 
-   **⿻ Floating Window:** A movable floating window that displays the time of the timer. This floating window can be activated by clicking on the timer or by pressing the shortcut **Ctrl + Shift + 0**.

-   **⏲️ Customizable Timers:** Adjust the duration of your work sessions, short breaks, and long breaks according to your preferences.7
       - Number of Pomodoros completed
       - Focus time
       - Session end time
  
-   **⏰ Sound Alerts:** Configure custom sound notifications to alert you when a timer ends, ensuring you stay on schedule.
  
-   **⚙️ Configuration Settings:** Fine-tune various aspects of the application, from timer durations to sound preferences, ensuring a tailored experience.
### Comming soon:
 -   **🎨 Multiple Themes:** Multiple themes to change the look of the application.
 -   **📈 Session Logs:** Detailed logs of your Pomodoro sessions are maintained, allowing you to track your productivity over time.

### Demonstration
![funcionalidades (1)](https://github.com/user-attachments/assets/86bc6873-f589-42eb-8605-68def035ff2b)

**Floating window:**  It can be activated with the shortcut **Ctrl + shift + 0** or by clicking on the timer label.

![float2 (1)](https://github.com/user-attachments/assets/7dccbef9-e0c1-428b-8a94-9607cb49ee7d)

## Installation Guide
You can install the application either by using the provided installer or by building it from source.

### Using the Installer
- Go to the [Releases](https://github.com/TiagoSBO/qt-pomodoro-timer/releases).

- Download the latest version (*.exe file).

- Run the installer and follow the on-screen instructions.

### Building from Source
If you prefer to build the application from source, follow these steps:

**Clone the repository:**
```
git clone https://github.com/TiagoSBO/qt-pomodoro-timer.`
git cd qt-pomodoro-timer
```
**Install Dependencies:**
You will need:
- CMake
- Qt 6.x (or Qt 5.15+)

> Make sure to add qmake and cmake to your system PATH

On Windows (using Chocolatey):
```
bash choco install cmake qt6
```
On Linux
```
sudo apt install cmake qt6-base-dev qt6-tools-dev-tools build-essential
```
**Build the Application:**
Create a build directory and use CMake to configure and build the project.
```
mkdir build
cd build
cmake ..
cmake --build .
```
**Run the Application:**
After a successful build, you can run the application executable. The location of the executable will depend on your build configuration and operating system. It is typically found within the `build` directory or a subdirectory like `build/bin` or `build/debug`.
```
./qt-pomodoro-timer
```

## Contributing

Contributions are welcome! If you have suggestions, bug reports, or wish to contribute code, please feel free to open an issue or submit a pull request.
