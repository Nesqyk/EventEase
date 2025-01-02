
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>   
#include <string.h>   
#include <stdio.h>
#include <direct.h>
#include <ctype.h>
#include <sys/stat.h>
#include "utils.h"
#include <time.h>


// type: info 0, warn 1, error 2
void log_event(int client_id,int type, const char *message) 
{
    char log_path[100];
    char log_filename[100];
    FILE *log_file;
    
    sprintf(log_path, "data/users/%d/logs/", client_id);
    
    if (_access(log_path, 0) != 0) // If directory doesn't exist
    {
        // Try creating the directory
        if (_mkdir(log_path) != 0) 
        {
            perror("Failed to create directory");
            return;
        }
    }

    time_t t = time(NULL);
    struct tm tm_info;
    char date_str[20], time_str[20];
    
    localtime_s(&tm_info, &t); 
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", &tm_info);
   strftime(time_str, sizeof(time_str), "%H:%M:%S", &tm_info);



    sprintf(log_filename, "%s%s.txt", log_path, date_str);

    log_file = fopen(log_filename, "a");
    if (log_file == NULL) 
    {
        perror("Failed to open log file");
        return;
    }

    char *type_log;

    switch(type)
    {
      case 1:
         type_log = "[INFO]";
         break;
      case 2:
         type_log = "[WARN]";
         break;
      case 3:
         type_log = "[ERROR]";
         break;
      default:
         type_log = "[INFO]";
         break;
    }


   fprintf(log_file, "%s %s %s: %s\n", date_str, time_str, type_log, message);

   fclose(log_file);
}


int is_numeric(const char *input) {
    if (input == NULL || *input == '\0') {
        return 0;
    }

    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit((unsigned char)input[i])) {
            return 0; 
        }
    }
    return 1;
}

void create_dir(const char *path)
{
   if(_mkdir(path) != 0)
   {
      perror("[ERROR] Failed creating directory");
   }
}


FILE *create_file(const char *path) {
    FILE *file = fopen(path, "w");

    if (file == NULL) {
        perror("[ERROR] Error creating file");
        return NULL; 
    }
    return file;
}

int remove_directory(const char *path) {
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d) {
      struct dirent *p;

      r = 0;
      while (!r && (p=readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
             continue;

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf) {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode))
                   r2 = remove_directory(buf);
                else
                   r2 = unlink(buf);
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }

   if (!r)
      r = rmdir(path);

   return r;
}


// Function that returns the index of the 
// day for date DD/MM/YYYY 
int dayNumber(int day, int month, int year) 
{ 

    static int t[] = { 0, 3, 2, 5, 0, 3, 
                    5, 1, 4, 6, 2, 4 }; 
    year -= month < 3; 
    return (year + year / 4 
            - year / 100 
            + year / 400 
            + t[month - 1] + day) 
        % 7; 
} 

// Function that returns the name of the 
// month for the given month Number 
// January - 0, February - 1 and so on 
char* getMonthName(int monthNumber) 
{ 
    char* month; 

    switch (monthNumber) { 
    case 0: 
        month = "January"; 
        break; 
    case 1: 
        month = "February"; 
        break; 
    case 2: 
        month = "March"; 
        break; 
    case 3: 
        month = "April"; 
        break; 
    case 4: 
        month = "May"; 
        break; 
    case 5: 
        month = "June"; 
        break; 
    case 6: 
        month = "July"; 
        break; 
    case 7: 
        month = "August"; 
        break; 
    case 8: 
        month = "September"; 
        break; 
    case 9: 
        month = "October"; 
        break; 
    case 10: 
        month = "November"; 
        break; 
    case 11: 
        month = "December"; 
        break; 
    default:
        month = "Invalid"; // Warning: Improper usage of function can lead to undefined behavior.
        break;

    } 
    return month; 
} 

