# Library Management System

A comprehensive C++ console application for managing library operations including book catalog, member management, book circulation, and fine tracking.

## Features

### Librarian Functions
- **Manage Books**: Add, update, delete, and view books
- **Manage Members**: Add new members, view all members, update member status
- **Issue/Return Books**: Track book loans and returns
- **Fines & Notifications**: Manage fines and send notifications to members

### Member Functions
- **Search Catalog**: Search books by title, author, or category
- **View My Loans**: Check borrowed books and due dates
- **View Profile**: View member information
- **Update Profile**: Update name and phone number

## Technology Stack

- **Language**: C++14
- **Build System**: Visual Studio (MSVC)
- **Architecture**: Modular with separate managers for authentication, circulation, and database operations

## Project Structure

```
LibraryManagementSystem/
 MenuSystem.cpp/h         # User interface and menu handling
 DatabaseManager.cpp/h    # Database operations
 AuthenticationManager.cpp/h  # Login and registration
 CirculationManager.cpp/h # Book issuance and return logic
 Database.cpp/h           # Database implementation
 Models.h                 # Data structures
 TestData.cpp             # Sample test data
 Dockerfile               # Docker configuration
 docker-compose.yml       # Docker Compose setup
 LibraryManagementSystem.cpp  # Main entry point
```

## Getting Started

### Prerequisites
- Visual Studio 2019 or later with C++ tools
- Windows OS (for .exe build)
- Docker (optional, for containerized deployment)

### Build & Run (Local)

1. **Clone the repository**
```bash
git clone https://github.com/utherntlinh/library.git
cd library
```

2. **Open Visual Studio**
```bash
LibraryManagementSystem.sln
```

3. **Build the project**
   - Press `Ctrl+Shift+B` or Build ? Build Solution

4. **Run the application**
   - Press `F5` or Debug ? Start Debugging

### Build & Run (Docker)

1. **Build Docker image**
```bash
docker-compose up --build
```

2. **Stop the container**
```bash
docker-compose down
```

## Test Accounts

### Member Accounts
| Email | Password |
|-------|----------|
| huy@email.com | huy123 |
| hung@email.com | hung123 |
| linh@email.com | linh123 |

### Librarian Account
| Username | Password |
|----------|----------|
| admin | admin123 |

## Test Data

The system comes pre-loaded with:
- **3 Books**: Harry Potter, Clean Code, The Alchemist
- **3 Members**: Huy, Hùng, Linh
- **2 Sample Loans**: Active book loans for testing
- **1 Sample Fine**: Unpaid fine for testing fine management
- **1 Notification**: Sample notification for member

## Member Workflow Example

1. Login as member (huy@email.com / huy123)
2. Search for books in catalog
3. View active loans (Harry Potter borrowed until 2026-04-15)
4. Check profile information
5. Update profile if needed

## Librarian Workflow Example

1. Login as librarian (admin / admin123)
2. View all members
3. Check fines and send notifications
4. Issue/return books
5. Manage book catalog

## Database

- **Type**: In-memory database with optional persistence
- **Main Tables**: Books, Members, Loans, Fines, Notifications, Librarians
- **Database File**: LibraryManagement.db (auto-created)

## Known Issues

- Password validation for members is basic (no hashing implemented)
- Database persistence needs improvement
- Console UI only (no GUI)

## Future Enhancements

- [ ] GUI interface with Qt or wxWidgets
- [ ] Password hashing and encryption
- [ ] Database migration to SQLite/MySQL
- [ ] Email notifications for members
- [ ] Book reservation system
- [ ] Late return penalties
- [ ] Book ratings and reviews

## Code Style

- Follows C++14 standard
- Uses smart pointers (shared_ptr)
- Modular design with separate manager classes
- Console I/O for user interaction

## Author

**Uthern T. Linh**
- GitHub: [@utherntlinh](https://github.com/utherntlinh)
- Email: utherntlinh@email.com

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

For issues or questions, please open an issue on GitHub.

---

**Last Updated**: April 2026
**Version**: 1.0.0
