# EventEase

A terminal-based, menu-driven event management system designed to simplify the process of organizing and managing events efficiently.

## 📌 Description

EventEase is a lightweight, menu-driven event management system that helps users plan, book, and manage events without the complexity of traditional graphical interfaces. It provides a streamlined experience for both clients and organizers, allowing seamless event booking, tracking payments, managing reminders, and generating insights.

Designed with efficiency in mind, EventEase offers a text-based, resource-friendly alternative to bulky event management platforms, making it ideal for low-spec machines, remote setups, and users who prefer keyboard-driven navigation.

## 🚀 Getting Started

### 📂 Dependencies

Before running EventEase, ensure you have the following installed:

*   **Compiler:** GCC (MinGW for Windows) or Clang

### 🔧 Installation

1.  Clone the repository

    ```sh
    git clone https://github.com/yourusername/EventEase.git
    cd EventEase
    ```

2.  Compile the program

    ```sh
    gcc main.c -o EventEase
    ```

3.  Run the program

    ```sh
    ./EventEase
    ```

## 🏃 Executing the Program

To use EventEase, follow these steps:

1.  Run the program

    ```sh
    ./EventEase
    ```

2.  Select your role
    *   📌 **Client** – Book and manage your events
    *   📌 **Organizer** – Manage bookings, event types, and packages

3.  Navigate through the menu
    *   Use **numeric input** to select options.
    *   Follow **prompts** to enter event details.
    *   **Confirm actions** before making changes.

## ❓ Help

For common issues, try the following:

*   **Program not compiling?**
    Ensure you have `gcc` installed. Run:

    ```sh
    gcc --version
    ```

*   **Missing files or folders?**
    Ensure the `data/` directory exists and contains the necessary subdirectories:

    ```sh
    mkdir -p data/users data/events data/packages
    ```

*   **Want to reset all data?**
    Delete stored event data manually:

    ```sh
    rm -rf data/
    ```

For additional troubleshooting, refer to the documentation.

## 📌 Features

*   ✅ **Event Booking** – Clients can book events based on predefined event types and packages.
*   ✅ **Event Management** – Organizers can create, update, or delete event types and packages.
*   ✅ **Reminders & Notifications** – Set and receive event reminders.
*   ✅ **Text-based Insights & Reports** – Generate statistics on bookings and event popularity.
*   ✅ **Lightweight & Fast** – Optimized for terminal-based execution.

For details, check the commit history.

## 📜 License

This project is licensed under the MIT License. See the [LICENSE.md](https://github.com/Nesqyk/EventEase?tab=MIT-1-ov-file) file for details.
