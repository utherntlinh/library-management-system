# Docker Setup Guide - Library Management System

## ?? Important Note

This is a **Windows Console Application**. Docker support is optional for deployment/demonstration purposes.

**For Development: Skip Docker and run locally** (faster and simpler)

---

## ?? Quick Start (Recommended: Local Build)

### Prerequisites
- Visual Studio 2019+ with C++ tools
- Windows 10/11 64-bit

### Build and Run Locally
```bash
# 1. Open LibraryManagementSystem.sln in Visual Studio
# 2. Press Ctrl+Shift+B to build
# 3. Press F5 to run
# Or double-click: x64\Release\LibraryManagementSystem.exe
```

---

## ?? Docker Option (For Container Deployment)

### Prerequisites
- Docker Desktop with Windows containers enabled
- 4GB RAM allocated to Docker
- 10GB free disk space

### Step 1: Build Executable Locally First
```bash
# Using Visual Studio
# Ctrl+Shift+B (Build Solution)

# Or using command line
msbuild LibraryManagementSystem.sln /p:Configuration=Release /p:Platform=x64
```

### Step 2: Build Docker Image
```bash
# Option A: Using docker-compose (Recommended)
docker-compose build

# Option B: Using Docker CLI
docker build -f Dockerfile.local -t library-management:local .
```

### Step 3: Run Container
```bash
# Option A: Using docker-compose
docker-compose up -d

# Option B: Using Docker CLI
docker run -it --name lib-app library-management:local

# View logs
docker-compose logs -f
```

### Step 4: Stop Container
```bash
# Using docker-compose
docker-compose down

# Using Docker CLI
docker stop lib-app
docker rm lib-app
```

---

## ?? Dockerfile Options

### `Dockerfile` (Full Build in Container - Not Recommended)
- Attempts to build Visual Studio project inside container
- Requires Visual Studio Build Tools in Windows image
- Takes longer, larger image size
- **Use only if local build not possible**

### `Dockerfile.local` (Recommended)
- Uses pre-built executable from local machine
- Smaller image size (~1GB vs 4GB+)
- Faster deployment
- **Use this for deployment**

---

## ?? Test Accounts

When the application starts, use:

**Member Accounts:**
- Email: huy@email.com, Password: huy123
- Email: hung@email.com, Password: hung123
- Email: linh@email.com, Password: linh123

**Librarian Account:**
- Username: admin, Password: admin123

---

## ?? Troubleshooting

### Image Build Fails
```bash
# Error: "failed to resolve source metadata"
# Solution: Build executable first locally
msbuild LibraryManagementSystem.sln /p:Configuration=Release /p:Platform=x64

# Then rebuild Docker image
docker build -f Dockerfile.local -t library-management:local .
```

### Container Exits Immediately
```bash
# Check logs
docker logs <container_name>

# Run with interactive terminal
docker run -it library-management:local
```

### Port Conflict
```bash
# Edit docker-compose.yml
# Change: "5000:5000" to "5001:5000"

# Then restart
docker-compose down
docker-compose up
```

For more troubleshooting, see [DOCKER_TROUBLESHOOTING.md](DOCKER_TROUBLESHOOTING.md)

---

## ?? Verification

After running, verify the container is working:

```bash
# In another terminal, check running containers
docker ps

# Should show: library-management-system running
```

Expected output:
```
CONTAINER ID   IMAGE                      NAMES
abc123def456   library-management:local   library-management-system
```

---

## ?? Commands Reference

| Command | Purpose |
|---------|---------|
| `docker-compose build` | Build Docker image |
| `docker-compose up` | Start container (show logs) |
| `docker-compose up -d` | Start container (background) |
| `docker-compose down` | Stop and remove container |
| `docker-compose logs -f` | View live logs |
| `docker ps -a` | List all containers |
| `docker images` | List all images |
| `docker system prune -a` | Clean up unused images |

---

## ?? Recommendation

**For this project:**
- ? **Development**: Run locally without Docker
- ? **Testing**: Run locally using Visual Studio
- ?? **Deployment**: Use Docker only if containerization needed
- ? **CI/CD**: GitHub Actions handles automated builds

---

## ?? Additional Resources

- [DOCKER_TROUBLESHOOTING.md](DOCKER_TROUBLESHOOTING.md) - Detailed troubleshooting guide
- [INSTALLATION.md](INSTALLATION.md) - Full installation instructions
- [README.md](README.md) - Project overview

---

**Version**: 1.0.0  
**Last Updated**: April 2026  
**Status**: Docker support is optional for this project

