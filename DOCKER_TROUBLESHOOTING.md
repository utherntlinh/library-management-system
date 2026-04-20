# Docker Troubleshooting Guide

## ?? Common Issues and Solutions

### 1. Image Not Found Error

**Error Message:**
```
failed to resolve source metadata for mcr.microsoft.com/windows/servercore:ltsc2022
```

**Cause:** 
- Docker registry connection issue
- Base image not available in your region
- Docker not properly configured

**Solutions:**

**Option A: Use Pre-built Executable (Recommended)**
```bash
# Build locally first using Visual Studio
# Press Ctrl+Shift+B in Visual Studio

# Then use Dockerfile.local
docker build -f Dockerfile.local -t library-management:local .

# Run
docker run -it library-management:local
```

**Option B: Build Without Docker (Easiest)**
```bash
# Just use Visual Studio directly
# Open: LibraryManagementSystem.sln
# Build ? Build Solution (Ctrl+Shift+B)
# Run the executable: x64\Release\LibraryManagementSystem.exe
```

**Option C: Fix Registry Issues**
```bash
# Ensure Docker can access registry
docker pull mcr.microsoft.com/windows/servercore:ltsc2022

# Or use different base image
# Edit Dockerfile to use: windows:ltsc2022 (simpler)
```

---

### 2. Docker Container Exits Immediately

**Error:**
```
Container exits with no output
```

**Solution:**
```bash
# Check logs
docker logs <container_id>

# Run with interactive terminal
docker run -it library-management:latest

# Or use docker-compose with logs
docker-compose up
docker-compose logs -f
```

---

### 3. File Not Found in Container

**Error:**
```
COPY failed: file not found in build context
```

**Solution:**
```bash
# Ensure executable exists locally first
dir x64\Release\LibraryManagementSystem.exe

# If not, build in Visual Studio first
# Then rebuild Docker image
docker build --no-cache -t library-management:latest .
```

---

### 4. Docker Compose Port Conflict

**Error:**
```
Port 5000 already in use
```

**Solution:**
```bash
# Edit docker-compose.yml
# Change port mapping: "5001:5000" instead of "5000:5000"

# Or kill process using port
netstat -ano | findstr :5000
taskkill /PID <PID> /F
```

---

### 5. Windows Container Platform Issue

**Error:**
```
image is for platform windows/amd64, expected linux/amd64
```

**Solution:**
```bash
# Switch Docker to Windows containers
# In Docker Desktop:
# Settings ? Daemon ? Switch to Windows containers

# Or use Linux build (create Dockerfile.linux):
# FROM ubuntu:22.04
# (Cross-compilation not practical for this project)
```

---

## ? Recommended Workflow

### For Local Development
```bash
# 1. Clone repo
git clone https://github.com/utherntlinh/library.git
cd library

# 2. Build using Visual Studio
# Open LibraryManagementSystem.sln
# Press Ctrl+Shift+B

# 3. Run executable
x64\Release\LibraryManagementSystem.exe
```

### For Docker (After Local Build)
```bash
# 1. Build local Docker image
docker build -f Dockerfile.local -t library-management:local .

# 2. Run container
docker run -it --name lib-app library-management:local

# 3. Stop container
docker stop lib-app
docker rm lib-app
```

### For Docker Compose
```bash
# 1. Edit docker-compose.yml if needed
# 2. Build and run
docker-compose up --build

# 3. View logs
docker-compose logs -f

# 4. Stop
docker-compose down
```

---

## ?? Debugging Commands

```bash
# Check Docker version
docker --version

# Check image architecture
docker image inspect library-management:local

# List all containers
docker ps -a

# View container logs
docker logs <container_name>

# Access running container
docker exec -it <container_name> powershell

# Check available disk space
docker system df

# Clean up unused images
docker image prune

# Remove all dangling images
docker system prune -a
```

---

## ?? Database Issues in Container

**Problem:** Database file not persisting

**Solution:**
```bash
# Use volume mount in docker-compose.yml
volumes:
  - ./data:/app/data
  - library-data:/app

# Or use named volume
docker volume create library-data

# Run with volume
docker run -v library-data:/app library-management:local
```

---

## ?? Alternative: Skip Docker Entirely

Since this is a Windows Console application, Docker might be overkill:

```bash
# Just build and run locally
# Visual Studio: Ctrl+Shift+B ? F5

# Or command line
cd D:\CNPM\LibraryManagementSystem
msbuild LibraryManagementSystem.sln /p:Configuration=Release /p:Platform=x64
x64\Release\LibraryManagementSystem.exe
```

---

## ?? Still Having Issues?

1. **Check system requirements**
   - Windows 10/11 64-bit
   - 4GB RAM minimum
   - 10GB free disk space

2. **Verify Visual Studio C++ tools installed**
   ```bash
   # Check MSVC version
   cl.exe /?
   ```

3. **Reinstall Docker**
   - Uninstall Docker Desktop
   - Restart machine
   - Reinstall latest version

4. **Open GitHub Issue**
   - Include error message
   - Include output of: `docker --version`
   - Include OS version

---

**Recommendation**: For this project, **skip Docker** and run locally for faster development cycle! ??
