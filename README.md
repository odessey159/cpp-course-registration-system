# cpp-course-registration-system

A full-stack course registration system featuring a C++ backend, a lightweight HTML
frontend, and an SQLite database. The system is designed with a clear separation
between client and server, where the frontend communicates with the backend via
HTTP requests using JSON as the data format.

The backend is implemented in C++ and is responsible for core business logic,
including authentication, course management, enrollment handling, and schedule
conflict detection. All database operations are executed using prepared statements
to ensure data integrity and prevent SQL injection vulnerabilities.

The frontend provides role-based user interfaces for students and administrators.
Students can register, log in, browse available courses, select or drop courses,
and view their current schedules. Administrators can manage course information,
review registration status, and maintain system data.

This project focuses on system architecture, backend logic, and database design,
with a deliberately lightweight frontend to emphasize functionality and clarity.
The system is intended for local deployment and can be extended to support more
advanced features or alternative database backends in the future.
