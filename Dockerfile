# Library Management System - C++ Docker Build
FROM ubuntu:22.04

# Avoid timezone prompt during apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy all source files
COPY *.cpp ./
COPY *.h ./

# Compile all .cpp files
RUN g++ -std=c++17 -o LibraryManagementSystem \
    LibraryManagementSystem.cpp \
    Database.cpp \
    DatabaseManager.cpp \
    AuthenticationManager.cpp \
    CirculationManager.cpp \
    MenuSystem.cpp \
    TestData.cpp \
    && echo "[OK] Build successful"

# Create data directory
RUN mkdir -p data

# Set environment variable for database path
ENV DATABASE_PATH=/app/data/LibraryManagement.db

# Run the application
CMD ["./LibraryManagementSystem"]
