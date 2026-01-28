
# 🗳️ Electronic Voting System

A secure electronic voting application with admin controls and multi-layer voter authentication.

## 📋 Features

### Admin Portal
- Manage candidates (Add, Update, Delete)
- Manage voters (Add, Update, Delete)
- Control voting process (Start/End)
- View real-time results
- Remove individual votes

### Voter Interface
- Secure authentication (CNIC, Password, Age verification)
- Human verification with random PIN
- Prevents duplicate voting
- Vote casting with candidate selection

## 🛠️ Tech Stack
- **Languages:** C++, Python 3
- **Concepts:** File I/O, Data Structures, Authentication, CRUD Operations

## 🚀 How to Run

### C++ Version
```bash
g++ FinalCPEVS__1_.cpp -o voting_system
./voting_system
```

### Python Version
```bash
python elect_vot.py
```

## 🔐 Default Admin Credentials
- Username: `admin`
- Password: `password`
- **Note:** Change these in production!

## 📊 Project Structure
- Candidate management with ID, Name, Party, Symbol
- Voter registration with CNIC validation
- Persistent storage using text files
- Age restriction (18+)

## 🎯 Skills Demonstrated
- File Handling & Data Persistence
- User Authentication & Authorization
- Input Validation & Error Handling
- Menu-Driven Application Design
- Security Implementation
- CRUD Operations

## 📝 Future Improvements
- Database integration (MySQL/PostgreSQL)
- GUI interface
- Encryption for passwords
- More detailed results analytics

## 👨‍💻 Author
Aqsa Sajjad

## 📄 License
This project is open source and available for educational purposes.