// Function to return the number of days 
// in a month 
int numberOfDays(int monthNumber, int year) 
{ 
    // January 
    if (monthNumber == 0) 
        return (31); 

    // February 
    if (monthNumber == 1) { 
        // If the year is leap then Feb 
        // has 29 days 
        if (year % 400 == 0 
            || (year % 4 == 0 
                && year % 100 != 0)) 
            return (29); 
        else
            return (28); 
    } 

    // March 
    if (monthNumber == 2) 
        return (31); 

    // April 
    if (monthNumber == 3) 
        return (30); 

    // May 
    if (monthNumber == 4) 
        return (31); 

    // June 
    if (monthNumber == 5) 
        return (30); 

    // July 
    if (monthNumber == 6) 
        return (31); 

    // August 
    if (monthNumber == 7) 
        return (31); 

    // September 
    if (monthNumber == 8) 
        return (30); 

    // October 
    if (monthNumber == 9) 
        return (31); 

    // November 
    if (monthNumber == 10) 
        return (30); 

    // December 
    if (monthNumber == 11) 
        return (31);
} 

// Function to print the calendar of 
// the given year 
void print_calendar(int year, int month, int events[10]) 
{ 
    printf("     Calendar - %d\n\n", year); 
    int days; 

    // Index of the day from 0 to 6 
    int current = dayNumber(1, 1, year); 

    // i for Iterate through months 
    // j for Iterate through days 
    // of the month - i 
    for (int i = 0; i < 12; i++) { 
        days = numberOfDays(i, year); 

        // Print the current month name 
        printf("\n ------------%s-------------\n", 
            getMonthName(i)); 

        // Print the columns 
        printf(" Sun Mon Tue Wed Thu Fri Sat\n"); 

        // Print appropriate spaces 
        int k; 
        for (k = 0; k < current; k++) 
            printf("     "); 

        for (int j = 1; j <= days; j++) { 
            printf("%5d", j); 

            if (++k > 6) { 
                k = 0; 
                printf("\n"); 
            } 
        } 

        if (k) 
            printf("\n"); 

        current = k; 
    } 

    return; 
} 


int validate_date(int year, int month, int day) 
{
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;

    // Check days in month
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) 
    {
        days_in_month[1] = 29; // Leap year
    }

    return day <= days_in_month[month - 1];
}

// Function to validate the time
int validate_time(int hour, int minute) 
{
    return hour >= 0 && hour < 24 && minute >= 0 && minute < 60;
}


// for numbers.
char *format_number(int number) 
{
    char temp[20]; 
    sprintf(temp, "%d", number); 

    int len = strlen(temp);
    int comma_count = (len - 1) / 3; 
    int new_len = len + comma_count;

    // Allocate memory for the formatted string (+1 for null terminator)
    char *buffer = (char *)malloc(new_len + 1); 
    if (buffer == NULL) 
    {
        perror("Memory allocation failed");
        return NULL;
    }

    buffer[new_len] = '\0'; // Null-terminate the string

    int j = new_len - 1; 
    for (int i = len - 1, counter = 0; i >= 0; i--, j--) 
    {
        buffer[j] = temp[i]; 
        counter++;
        if (counter == 3 && i > 0) 
        { 
            buffer[--j] = ','; // Insert a comma
            counter = 0;
        }
    }
    return buffer;
}

// 29 Dec 2024 21:42:36 format
int parse_date(const char *date_str, struct tm *tm_date) 
{
    if (tm_date == NULL || date_str == NULL) 
    {
        return -1; // Invalid arguments
    }

    memset(tm_date, 0, sizeof(struct tm));

    // Temporary buffer for the month name
    char month_str[4];

    // Parse components using sscanf
    if (sscanf(date_str, "%d %3s %d %d:%d:%d",
               &tm_date->tm_mday, 
               month_str,
               &tm_date->tm_year, 
               &tm_date->tm_hour, 
               &tm_date->tm_min, 
               &tm_date->tm_sec) != 6) {
        return -1; // Failed to parse the date
    }

    tm_date->tm_year -= 1900;

    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char *month_ptr = strstr(months, month_str);
    if (month_ptr == NULL) 
    {
        return -1; 
    }
    tm_date->tm_mon = (month_ptr - months) / 3;

    return 0; 
}

