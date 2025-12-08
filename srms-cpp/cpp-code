// student_srms_c.c  — Simple C version of Student Record Management System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
    #include <conio.h>   // for getch()
#else
    // Fallback: no hidden input, use normal input
    #define getch() getchar()
#endif
// ----------------- Constants -----------------
#define CRED_FILE   "credentials.txt"
#define STUD_FILE   "students.txt"
#define MAX_ATTEMPTS 3
#define DELIM       '|'
#define MAX_STUDENTS 100
#define MAX_CREDS    20
#define MAX_NAME     100
#define MAX_USER     32
#define MAX_PASS     32
#define MAX_ROLE     16
#define LINE_BUF     256
// ----------------- Structs -----------------
typedef struct {
    int roll;
    char name[MAX_NAME];
    double cgpa;
} Student;
typedef struct {
    char username[MAX_USER];
    char password[MAX_PASS];
    char role[MAX_ROLE];   // "admin"
} Credential;
// ----------------- Global Data -----------------
Student students[MAX_STUDENTS];
int studentCount = 0;
Credential creds[MAX_CREDS];
int credCount = 0;
// ----------------- Utility Functions -----------------
void strip_newline(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}
void getHiddenPassword(char *buffer, int maxLen) {
    int idx = 0;
    int ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // ENTER
            break;
        } else if (ch == 8) { // BACKSPACE
            if (idx > 0) {
                idx--;
                printf("\b \b");
            }
        } else if (ch >= 32 && ch < 127) { // visible chars
            if (idx < maxLen - 1) {
                buffer[idx++] = (char)ch;
                putchar('*');
            }
        }
    }
    buffer[idx] = '\0';
    putchar('\n');
}
int fileEmptyOrMissing(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 1;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (size <= 0);
}
// ----------------- Credentials Handling -----------------
void ensureDefaultCredentials() {
    if (!fileEmptyOrMissing(CRED_FILE)) return;
    FILE *fp = fopen(CRED_FILE, "w");
    if (!fp) {
        printf("Error creating %s\n", CRED_FILE);
        return;
    }
    // default: admin secret123 admin
    fprintf(fp, "admin secret123 admin\n");
    fclose(fp);
}
void loadCredentials() {
    FILE *fp = fopen(CRED_FILE, "r");
    if (!fp) {
        credCount = 0;
        return;
    }
    credCount = 0;
    while (credCount < MAX_CREDS) {
        Credential c;
        if (fscanf(fp, "%31s %31s %15s", c.username, c.password, c.role) != 3)
            break;
        creds[credCount++] = c;
    }
    fclose(fp);
}
// ----------------- Student File Handling -----------------
void loadStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        // create empty file
        fp = fopen(STUD_FILE, "w");
        if (fp) fclose(fp);
        studentCount = 0;
        return;
    }
    char line[LINE_BUF];
    studentCount = 0;
    while (fgets(line, sizeof(line), fp)) {
        strip_newline(line);
        if (line[0] == '\0') continue;
        char *p1 = strchr(line, DELIM);
        char *p2 = strrchr(line, DELIM);
        if (!p1 || !p2 || p1 == p2) continue;
        *p1 = '\0';
        *p2 = '\0';
        char *rollStr = line;
        char *nameStr = p1 + 1;
        char *cgpaStr = p2 + 1;
        strip_newline(rollStr);
        strip_newline(nameStr);
        strip_newline(cgpaStr);
        int roll = atoi(rollStr);
        double cgpa = atof(cgpaStr);
        if (studentCount < MAX_STUDENTS) {
            students[studentCount].roll = roll;
            strncpy(students[studentCount].name, nameStr, MAX_NAME - 1);
            students[studentCount].name[MAX_NAME - 1] = '\0';
            students[studentCount].cgpa = cgpa;
            studentCount++;
        }
    }
    fclose(fp);
}
void saveStudents() {
    FILE *fp = fopen(STUD_FILE, "w");
    if (!fp) {
        printf("Error writing %s\n", STUD_FILE);
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        fprintf(fp, "%d|%s|%.2f\n",
                students[i].roll,
                students[i].name,
                students[i].cgpa);
    }
    fclose(fp);
}
void appendStudentToFile(const Student *s) {
    FILE *fp = fopen(STUD_FILE, "a");
    if (!fp) {
        printf("Error appending to %s\n", STUD_FILE);
        return;
    }
    fprintf(fp, "%d|%s|%.2f\n", s->roll, s->name, s->cgpa);
    fclose(fp);
}
// ----------------- Student Helpers -----------------
int findStudentIndexByRoll(int roll) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].roll == roll)
            return i;
    }
    return -1;
}
double inputCgpa(const char *prompt) {
    double val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &val) != 1) {
            printf("Invalid input. Enter a number between 1.00 and 10.00.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        if (val < 1.0 || val > 10.0) {
            printf("CGPA must be between 1.00 and 10.00.\n");
            continue;
        }
        // Round to two decimals
        val = (int)(val * 100 + 0.5) / 100.0;
        return val;
    }
}
void printTableHeader() {
    printf("%-8s%-30s%8s\n", "Roll", "Name", "CGPA");
    for (int i = 0; i < 8 + 30 + 8; i++) putchar('-');
    putchar('\n');
}
int compareStudents(const void *a, const void *b) {
    const Student *sa = (const Student *)a;
    const Student *sb = (const Student *)b;
    return (sa->roll - sb->roll);
}
void displayStudents() {
    if (studentCount == 0) {
        printf("No student records.\n");
        return;
    }
    Student temp[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) {
        temp[i] = students[i];
    }
    qsort(temp, studentCount, sizeof(Student), compareStudents);
    printTableHeader();
    for (int i = 0; i < studentCount; i++) {
        printf("%-8d%-30s%8.2f\n",
               temp[i].roll,
               temp[i].name,
               temp[i].cgpa);
    }
}
// ----------------- Admin Menu -----------------
void adminMenu() {
    while (1) {
        int choice;
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search by Roll\n");
        printf("4. Update Record\n");
        printf("5. Delete Record\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        if (choice == 1) {
            if (studentCount >= MAX_STUDENTS) {
                printf("Cannot add more students.\n");
                continue;
            }
            Student s;
            printf("Enter Roll Number: ");
            if (scanf("%d", &s.roll) != 1) {
                printf("Invalid roll.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);
            if (findStudentIndexByRoll(s.roll) != -1) {
                printf("Roll already exists.\n");
                continue;
            }
            printf("Enter Name (may contain spaces): ");
            if (!fgets(s.name, sizeof(s.name), stdin)) {
                printf("Error reading name.\n");
                continue;
            }
            strip_newline(s.name);
            s.cgpa = inputCgpa("Enter CGPA (1.00 - 10.00): ");
            students[studentCount++] = s;
            appendStudentToFile(&s);
            printf("Student record added successfully!\n");
        }
        else if (choice == 2) {
            displayStudents();
        }
        else if (choice == 3) {
            int roll;
            printf("Enter roll number to search: ");
            if (scanf("%d", &roll) != 1) {
                printf("Invalid roll.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);
            int idx = findStudentIndexByRoll(roll);
            if (idx == -1) {
                printf("Record not found.\n");
            } else {
                printTableHeader();
                printf("%-8d%-30s%8.2f\n",
                       students[idx].roll,
                       students[idx].name,
                       students[idx].cgpa);
            }
        }
        else if (choice == 4) {
            int roll;
            printf("Enter roll number to update: ");
            if (scanf("%d", &roll) != 1) {
                printf("Invalid roll.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);
            int idx = findStudentIndexByRoll(roll);
            if (idx == -1) {
                printf("Roll not found.\n");
            } else {
                printf("Updating Roll: %d (Current Name: %s)\n",
                       students[idx].roll, students[idx].name);
                char newName[MAX_NAME];
                printf("New Name (leave empty to keep current): ");
                if (!fgets(newName, sizeof(newName), stdin)) {
                    printf("Error reading name.\n");
                    continue;
                }
                strip_newline(newName);
                if (strlen(newName) > 0) {
                    strncpy(students[idx].name, newName, MAX_NAME - 1);
                    students[idx].name[MAX_NAME - 1] = '\0';
                }
                students[idx].cgpa = inputCgpa("New CGPA (1.00 - 10.00): ");
                saveStudents();
                printf("Record updated successfully.\n");
            }
        }
        else if (choice == 5) {
            int roll;
            printf("Enter roll number to delete: ");
            if (scanf("%d", &roll) != 1) {
                printf("Invalid roll.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);
            int idx = findStudentIndexByRoll(roll);
            if (idx == -1) {
                printf("Roll not found. No deletion.\n");
            } else {
                for (int i = idx; i < studentCount - 1; i++) {
                    students[i] = students[i + 1];
                }
                studentCount--;
                saveStudents();
                printf("Record deleted successfully. File rebuilt.\n");
            }
        }
        else if (choice == 6) {
            printf("Logging out...\n");
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }
}
// ----------------- Main -----------------
int main(void) {
    printf("==============================================\n");
    printf("Student Record Management System (CGPA Mode)\n");
    printf("==============================================\n");
    ensureDefaultCredentials();
    loadCredentials();
    loadStudents();
    int attempts = 0;
    int loggedIn = 0;
    char username[MAX_USER];
    char password[MAX_PASS];
    char role[MAX_ROLE] = "";
    while (attempts < MAX_ATTEMPTS && !loggedIn) {
        printf("\n--- Login ---\n");
        printf("USERNAME: ");
        if (scanf("%31s", username) != 1) return 0;
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("PASSWORD: ");
        getHiddenPassword(password, MAX_PASS);
        attempts++;
        for (int i = 0; i < credCount; i++) {
            if (strcmp(creds[i].username, username) == 0 &&
                strcmp(creds[i].password, password) == 0) {
                loggedIn = 1;
                strncpy(role, creds[i].role, MAX_ROLE - 1);
                role[MAX_ROLE - 1] = '\0';
                break;
            }
        }
        if (loggedIn) {
            printf("Login successful!\n");
            printf("Logged in as: %s\n", role);
        } else if (attempts < MAX_ATTEMPTS) {
            printf("Incorrect login. Attempt %d of %d.\n",
                   attempts, MAX_ATTEMPTS);
        } else {
            printf("Incorrect login. No attempts left. Exiting.\n");
        }
    }
    if (!loggedIn) return 0;
    // Only admin mode is used here
    adminMenu();
    printf("Program terminated. Goodbye!\n");
    return 0;
}
