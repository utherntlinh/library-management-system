# Installation Guide

## ?? Local Installation (Windows)

### Step 1: Prerequisites
- Windows 10/11 64-bit
- Visual Studio 2019 or later with C++ tools
- Git

### Step 2: Clone Repository
```bash
git clone https://github.com/utherntlinh/library.git
cd library
```

### Step 3: Open in Visual Studio
```bash
# Double-click LibraryManagementSystem.sln
# Or use command line:
start LibraryManagementSystem.sln
```

### Step 4: Build
1. In Visual Studio, select **Release** configuration
2. Select **x64** platform
3. Press `Ctrl+Shift+B` or go to **Build ? Build Solution**
4. Wait for build to complete

### Step 5: Run
- Press `F5` or go to **Debug ? Start Debugging**
- Or run the executable directly:
```bash
x64/Release/LibraryManagementSystem.exe
```

---

## ?? Docker Installation

### Step 1: Prerequisites
- Docker Desktop installed and running
- Windows containers enabled
- At least 4GB RAM allocated to Docker

### Step 2: Clone Repository
```bash
git clone https://github.com/utherntlinh/library.git
cd library
```

### Step 3: Build Docker Image
```bash
docker-compose build
```

### Step 4: Run Container
```bash
docker-compose up
```

### Step 5: Access Application
- Application runs inside container with console output visible
- To stop: `docker-compose down`

---

## ?? Manual Build Instructions

### Using MSBuild (Command Line)
```bash
cd D:\CNPM\LibraryManagementSystem

# Restore NuGet packages
nuget restore LibraryManagementSystem.sln

# Build Release x64
msbuild /m /p:Configuration=Release /p:Platform=x64 LibraryManagementSystem.sln

# Run
x64\Release\LibraryManagementSystem.exe
```

### Using Visual Studio 2019 Developer Command Prompt
```bash
@echo off
setlocal enabledelayedexpansion

REM Navigate to project
cd /d "D:\CNPM\LibraryManagementSystem"

REM Clean previous builds
rmdir /s /q x64 2>nul

REM Build
msbuild LibraryManagementSystem.sln /p:Configuration=Release /p:Platform=x64 /m

REM Check result
if exist "x64\Release\LibraryManagementSystem.exe" (
    echo Build successful!
    x64\Release\LibraryManagementSystem.exe
) else (
    echo Build failed!
    exit /b 1
)
```

---

## ?? First Run Checklist

After installation, verify:

- [ ] Application starts without errors
- [ ] Main menu displays correctly
- [ ] Can login as admin/admin123
- [ ] Can login as member huy@email.com/huy123
- [ ] Can search books
- [ ] Can view member loans
- [ ] Database file created (LibraryManagement.db)

---

## ?? Troubleshooting

### Build Fails
**Problem**: `msbuild not found`
- **Solution**: Add Visual Studio tools to PATH or use full path:
```bash
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
```

### Docker Build Fails
**Problem**: `Cannot find base image`
- **Solution**: Ensure Docker is running and has internet connection
```bash
docker pull mcr.microsoft.com/windows/servercore:ltsc2022
```

### Database Issues
**Problem**: `Database not found` or `Failed to initialize`
- **Solution**: Delete existing database and restart:
```bash
del LibraryManagement.db
LibraryManagementSystem.exe
```

### Docker Container Exits Immediately
**Problem**: Container exits without showing errors
- **Solution**: Check logs:
```bash
docker logs <container_id>
docker-compose logs
```

---

## ?? System Requirements

| Component | Requirement |
|-----------|------------|
| OS | Windows 10/11 64-bit |
| RAM | 512MB minimum, 4GB for Docker |
| Disk | 100MB executable + database space |
| Compiler | MSVC v142+ (Visual Studio 2019+) |
| C++ Standard | C++14 or higher |

---

## ?? Support

If you encounter issues:
1. Check [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
2. Review [README.md](README.md)
3. Open an issue on GitHub with:
   - Error message
   - Build output
   - System information (OS, Visual Studio version)

---

**Last Updated**: April 2026
**Version**: 1.0.0
