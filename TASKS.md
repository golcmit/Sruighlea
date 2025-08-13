# Sruighlea - Product Backlog

This document lists the features, improvements, and tasks for the project.

## Development Approach

- **Test-Driven Development (TDD)**: All new features and modifications will be developed following the TDD cycle.
  1.  **Red**: Write a failing test that defines the desired functionality or improvement.
  2.  **Green**: Write the minimum amount of product code necessary to make the test pass.
  3.  **Refactor**: Clean up and optimize the code while ensuring all tests still pass.

This ensures that all code is covered by tests, improving robustness and maintainability.

## Milestone 1: Basic Viewer (Completed)

- [x] Establish connection to the SQLite database.
- [x] Fetch a list of characters with basic information (name, house).
- [x] Display the character list in a simple table view upon application launch.

## Milestone 2: Core Functionality

### Feature: Character Detail View
- [x] Implement a backend service to fetch character details by ID (TDD Cycle 1).
- [x] When a user selects a row in the character list, display all details of that character in a separate panel or window.
- [x] Design a clear and readable layout for the detail view.

### Feature: Search and Filter
- [ ] Add a search bar to the main window.
- [ ] Implement real-time filtering of the character list based on text input (search by first name or last name).

### Feature: Add New Character
- [ ] Add an "Add New Character" button.
- [ ] Create a form/dialog to input all necessary information for a new character.
- [ ] Implement data validation for the input fields.
- [ ] Save the new character to the database.

### Feature: Edit Character
- [ ] Add an "Edit" button to the detail view.
- [ ] Allow users to modify the information of an existing character.
- [ ] Update the character's information in the database.

## UI/UX Improvements

- [x] Make the table view resizable with the main window.
- [ ] Set a proper application icon.
- [ ] Improve the visual styling of the table and other UI elements for a more polished look.

## Technical Debt & Refactoring

- [ ] Refactor database connection logic into a dedicated singleton or service class.
- [ ] Move SQL queries from inline code to a separate constants file or a query manager.
- [ ] Implement a more robust error handling and logging mechanism.