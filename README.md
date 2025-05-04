# 🍎Pomodoro Timer Application

[![In Development](https://img.shields.io/badge/Status-In%20Development-yellow)](https://shields.io/)

## Description

Pomodoro Timer is a project based on the pomodoro technique developed by the author [Francesco Cirillo](https://www-pomodorotechnique-com.translate.goog/the-pomodoro-technique-book/?_x_tr_sl=en&_x_tr_tl=pt&_x_tr_hl=pt&_x_tr_pto=tc). The objective of this project was to design a system focused on increasing productivity and focus.

The application operates in the system tray, providing discreet access to timer controls and settings. Users can customize the length of work periods and breaks, set up sound alerts, and review their session history through detailed logs.

## Features

-   **🪟 System Tray Integration:** The application runs minimized in the system tray, keeping your workspace clean and organized.
-   **⏲️ Customizable Timers:** Adjust the duration of your work sessions, short breaks, and long breaks according to your preferences.
-   **⏰ Sound Alerts:** Configure custom sound notifications to alert you when a timer ends, ensuring you stay on schedule.
-   **📈 Session Logs:** Detailed logs of your Pomodoro sessions are maintained, allowing you to track your productivity over time.
-   **⚙️ Configuration Settings:** Fine-tune various aspects of the application, from timer durations to sound preferences, ensuring a tailored experience.
- **🎨 Multiple Themes:** Multiple themes to change the look of the application.

### Demonstration
![funcionalidades](https://github.com/user-attachments/assets/2bcd69c3-4b2a-4a1a-9757-4776e201d8f7)


## Technologies used

- ``C++``
- ``QT``
- ``CMake``

## Building the Project

Follow these steps to build the Pomodoro Timer application from source:

1.  **Prerequisites:**
    -   Qt development environment (with Qt Creator is recommended).
    -   C++ compiler (compatible with your Qt version).
    -   QMake build system.

2.  **Clone the Repository:**
```
    git clone [repository_url]
    cd [project_directory]
```
3.  **Build with QMake:**
```
    qmake
    make # or nmake on Windows 
```
## Running the Project

Once the build process is complete, you can run the application directly from the build directory:

1.  **Navigate to the Build Directory:**
```
    cd build-directory # Usually in the project directory
```
2.  **Execute the Application:**
```
    ./pomodoro-timer # or pomodoro-timer.exe on Windows
```
The application will start in the system tray.

## Upcoming Features

- **Improved Session Analytics:** Provides more detailed statistics and visuals for session analysis.

- **Cloud Sync:** Allows users to sync their settings and session logs across multiple devices.

- **Add More Themes:** New and more attractive themes for the app.

- **More Sound:** Options: More alarm sound options

## Contributing

Contributions are welcome! If you have suggestions, bug reports, or wish to contribute code, please feel free to open an issue or submit a pull request.
