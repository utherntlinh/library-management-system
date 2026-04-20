# Library Management System

This project is a C++ console-based application designed to manage library operations including book catalog management, member management, and book circulation.

## Features

* Book management: add, update, delete, and search books
* Member management: create and manage member accounts
* Book circulation: issue and return books
* Fine tracking for overdue books

## Technology Stack

* Programming Language: C++14
* Development Environment: Visual Studio

## Project Structure

* MenuSystem.cpp/h – User interface and navigation
* AuthenticationManager.cpp/h – User login and authentication
* CirculationManager.cpp/h – Book issuing and returning
* Database.cpp/h – Core data storage
* DatabaseManager.cpp/h – Data processing and management
* Models.h – Data structures
* TestData.cpp – Sample data for testing
* LibraryManagementSystem.cpp – Main program

## Build and Run

1. Open the solution file in Visual Studio
2. Build the project (Ctrl + Shift + B)
3. Run the application (F5)

## Testing

The system was tested using predefined test cases including:

* User login validation
* Book search functionality
* Book issuing and returning
* Fine calculation

## Docker

A Dockerfile is provided to support containerized deployment of the application.

## Team Work Distribution

* Authentication Module: AuthenticationManager
* Circulation Module: CirculationManager
* Database Module: Database, DatabaseManager
* User Interface: MenuSystem
* Integration & Testing: Main program and test data
