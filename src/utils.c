
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

