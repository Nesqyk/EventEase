# EventEase
A terminal-based, menu-driven event management system. 

## Userflow

### Start
1. User launches the EventEase system in the terminal.

2. Authentication System (Optional):
If handling confidential data, implement a login system.
Benefits: Enhances security and tracks user activity.
3. Apply padding for centered terminal output for a polished user interface.

### Main Menu
#### Options:
1. Dashboard
    - Displays:
        - Upcoming Events
        - Completed Events
        - Quick Stats (e.g., Total Events, Revenue, Popular Event Types).
        - **Navigation:** Back to Main Menu.
2. Create Event
    - Prompt the user to select the type of event (e.g., Wedding, Corporate Meeting).
    - Automatically Generated Fields:
        - ID
        - Date Created
        - User Inputs:
        - Client Name
        - Cost
        - Venue
        - Date of Event
        - Number of Attendees
    - Additional Fields:
        - Classification (0 = Not Done, 1 = Done)
    - Actions:
        - Review Details
        - Confirm or Go Back to the Main Menu.
3. My Events
    - Features:
        - Search Event:
            - By Event ID or Client Name.
    - View Event:
        - Display overview including: ID, Name, Date, Venue, Status.
    - Add status legend: Upcoming, Tomorrow, Canceled.
    - Edit Event:
        - Enter Event ID.
    - Editable Fields:
        - Client Name
        - Cost
        - Venue
        - Number of Attendees
        - Classification.
    - Navigation: Back to Main Menu.
4. Generate Reports 
    - Search Event by ID.
    - Available Reports:
        - Cost Breakdown
        - Profit Calculation
        - Receipt (Printable Format if possible).
    - Navigation: Back to Main Menu.
5. Check Reviews
    - Search Event by ID.
    - Actions:
    - Add Review
    - Delete Review
    - **Navigation:** Back to Main Menu.
6. Exit
    - Automatically save all data before closing.

### Additional Improvements
Error Handling:
Validate inputs (e.g., ensure numeric values for cost and attendees).
Display friendly error messages for invalid or incomplete entries.
Search Enhancement:
Include partial matching for searches (e.g., partial client names).
Reports Section:
Option to generate summary reports for a range of dates or types of events.
Customization:
Allow custom tags or notes for events for better categorization.
Back Option:
Ensure every submenu has a “Back to Main Menu” option.
