# C++ GUI Application Project: Sruighlea

## 1. Project Goal
To create a user-friendly desktop GUI application designed for creative writers, enabling them to manage the rich, complex lives of their characters. The application will allow for easy viewing, adding, and editing of detailed character histories, relationships, and life events.

## 2. Technology Stack
- **Language:** C++
- **GUI Framework:** Qt
- **Database:** SQLite (using a new, custom-designed schema to support complex data structures)

## 3. Core Features (New Vision)
The application will evolve beyond a simple database viewer into a comprehensive character management tool.
- **Comprehensive Character Dossiers:** View all aspects of a character's life on a single, intuitive screen: core profile, physical attributes, magical abilities, and detailed histories.
- **Dynamic Relationship Graph:** Visualize and manage the complex, time-variant network of relationships. Track friendships, rivalries, and family ties as they evolve over time.
- **Historical Tracking:** Log and review key life events, including academic records, career paths, and changes in residence, providing a complete character timeline.
- **Flexible Data Management:** Easily add, edit, and search for characters and their extensive life events through a clean and polished user interface.

## 4. New Database Design
The application is built upon a new, robust database schema designed for flexibility and power. It handles complex data like structured names (JSON), time-based events, and reciprocal relationships.
- **Conceptual Design:** [./neo_design/schema_draft.md](./neo_design/schema_draft.md)
- **SQL Implementation:** [./neo_design/schema.sql](./neo_design/schema.sql)

## 5. Next Major Milestone: Application Refactoring
The next primary goal is to refactor the C++ application to be fully compatible with the new database schema. This involves:
1.  Updating the `DatabaseManager` to connect to the new database file.
2.  Completely rewriting the `CharacterService` and all associated SQL queries to work with the new table structures.
3.  Redesigning the UI (`MainWindow`, dialogs, etc.) to display and manage the new, richer data, such as relationship histories and academic records.

## MD Files
- [TODO](./TASKS.md)
- [Diary](./DevLog.md)