int validate_date_manual(int day, int month, int year) {
    if (year < 1900 || month < 1 || month > 12 || day < 1) {
        return 0;
    }

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
        days_in_month[1] = 29; // Leap year
    }

    return day <= days_in_month[month - 1];
}

int validate_time_manual(int hour, int minute, int second) {
    return (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59) && (second >= 0 && second <= 59);
}

int parse_date_manual(const char *date_str, char *formatted_date, char *formatted_time) {
    char day_str[3], month_str[4], year_str[5], hour_str[3], min_str[3], sec_str[3];
    const char *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    
    if (sscanf(date_str, "%2s %3s %4s %2s:%2s:%2s", day_str, month_str, year_str, hour_str, min_str, sec_str) != 6) {
        printf("\nError: Invalid date format. Please use 'DD Mon YYYY HH:MM:SS'.\n");
        return 0;
    }

    int day = atoi(day_str);
    int year = atoi(year_str);
    int hour = atoi(hour_str);
    int minute = atoi(min_str);
    int second = atoi(sec_str);

    // Find month as an index (0-11)
    const char *month_ptr = strstr(months, month_str);
    if (month_ptr == NULL) {
        printf("\nError: Invalid month name.\n");
        return 0;
    }
    int month = (month_ptr - months) / 3 + 1;

    if (!validate_date_manual(day, month, year)) {
        printf("\nError: Invalid date entered. Please check the values.\n");
        return 0;
    }

    if (!validate_time_manual(hour, minute, second)) {
        printf("\nError: Invalid time entered. Please check the values.\n");
        return 0;
    }

    snprintf(formatted_date, 20, "%04d-%02d-%02d", year, month, day);
    snprintf(formatted_time, 10, "%02d:%02d:%02d", hour, minute, second);
    return 1;
}

char *display_current_month_with_highlight() 
{
    const char *months[] = {"January", "February", "March", "April", "May", "June",
                            "July", "August", "September", "October", "November", "December"};
    const char *weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Get current time
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    int year = current_time->tm_year + 1900;
    int month = current_time->tm_mon + 1;
    int day_today = current_time->tm_mday;

    // Leap year adjustment
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        days_in_month[1] = 29;
    }

    // Get the day of the week for the first day of the month
    struct tm first_day = {0};
    first_day.tm_year = year - 1900;
    first_day.tm_mon = month - 1;
    first_day.tm_mday = 1;
    mktime(&first_day);

    int start_day = first_day.tm_wday;
    int total_days = days_in_month[month - 1];

    // Allocate memory for the result
    char *result = malloc(2048);
    if (!result) {
        perror("Memory allocation failed");
        return NULL;
    }
    result[0] = '\0';

    // Header
    char buffer[256];
    sprintf(buffer, "==================================================\n");
    strcat(result, buffer);
    sprintf(buffer, "                 📅 %s %d\n", months[month - 1], year);
    strcat(result, buffer);
    sprintf(buffer, "==================================================\n\n");
    strcat(result, buffer);

    // Weekdays
    for (int i = 0; i < 7; i++) {
        sprintf(buffer, "   %s ", weekdays[i]);
        strcat(result, buffer);
    }
    strcat(result, "\n--------------------------------------------------\n");

    // Leading spaces
    for (int i = 0; i < start_day; i++) {
        strcat(result, "       ");
    }

    // Days with today's date highlighted
    for (int day = 1; day <= total_days; day++) {
        if (day == day_today) {
            sprintf(buffer, "   \033[30;47m%3d\033[0m ", day);
        } else {
            sprintf(buffer, "   %3d ", day);
        }
        strcat(result, buffer);

        if ((start_day + day) % 7 == 0) {
            strcat(result, "\n");
        }
    }
    strcat(result, "\n\n==================================================\n");

    // Current time
    sprintf(buffer, "🕒 Today's Date: %02d %s %04d | Current Time: %02d:%02d:%02d\n",
            day_today, months[month - 1], year,
            current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
    strcat(result, buffer);

    return result;
}
