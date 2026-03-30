# cpp-course-registration-system

A full-stack course registration system enhanced with an LLM-powered agent layer, featuring a C++ backend, a Python-based agent service, a lightweight HTML frontend, and an SQLite database. The system follows a modular architecture where the frontend communicates with backend services via HTTP using JSON.

The backend is implemented in C++ and is responsible for core business logic, including authentication, course management, enrollment handling, and schedule conflict detection. All database operations are executed using prepared statements to ensure data integrity and prevent SQL injection vulnerabilities.

A Python-based LLM Agent service is introduced to enable natural language interaction. The agent interprets user queries, decomposes tasks, and invokes backend functionalities as tools (e.g., course search, schedule conflict checking, prerequisite validation). This allows users to perform complex operations such as course planning through flexible natural language input.

The frontend provides role-based user interfaces for students and administrators. In addition to traditional operations (registration, login, course browsing, enrollment, and schedule viewing), users can interact with the system through a text-based interface powered by the agent.

This project emphasizes system architecture, tool abstraction, and LLM-driven workflow design, where deterministic logic is implemented as backend tools and decision-making is handled by the agent. The system is designed for local deployment and can be further extended with advanced agent workflows, additional tools, or alternative database backends.
