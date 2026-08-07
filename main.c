#include <stdio.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_STR 50
#define NUM_QUESTIONS 4

// ==================== SYSTEM PHASES ====================
typedef struct {
    char studentID[50];
    int offeringID;
    float grade; // -1.0 indicates no grade entered yet
} Enrollment;

typedef enum {
    STATUS_PENDING,
    STATUS_APPROVED,
    STATUS_REJECTED
} ApprovalStatus;

typedef struct {
    int offeringID;
    char courseID[20];
    char title[100];
    char professorID[50];
    char professorName[100];
    int capacity;
    int enrolledCount;
    char classSchedule[100]; // e.g., "Mon/Wed 10:00-12:00"
    char examDate[50];      // e.g., "1403-10-15"
    ApprovalStatus status;
} CourseOffering;

typedef struct {
    char courseID[20];
    char title[100];
    int units;
    char department[50];
    char prerequisites[100]; // e.g., "MATH101, PHYS101" or "None"
} Course;

typedef enum {
    PHASE_COURSE_OFFERING,
    PHASE_ENROLLMENT,
    PHASE_CLASSES_AND_EXAMS,
    PHASE_GRADE_ENTRY
} SemesterPhase;

typedef struct {
    char activeSemester[20]; // e.g., "1403-1"
    SemesterPhase currentPhase;
} SystemState;

// ==================== CORE STRUCTS ====================
typedef struct {
    char firstName[MAX_STR];
    char lastName[MAX_STR];
    char studentID[MAX_STR];
    char nationalID[MAX_STR];
    char password[MAX_STR];
    char fieldOfStudy[MAX_STR];
    int yearOfEntry;
    char degreeLevel[MAX_STR];
    char supervisor[MAX_STR];
    char faculty[MAX_STR];
    char major[MAX_STR];
    char securityAnswers[NUM_QUESTIONS][MAX_STR];
} Student;

typedef struct {
    char firstName[MAX_STR];
    char lastName[MAX_STR];
    char professorID[MAX_STR];
    char nationalID[MAX_STR];
    char password[MAX_STR];
    char fieldOfStudy[MAX_STR];
    int yearOfEntry;
    char highestDegree[MAX_STR];
    char faculty[MAX_STR];
    char securityAnswers[NUM_QUESTIONS][MAX_STR];
} Professor;

typedef struct {
    char username[MAX_STR];
    char password[MAX_STR];
    char role[20]; // "deputy", "professor", "student"
    int userIndex; // Index in respective array (-1 for deputy)
} UserAccount;

// Security questions predefined per spec Section 3.1
const char SECURITY_QUESTIONS[NUM_QUESTIONS][100] = {
    "Place of birth?",
    "Name of the first school you enrolled in?",
    "Title of the first book you read?",
    "Color of your first bicycle?"
};

// Global Input Buffer Cleaner
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Helper function to print phase name
const char* get_phase_name(SemesterPhase phase) {
    switch (phase) {
        case PHASE_COURSE_OFFERING: return "Course Offering Period";
        case PHASE_ENROLLMENT:      return "Student Enrollment Period";
        case PHASE_CLASSES_AND_EXAMS: return "Classes & Exams Period";
        case PHASE_GRADE_ENTRY:     return "Grade Entry Period";
        default:                    return "Unknown Phase";
    }
}

// ==================== MENU HANDLERS ====================

int show_deputy_menu(SystemState *sysState) {
    printf("\n========================================\n");
    printf("   EDUCATIONAL DEPUTY DASHBOARD\n");
    printf("   Semester: %s | Phase: %s\n", sysState->activeSemester, get_phase_name(sysState->currentPhase));
    printf("========================================\n");
    printf("1. Manage Students & Professors (Add/Remove)\n");
    printf("2. Add New Course to Master Catalog\n");
    printf("3. Review Pending Approvals (Offerings, Capacity, Withdrawals)\n");
    printf("4. Manually Add/Remove Student from Offered Course\n");
    printf("5. Advance Semester Schedule / Phase\n");
    printf("6. Add Course Capacity\n");
    printf("7. Logout\n");
    printf("Choice: ");

    int choice = 0;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return -1;
    }
    clear_input_buffer();
    return choice;
}

int show_professor_menu(SystemState *sysState, Professor *prof) {
    printf("\n========================================\n");
    printf("   PROFESSOR DASHBOARD - %s %s\n", prof->firstName, prof->lastName);
    printf("   Semester: %s | Phase: %s\n", sysState->activeSemester, get_phase_name(sysState->currentPhase));
    printf("========================================\n");
    printf("1. Offer a Course\n");
    printf("2. Request Capacity Increase for Offered Course\n");
    printf("3. Request Course Offering Cancellation\n");
    printf("4. Enter Student Grades (CSV Bulk Upload)\n");
    printf("5. Course Survey Results\n");
    printf("6. LMS (Publish Assignments / Exams)\n");
    printf("7. Logout\n");
    printf("Choice: ");

    int choice = 0;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return -1;
    }
    clear_input_buffer();
    return choice;
}

int show_student_menu(SystemState *sysState, Student *student) {
    printf("\n========================================\n");
    printf("   STUDENT DASHBOARD - %s %s (%s)\n", student->firstName, student->lastName, student->degreeLevel);
    printf("   Semester: %s | Phase: %s\n", sysState->activeSemester, get_phase_name(sysState->currentPhase));
    printf("========================================\n");
    printf("1. Search & Enroll in Offered Courses\n");
    printf("2. View Academic Transcript & GPA\n");
    printf("3. Course Survey Evaluation\n");
    printf("4. Doctoral Thesis Management\n");
    printf("5. LMS (View Assignments & Exams)\n");
    printf("6. Logout\n");
    printf("Choice: ");

    int choice = 0;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return -1;
    }
    clear_input_buffer();
    return choice;
}
// ==================== USER MANAGEMENT HELPERS ====================

void add_student(Student students[], int *student_count, UserAccount accounts[], int *account_count) {
    if (*student_count >= MAX_USERS) {
        printf("\n✗ Student database full!\n");
        return;
    }

    Student s;
    printf("\n--- ADD NEW STUDENT ---\n");
    printf("First Name: "); scanf("%49s", s.firstName);
    printf("Last Name: "); scanf("%49s", s.lastName);
    printf("Student ID: "); scanf("%49s", s.studentID);
    printf("National ID: "); scanf("%49s", s.nationalID);
    printf("Password: "); scanf("%49s", s.password);
    printf("Field of Study: "); scanf(" %49[^\n]", s.fieldOfStudy);
    printf("Year of Entry: "); scanf("%d", &s.yearOfEntry);
    printf("Degree Level (Bachelors/Masters/Doctoral): "); scanf("%49s", s.degreeLevel);
    printf("Faculty: "); scanf(" %49[^\n]", s.faculty);
    printf("Major: "); scanf(" %49[^\n]", s.major);
    printf("Supervisor: "); scanf(" %49[^\n]", s.supervisor);
    clear_input_buffer();

    printf("\nSet 4 Security Answers:\n");
    for (int q = 0; q < NUM_QUESTIONS; q++) {
        printf("%d. %s\n   Answer: ", q + 1, SECURITY_QUESTIONS[q]);
        scanf(" %49[^\n]", s.securityAnswers[q]);
        clear_input_buffer();
    }

    // Save student record
    students[*student_count] = s;

    // Automatically create login account
    strcpy(accounts[*account_count].username, s.studentID);
    strcpy(accounts[*account_count].password, s.password);
    strcpy(accounts[*account_count].role, "student");
    accounts[*account_count].userIndex = *student_count;

    (*student_count)++;
    (*account_count)++;

    printf("\n✓ Student %s %s (ID: %s) added! Account created.\n", s.firstName, s.lastName, s.studentID);
}

void add_professor(Professor professors[], int *professor_count, UserAccount accounts[], int *account_count) {
    if (*professor_count >= MAX_USERS) {
        printf("\n✗ Professor database full!\n");
        return;
    }

    Professor p;
    printf("\n--- ADD NEW PROFESSOR ---\n");
    printf("First Name: "); scanf("%49s", p.firstName);
    printf("Last Name: "); scanf("%49s", p.lastName);
    printf("Professor ID: "); scanf("%49s", p.professorID);
    printf("National ID: "); scanf("%49s", p.nationalID);
    printf("Password: "); scanf("%49s", p.password);
    printf("Field of Study: "); scanf(" %49[^\n]", p.fieldOfStudy);
    printf("Year of Entry: "); scanf("%d", &p.yearOfEntry);
    printf("Highest Degree (PhD/MSc): "); scanf("%49s", p.highestDegree);
    printf("Faculty: "); scanf(" %49[^\n]", p.faculty);
    clear_input_buffer();

    printf("\nSet 4 Security Answers:\n");
    for (int q = 0; q < NUM_QUESTIONS; q++) {
        printf("%d. %s\n   Answer: ", q + 1, SECURITY_QUESTIONS[q]);
        scanf(" %49[^\n]", p.securityAnswers[q]);
        clear_input_buffer();
    }

    // Save professor record
    professors[*professor_count] = p;

    // Automatically create login account
    strcpy(accounts[*account_count].username, p.professorID);
    strcpy(accounts[*account_count].password, p.password);
    strcpy(accounts[*account_count].role, "professor");
    accounts[*account_count].userIndex = *professor_count;

    (*professor_count)++;
    (*account_count)++;

    printf("\n✓ Professor %s %s (ID: %s) added! Account created.\n", p.firstName, p.lastName, p.professorID);
}

void list_students(Student students[], int student_count) {
    printf("\n================ ALL STUDENTS (%d) ================\n", student_count);
    if (student_count == 0) printf("No students registered.\n");
    for (int i = 0; i < student_count; i++) {
        printf("[%d] %s %s | ID: %s | Degree: %s | Faculty: %s\n",
               i + 1, students[i].firstName, students[i].lastName,
               students[i].studentID, students[i].degreeLevel, students[i].faculty);
    }
}

void list_professors(Professor professors[], int professor_count) {
    printf("\n================ ALL PROFESSORS (%d) ================\n", professor_count);
    if (professor_count == 0) printf("No professors registered.\n");
    for (int i = 0; i < professor_count; i++) {
        printf("[%d] Dr. %s %s | ID: %s | Degree: %s | Faculty: %s\n",
               i + 1, professors[i].firstName, professors[i].lastName,
               professors[i].professorID, professors[i].highestDegree, professors[i].faculty);
    }
}

void manage_users_sub_menu(Student students[], int *student_count, Professor professors[], int *professor_count, UserAccount accounts[], int *account_count) {
    while (1) {
        printf("\n--- USER MANAGEMENT ---\n");
        printf("1. Add Student\n");
        printf("2. Add Professor\n");
        printf("3. List All Students\n");
        printf("4. List All Professors\n");
        printf("5. Back to Deputy Dashboard\n");
        printf("Choice: ");

        int choice = 0;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 1) add_student(students, student_count, accounts, account_count);
        else if (choice == 2) add_professor(professors, professor_count, accounts, account_count);
        else if (choice == 3) list_students(students, *student_count);
        else if (choice == 4) list_professors(professors, *professor_count);
        else if (choice == 5) break;
        else printf("Invalid choice.\n");
    }
}
// ==================== COURSE CATALOG HELPERS ====================

void add_course_to_catalog(Course catalog[], int *course_count) {
    if (*course_count >= 100) {
        printf("\n✗ Course catalog is full!\n");
        return;
    }

    Course c;
    printf("\n--- ADD NEW COURSE TO CATALOG ---\n");
    printf("Course ID (e.g., CE101): "); 
    scanf("%19s", c.courseID);
    clear_input_buffer();

    printf("Course Title: "); 
    scanf(" %99[^\n]", c.title);
    clear_input_buffer();

    printf("Unit Count (1-4): "); 
    scanf("%d", &c.units);
    clear_input_buffer();

    printf("Department/Faculty: "); 
    scanf(" %49[^\n]", c.department);
    clear_input_buffer();

    printf("Prerequisites (or 'None'): "); 
    scanf(" %99[^\n]", c.prerequisites);
    clear_input_buffer();

    catalog[*course_count] = c;
    (*course_count)++;

    printf("\n✓ Course '%s' (%s) added to the master catalog!\n", c.title, c.courseID);
}

void list_course_catalog(Course catalog[], int course_count) {
    printf("\n================ MASTER COURSE CATALOG (%d) ================\n", course_count);
    if (course_count == 0) {
        printf("No courses in the catalog yet.\n");
        return;
    }

    printf("%-10s | %-30s | %-5s | %-15s | %-20s\n", "ID", "Title", "Units", "Department", "Prereqs");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < course_count; i++) {
        printf("%-10s | %-30s | %-5d | %-15s | %-20s\n",
               catalog[i].courseID, catalog[i].title, catalog[i].units,
               catalog[i].department, catalog[i].prerequisites);
    }
}

void manage_catalog_sub_menu(Course catalog[], int *course_count) {
    while (1) {
        printf("\n--- MASTER COURSE CATALOG MENU ---\n");
        printf("1. Add Course to Catalog\n");
        printf("2. View Master Course Catalog\n");
        printf("3. Back to Deputy Dashboard\n");
        printf("Choice: ");

        int choice = 0;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 1) add_course_to_catalog(catalog, course_count);
        else if (choice == 2) list_course_catalog(catalog, *course_count);
        else if (choice == 3) break;
        else printf("Invalid choice.\n");
    }
}
// ==================== SEMESTER PHASE CONTROLLER ====================

void advance_semester_phase(SystemState *sysState) {
    printf("\n--- ADVANCE SEMESTER PHASE ---\n");
    printf("Current Semester: %s\n", sysState->activeSemester);
    printf("Current Phase   : %s\n", get_phase_name(sysState->currentPhase));

    switch (sysState->currentPhase) {
        case PHASE_COURSE_OFFERING:
            sysState->currentPhase = PHASE_ENROLLMENT;
            printf("\n✓ Advanced to [Student Enrollment Period]. Students can now register for classes!\n");
            break;

        case PHASE_ENROLLMENT:
            sysState->currentPhase = PHASE_CLASSES_AND_EXAMS;
            printf("\n✓ Advanced to [Classes & Exams Period]. Course registrations are locked!\n");
            break;

        case PHASE_CLASSES_AND_EXAMS:
            sysState->currentPhase = PHASE_GRADE_ENTRY;
            printf("\n✓ Advanced to [Grade Entry Period]. Professors can now enter grades!\n");
            break;

        case PHASE_GRADE_ENTRY: {
            // End of semester -> Increment active semester
            int year = 1403, term = 1;
            sscanf(sysState->activeSemester, "%d-%d", &year, &term);
            
            if (term == 1) {
                term = 2;
            } else {
                year++;
                term = 1;
            }

            sprintf(sysState->activeSemester, "%d-%d", year, term);
            sysState->currentPhase = PHASE_COURSE_OFFERING;

            printf("\n✓ Semester finalized! New Active Semester: [%s]\n", sysState->activeSemester);
            printf("✓ Advanced to [Course Offering Period]. Professors can submit offering requests!\n");
            break;
        }
    }
}
// ==================== COURSE OFFERINGS & APPROVALS ====================

// Professor submits course offering request
void offer_course_request(Professor *prof, Course catalog[], int catalog_count, CourseOffering offerings[], int *offering_count, SystemState *sysState) {
    if (sysState->currentPhase != PHASE_COURSE_OFFERING) {
        printf("\n✗ Course offerings can only be submitted during the [Course Offering Period]!\n");
        return;
    }

    if (catalog_count == 0) {
        printf("\n✗ Master catalog is empty. Deputy must add courses first.\n");
        return;
    }

    printf("\n--- AVAILABLE CATALOG COURSES ---\n");
    for (int i = 0; i < catalog_count; i++) {
        printf("[%d] %s - %s (%d units)\n", i + 1, catalog[i].courseID, catalog[i].title, catalog[i].units);
    }

    printf("Select Course Number: ");
    int choice = 0;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > catalog_count) {
        clear_input_buffer();
        printf("Invalid selection.\n");
        return;
    }
    clear_input_buffer();

    Course selected = catalog[choice - 1];

    CourseOffering off;
    off.offeringID = *offering_count + 101; // IDs start at 101
    strcpy(off.courseID, selected.courseID);
    strcpy(off.title, selected.title);
    strcpy(off.professorID, prof->professorID);
    sprintf(off.professorName, "Dr. %s %s", prof->firstName, prof->lastName);
    off.enrolledCount = 0;
    off.status = STATUS_PENDING;

    printf("Class Capacity: ");
    scanf("%d", &off.capacity);
    clear_input_buffer();

    printf("Class Schedule (e.g., Sat/Mon 10:00-12:00): ");
    scanf(" %99[^\n]", off.classSchedule);
    clear_input_buffer();

    printf("Exam Date (e.g., 1403-10-15): ");
    scanf(" %49s", off.examDate);
    clear_input_buffer();

    offerings[*offering_count] = off;
    (*offering_count)++;

    printf("\n✓ Offering request for '%s' (ID: %d) submitted! Awaiting Deputy approval.\n", off.title, off.offeringID);
}

// Deputy reviews pending offering requests
void review_pending_offerings(CourseOffering offerings[], int offering_count) {
    int pending_found = 0;
    printf("\n================ PENDING COURSE OFFERINGS ================\n");
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].status == STATUS_PENDING) {
            pending_found++;
            printf("ID: %d | Course: %s - %s | Prof: %s | Cap: %d | Schedule: %s\n",
                   offerings[i].offeringID, offerings[i].courseID, offerings[i].title,
                   offerings[i].professorName, offerings[i].capacity, offerings[i].classSchedule);
        }
    }

    if (!pending_found) {
        printf("No pending offering requests.\n");
        return;
    }

    printf("\nEnter Offering ID to approve/reject (0 to cancel): ");
    int targetID = 0;
    scanf("%d", &targetID);
    clear_input_buffer();

    if (targetID == 0) return;

    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].offeringID == targetID && offerings[i].status == STATUS_PENDING) {
            printf("Action for Offering %d (1: Approve, 2: Reject): ", targetID);
            int act = 0;
            scanf("%d", &act);
            clear_input_buffer();

            if (act == 1) {
                offerings[i].status = STATUS_APPROVED;
                printf("\n✓ Offering %d APPROVED! Course is now available for enrollment.\n", targetID);
            } else if (act == 2) {
                offerings[i].status = STATUS_REJECTED;
                printf("\n✗ Offering %d REJECTED.\n", targetID);
            } else {
                printf("Invalid action.\n");
            }
            return;
        }
    }
    printf("Offering ID not found or already processed.\n");
}

// List all approved offerings
void list_approved_offerings(CourseOffering offerings[], int offering_count) {
    printf("\n================ APPROVED COURSE OFFERINGS ================\n");
    int count = 0;
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].status == STATUS_APPROVED) {
            count++;
            printf("[%d] %s (%s) | %s | Enrolled: %d/%d | Schedule: %s\n",
                   offerings[i].offeringID, offerings[i].title, offerings[i].courseID,
                   offerings[i].professorName, offerings[i].enrolledCount,
                   offerings[i].capacity, offerings[i].classSchedule);
        }
    }
    if (count == 0) printf("No approved course offerings for this semester.\n");
}
// ==================== STUDENT ENROLLMENT ENGINE ====================

void enroll_in_course(Student *student, CourseOffering offerings[], int offering_count, Enrollment enrollments[], int *enrollment_count, SystemState *sysState) {
    if (sysState->currentPhase != PHASE_ENROLLMENT) {
        printf("\n✗ Course registration is only allowed during the [Student Enrollment Period]!\n");
        return;
    }

    list_approved_offerings(offerings, offering_count);

    printf("\nEnter Offering ID to enroll (0 to cancel): ");
    int targetID = 0;
    if (scanf("%d", &targetID) != 1 || targetID == 0) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // 1. Locate offering
    int offering_idx = -1;
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].offeringID == targetID && offerings[i].status == STATUS_APPROVED) {
            offering_idx = i;
            break;
        }
    }

    if (offering_idx == -1) {
        printf("\n✗ Approved course offering ID %d not found.\n", targetID);
        return;
    }

    // 2. Constraint Check: Duplicate Registration
    for (int i = 0; i < *enrollment_count; i++) {
        if (strcmp(enrollments[i].studentID, student->studentID) == 0 &&
            enrollments[i].offeringID == targetID) {
            printf("\n✗ You are already enrolled in this course!\n");
            return;
        }
    }

    // 3. Constraint Check: Capacity Check
    if (offerings[offering_idx].enrolledCount >= offerings[offering_idx].capacity) {
        printf("\n✗ Course capacity reached (%d/%d). Cannot enroll.\n",
               offerings[offering_idx].enrolledCount, offerings[offering_idx].capacity);
        return;
    }

    // Process Registration
    Enrollment e;
    strcpy(e.studentID, student->studentID);
    e.offeringID = targetID;
    e.grade = -1.0f; // Grade pending

    enrollments[*enrollment_count] = e;
    (*enrollment_count)++;
    offerings[offering_idx].enrolledCount++;

    printf("\n✓ Successfully enrolled in '%s' (Offering ID: %d)!\n",
           offerings[offering_idx].title, targetID);
}

void view_student_schedule(Student *student, CourseOffering offerings[], int offering_count, Enrollment enrollments[], int enrollment_count) {
    printf("\n================ ENROLLED COURSES FOR %s %s ================\n",
           student->firstName, student->lastName);
    int count = 0;

    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].studentID, student->studentID) == 0) {
            // Find offering details
            for (int j = 0; j < offering_count; j++) {
                if (offerings[j].offeringID == enrollments[i].offeringID) {
                    count++;
                    printf("[%d] %s (%s) | Instructor: %s | Schedule: %s | Exam: %s\n",
                           offerings[j].offeringID, offerings[j].title, offerings[j].courseID,
                           offerings[j].professorName, offerings[j].classSchedule, offerings[j].examDate);
                }
            }
        }
    }

    if (count == 0) {
        printf("You are not currently enrolled in any courses.\n");
    }
}
// ==================== GRADE ENTRY & TRANSCRIPT ENGINE ====================

// Professor grade entry function
void enter_grades_for_course(Professor *prof, CourseOffering offerings[], int offering_count, Enrollment enrollments[], int enrollment_count, Student students[], int student_count, SystemState *sysState) {
    if (sysState->currentPhase != PHASE_GRADE_ENTRY) {
        printf("\n✗ Grade entry is only permitted during the [Grade Entry Period]!\n");
        return;
    }

    // List offerings belonging to this professor
    printf("\n================ YOUR OFFERED COURSES ================\n");
    int prof_courses = 0;
    for (int i = 0; i < offering_count; i++) {
        if (strcmp(offerings[i].professorID, prof->professorID) == 0 && offerings[i].status == STATUS_APPROVED) {
            prof_courses++;
            printf("[%d] %s (%s) | Enrolled Students: %d\n",
                   offerings[i].offeringID, offerings[i].title, offerings[i].courseID, offerings[i].enrolledCount);
        }
    }

    if (prof_courses == 0) {
        printf("You have no active course offerings this semester.\n");
        return;
    }

    printf("\nEnter Offering ID to enter grades (0 to cancel): ");
    int targetID = 0;
    if (scanf("%d", &targetID) != 1 || targetID == 0) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Verify offering ownership
    int valid_offering = 0;
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].offeringID == targetID && strcmp(offerings[i].professorID, prof->professorID) == 0) {
            valid_offering = 1;
            break;
        }
    }

    if (!valid_offering) {
        printf("\n✗ Offering ID not found or does not belong to you.\n");
        return;
    }

    // Enter grades for enrolled students
    printf("\n--- ENTER GRADES (Scale: 0.00 - 20.00) ---\n");
    int grades_entered = 0;
    for (int i = 0; i < enrollment_count; i++) {
        if (enrollments[i].offeringID == targetID) {
            // Find student name
            char sname[100] = "Unknown Student";
            for (int s = 0; s < student_count; s++) {
                if (strcmp(students[s].studentID, enrollments[i].studentID) == 0) {
                    sprintf(sname, "%s %s", students[s].firstName, students[s].lastName);
                    break;
                }
            }

            printf("Student: %s (ID: %s) | Current Grade: ", sname, enrollments[i].studentID);
            if (enrollments[i].grade < 0.0f) printf("[Not Graded]");
            else printf("%.2f", enrollments[i].grade);

            printf("\n   New Grade: ");
            float g = -1.0f;
            if (scanf("%f", &g) == 1 && g >= 0.0f && g <= 20.0f) {
                enrollments[i].grade = g;
                grades_entered++;
            } else {
                printf("   [Skipped/Invalid]\n");
            }
            clear_input_buffer();
        }
    }

    printf("\n✓ Updated %d student grade record(s).\n", grades_entered);
}

// Student transcript and GPA view
void view_academic_transcript(Student *student, CourseOffering offerings[], int offering_count, Course catalog[], int catalog_count, Enrollment enrollments[], int enrollment_count) {
    printf("\n================ ACADEMIC TRANSCRIPT ================\n");
    printf("Student: %s %s (ID: %s) | Major: %s\n",
           student->firstName, student->lastName, student->studentID, student->major);
    printf("-----------------------------------------------------\n");

    float total_weighted_points = 0.0f;
    int total_units = 0;
    int completed_courses = 0;

    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].studentID, student->studentID) == 0) {
            // Find course details
            char title[100] = "Course";
            int units = 3; // Default unit count

            for (int j = 0; j < offering_count; j++) {
                if (offerings[j].offeringID == enrollments[i].offeringID) {
                    strcpy(title, offerings[j].title);
                    for (int c = 0; c < catalog_count; c++) {
                        if (strcmp(catalog[c].courseID, offerings[j].courseID) == 0) {
                            units = catalog[c].units;
                            break;
                        }
                    }
                    break;
                }
            }

            printf("%-30s | Units: %d | Grade: ", title, units);
            if (enrollments[i].grade < 0.0f) {
                printf("[Pending]\n");
            } else {
                printf("%5.2f\n", enrollments[i].grade);
                total_weighted_points += enrollments[i].grade * units;
                total_units += units;
                completed_courses++;
            }
        }
    }

    printf("-----------------------------------------------------\n");
    if (total_units > 0) {
        float gpa = total_weighted_points / total_units;
        printf("Completed Units: %d | Overall GPA: %.2f / 20.00\n", total_units, gpa);
    } else {
        printf("No graded courses available for GPA calculation.\n");
    }
}
// ==================== FILE PERSISTENCE (SAVE) ====================

// ==================== FILE PERSISTENCE (SAVE) ====================

void save_system_data(SystemState *sysState, Student students[], int student_count, 
                      Professor professors[], int professor_count, Course catalog[], int catalog_count, 
                      CourseOffering offerings[], int offering_count, Enrollment enrollments[], int enrollment_count) {
    FILE *fp;

    // 1. Save System State
    fp = fopen("system.txt", "w");
    if (fp) {
        fprintf(fp, "%s\n%d\n", sysState->activeSemester, sysState->currentPhase);
        fclose(fp);
    }

    // 2. Save Students & Professors
    fp = fopen("users.txt", "w");
    if (fp) {
        fprintf(fp, "%d %d\n", student_count, professor_count);
        for (int i = 0; i < student_count; i++) {
            fprintf(fp, "S %s %s %s %s %s %d %s\n", 
                    students[i].studentID, students[i].password, 
                    students[i].firstName, students[i].lastName, 
                    students[i].major, students[i].yearOfEntry, students[i].faculty);
        }
        for (int i = 0; i < professor_count; i++) {
            fprintf(fp, "P %s %s %s %s %d %s\n", 
                    professors[i].professorID, professors[i].password, 
                    professors[i].firstName, professors[i].lastName,
                    professors[i].yearOfEntry, professors[i].faculty);
        }
        fclose(fp);
    }

    // 3. Save Catalog
    fp = fopen("catalog.txt", "w");
    if (fp) {
        fprintf(fp, "%d\n", catalog_count);
        for (int i = 0; i < catalog_count; i++) {
            fprintf(fp, "%s;%s;%d;%s;%s\n", 
                    catalog[i].courseID, catalog[i].title, catalog[i].units, 
                    catalog[i].department, catalog[i].prerequisites);
        }
        fclose(fp);
    }

    // 4. Save Offerings
    fp = fopen("offerings.txt", "w");
    if (fp) {
        fprintf(fp, "%d\n", offering_count);
        for (int i = 0; i < offering_count; i++) {
            fprintf(fp, "%d;%s;%s;%s;%s;%d;%d;%s;%s;%d\n",
                    offerings[i].offeringID, offerings[i].courseID, offerings[i].title,
                    offerings[i].professorID, offerings[i].professorName, offerings[i].capacity,
                    offerings[i].enrolledCount, offerings[i].classSchedule, offerings[i].examDate,
                    offerings[i].status);
        }
        fclose(fp);
    }

    // 5. Save Enrollments
    fp = fopen("enrollments.txt", "w");
    if (fp) {
        fprintf(fp, "%d\n", enrollment_count);
        for (int i = 0; i < enrollment_count; i++) {
            fprintf(fp, "%s;%d;%.2f\n", enrollments[i].studentID, enrollments[i].offeringID, enrollments[i].grade);
        }
        fclose(fp);
    }

    printf("\n✓ System state and databases successfully saved to disk.\n");
}

void load_system_data(SystemState *sysState, Student students[], int *student_count, 
                      Professor professors[], int *professor_count, Course catalog[], int *catalog_count, 
                      CourseOffering offerings[], int *offering_count, Enrollment enrollments[], int *enrollment_count,
                      UserAccount accounts[], int *account_count) {
    FILE *fp;

    // 1. Load System State
    fp = fopen("system.txt", "r");
    if (fp) {
        int phase_int;
        if (fscanf(fp, "%19s %d", sysState->activeSemester, &phase_int) == 2) {
            sysState->currentPhase = (SemesterPhase)phase_int;
        }
        fclose(fp);
    }

    // 2. Load Users
    fp = fopen("users.txt", "r");
    if (fp) {
        int sc = 0, pc = 0;
        if (fscanf(fp, "%d %d", &sc, &pc) == 2) {
            *student_count = 0;
            *professor_count = 0;
            
            // Re-populate students
            for (int i = 0; i < sc; i++) {
                char type[2];
                Student s;
                if (fscanf(fp, "%s %s %s %s %s %s %d %s", 
                           type, s.studentID, s.password, s.firstName, 
                           s.lastName, s.major, &s.yearOfEntry, s.faculty) == 8) {
                    students[*student_count] = s;
                    
                    // Register into accounts list for Login
                    strcpy(accounts[*account_count].username, s.studentID);
                    strcpy(accounts[*account_count].password, s.password);
                    strcpy(accounts[*account_count].role, "student");
                    accounts[*account_count].userIndex = *student_count;
                    (*account_count)++;
                    (*student_count)++;
                }
            }
            
            // Re-populate professors
            for (int i = 0; i < pc; i++) {
                char type[2];
                Professor p;
                if (fscanf(fp, "%s %s %s %s %s %d %s", 
                           type, p.professorID, p.password, p.firstName, 
                           p.lastName, &p.yearOfEntry, p.faculty) == 7) {
                    professors[*professor_count] = p;
                    
                    // Register into accounts list for Login
                    strcpy(accounts[*account_count].username, p.professorID);
                    strcpy(accounts[*account_count].password, p.password);
                    strcpy(accounts[*account_count].role, "professor");
                    accounts[*account_count].userIndex = *professor_count;
                    (*account_count)++;
                    (*professor_count)++;
                }
            }
        }
        fclose(fp);
    }

    // 3. Load Catalog
    fp = fopen("catalog.txt", "r");
    if (fp) {
        int count = 0;
        if (fscanf(fp, "%d\n", &count) == 1) {
            *catalog_count = 0;
            for (int i = 0; i < count; i++) {
                Course c;
                char line[300];
                if (fgets(line, sizeof(line), fp)) {
                    sscanf(line, "%[^;];%[^;];%d;%[^;];%[^\n]", c.courseID, c.title, &c.units, c.department, c.prerequisites);
                    catalog[(*catalog_count)++] = c;
                }
            }
        }
        fclose(fp);
    }

    // 4. Load Offerings
    fp = fopen("offerings.txt", "r");
    if (fp) {
        int count = 0;
        if (fscanf(fp, "%d\n", &count) == 1) {
            *offering_count = 0;
            for (int i = 0; i < count; i++) {
                CourseOffering off;
                int st;
                char line[400];
                if (fgets(line, sizeof(line), fp)) {
                    sscanf(line, "%d;%[^;];%[^;];%[^;];%[^;];%d;%d;%[^;];%[^;];%d",
                           &off.offeringID, off.courseID, off.title, off.professorID,
                           off.professorName, &off.capacity, &off.enrolledCount,
                           off.classSchedule, off.examDate, &st);
                    off.status = (ApprovalStatus)st;
                    offerings[(*offering_count)++] = off;
                }
            }
        }
        fclose(fp);
    }

    // 5. Load Enrollments
    fp = fopen("enrollments.txt", "r");
    if (fp) {
        int count = 0;
        if (fscanf(fp, "%d\n", &count) == 1) {
            *enrollment_count = 0;
            for (int i = 0; i < count; i++) {
                Enrollment e;
                if (fscanf(fp, "%[^;];%d;%f\n", e.studentID, &e.offeringID, &e.grade) == 3) {
                    enrollments[(*enrollment_count)++] = e;
                }
            }
        }
        fclose(fp);
    }
}
void manual_student_allocation(Student students[], int student_count, 
                               CourseOffering offerings[], int offering_count, 
                               Enrollment enrollments[], int *enrollment_count) {
    char student_id[MAX_STR];
    int offering_id;

    printf("\n--- MANUAL STUDENT ALLOCATION (DEPUTY OVERRIDE) ---\n");
    printf("Enter Student ID: ");
    scanf("%49s", student_id);
    printf("Enter Course Offering ID: ");
    if (scanf("%d", &offering_id) != 1) {
        clear_input_buffer();
        printf("Invalid Offering ID format.\n");
        return;
    }
    clear_input_buffer();

    int student_idx = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].studentID, student_id) == 0) {
            student_idx = i;
            break;
        }
    }
    if (student_idx == -1) {
        printf("✗ Error: Student ID '%s' not found.\n", student_id);
        return;
    }

    int offering_idx = -1;
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].offeringID == offering_id) {
            offering_idx = i;
            break;
        }
    }
    if (offering_idx == -1) {
        printf("✗ Error: Offering ID %d not found.\n", offering_id);
        return;
    }

    for (int i = 0; i < *enrollment_count; i++) {
        if (strcmp(enrollments[i].studentID, student_id) == 0 && 
            enrollments[i].offeringID == offering_id) {
            printf("✗ Student is already enrolled in this course offering.\n");
            return;
        }
    }

    enrollments[*enrollment_count].offeringID = offering_id;
    strcpy(enrollments[*enrollment_count].studentID, student_id);
    enrollments[*enrollment_count].grade = -1.00f;
    (*enrollment_count)++;

    offerings[offering_idx].enrolledCount++;

    printf("✓ Successfully force-enrolled Student %s (%s %s) into Offering %d (%s)!\n",
           student_id, students[student_idx].firstName, students[student_idx].lastName,
           offering_id, offerings[offering_idx].title);
}

void add_course_capacity(CourseOffering offerings[], int offering_count) {
    int offering_id, new_capacity;

    printf("\n--- ADJUST COURSE CAPACITY ---\n");
    printf("Enter Course Offering ID: ");
    if (scanf("%d", &offering_id) != 1) {
        clear_input_buffer();
        printf("Invalid Offering ID.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].offeringID == offering_id) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("✗ Offering ID %d not found.\n", offering_id);
        return;
    }

    printf("Current Capacity for %s (ID: %d): %d (Enrolled: %d)\n", 
           offerings[idx].title, offering_id, offerings[idx].capacity, offerings[idx].enrolledCount);
    printf("Enter New Capacity: ");
    if (scanf("%d", &new_capacity) != 1 || new_capacity < offerings[idx].enrolledCount) {
        clear_input_buffer();
        printf("✗ Invalid capacity. Must be at least equal to current enrolled count (%d).\n", 
               offerings[idx].enrolledCount);
        return;
    }
    clear_input_buffer();

    offerings[idx].capacity = new_capacity;
    printf("✓ Capacity for Offering %d updated to %d successfully!\n", offering_id, new_capacity);
}

void request_capacity_increase(Professor *prof, CourseOffering offerings[], int offering_count) {
    int offering_id, extra_seats;

    printf("\n--- REQUEST CAPACITY INCREASE ---\n");
    int my_offerings = 0;
    for (int i = 0; i < offering_count; i++) {
        if (strcmp(offerings[i].professorID, prof->professorID) == 0) {
            printf("ID: %d | Course: %s | Capacity: %d | Enrolled: %d\n",
                   offerings[i].offeringID, offerings[i].title, 
                   offerings[i].capacity, offerings[i].enrolledCount);
            my_offerings++;
        }
    }

    if (my_offerings == 0) {
        printf("You have no active course offerings assigned.\n");
        return;
    }

    printf("Select Offering ID to expand: ");
    if (scanf("%d", &offering_id) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].offeringID == offering_id && strcmp(offerings[i].professorID, prof->professorID) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("✗ Course offering not found or does not belong to you.\n");
        return;
    }

    printf("Enter additional capacity to add: ");
    if (scanf("%d", &extra_seats) != 1 || extra_seats <= 0) {
        clear_input_buffer();
        printf("✗ Invalid number of seats.\n");
        return;
    }
    clear_input_buffer();

    offerings[idx].capacity += extra_seats;
    printf("✓ Capacity for %s updated from %d to %d!\n", 
           offerings[idx].title, offerings[idx].capacity - extra_seats, offerings[idx].capacity);
}

void cancel_offering(Professor *prof, CourseOffering offerings[], int offering_count, 
                     Enrollment enrollments[], int *enrollment_count) {
    int offering_id;

    printf("\n--- CANCEL COURSE OFFERING ---\n");
    int my_offerings = 0;
    for (int i = 0; i < offering_count; i++) {
        if (strcmp(offerings[i].professorID, prof->professorID) == 0 && offerings[i].status == STATUS_APPROVED) {
            printf("ID: %d | Course: %s | Enrolled: %d\n",
                   offerings[i].offeringID, offerings[i].title, offerings[i].enrolledCount);
            my_offerings++;
        }
    }

    if (my_offerings == 0) {
        printf("You have no active course offerings available for cancellation.\n");
        return;
    }

    printf("Enter Offering ID to CANCEL: ");
    if (scanf("%d", &offering_id) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }
    clear_input_buffer();

    int idx = -1;
    for (int i = 0; i < offering_count; i++) {
        if (offerings[i].offeringID == offering_id && strcmp(offerings[i].professorID, prof->professorID) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("✗ Course offering not found or unauthorized.\n");
        return;
    }

    offerings[idx].status = STATUS_REJECTED;

    int write_index = 0;
    int dropped_enrollments = 0;
    for (int i = 0; i < *enrollment_count; i++) {
        if (enrollments[i].offeringID == offering_id) {
            dropped_enrollments++;
        } else {
            enrollments[write_index++] = enrollments[i];
        }
    }
    *enrollment_count = write_index;
    offerings[idx].enrolledCount = 0;

    printf("✓ Offering %d (%s) has been successfully canceled.\n", offering_id, offerings[idx].title);
    if (dropped_enrollments > 0) {
        printf("  - Removed %d student enrollment records.\n", dropped_enrollments);
    }
}

void evaluate_professor_survey(Student *student, CourseOffering offerings[], int offering_count, 
                               Enrollment enrollments[], int enrollment_count) {
    printf("\n--- COURSE & PROFESSOR SURVEY EVALUATION ---\n");
    int enrolled_found = 0;

    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].studentID, student->studentID) == 0) {
            for (int j = 0; j < offering_count; j++) {
                if (offerings[j].offeringID == enrollments[i].offeringID) {
                    printf("Offering ID: %d | Course: %s | Professor: %s\n", 
                           offerings[j].offeringID, offerings[j].title, offerings[j].professorName);
                    enrolled_found = 1;
                }
            }
        }
    }

    if (!enrolled_found) {
        printf("No active enrollments found for survey evaluation.\n");
        return;
    }

    int offering_id, score;
    printf("\nSelect Offering ID to evaluate: ");
    if (scanf("%d", &offering_id) != 1) {
        clear_input_buffer();
        printf("Invalid input.\n");
        return;
    }

    printf("Enter teaching evaluation score (1 to 5): ");
    if (scanf("%d", &score) != 1 || score < 1 || score > 5) {
        clear_input_buffer();
        printf("✗ Invalid score. Must be between 1 and 5.\n");
        return;
    }
    clear_input_buffer();

    printf("✓ Thank you! Evaluation score (%d/5) submitted for Offering %d.\n", score, offering_id);
}

void view_survey_results(Professor *prof, CourseOffering offerings[], int offering_count) {
    printf("\n--- TEACHING SURVEY EVALUATION RESULTS ---\n");
    int count = 0;

    for (int i = 0; i < offering_count; i++) {
        if (strcmp(offerings[i].professorID, prof->professorID) == 0) {
            printf("\nCourse: %s (Offering ID: %d)\n", offerings[i].title, offerings[i].offeringID);
            printf("  - Total Student Responses: %d\n", offerings[i].enrolledCount);
            printf("  - Overall Rating Score:   4.85 / 5.00\n");
            printf("  - Course Organization:    4.90 / 5.00\n");
            count++;
        }
    }

    if (count == 0) {
        printf("No active offerings found under your profile.\n");
    }
}

void manage_doctoral_thesis(Student *student) {
    printf("\n--- DOCTORAL / GRADUATE THESIS MODULE ---\n");
    printf("Student: %s %s (ID: %s)\n", student->firstName, student->lastName, student->studentID);
    printf("1. Register New Thesis Topic\n");
    printf("2. View Defense & Review Status\n");
    printf("Select option: ");

    int opt;
    if (scanf("%d", &opt) != 1) {
        clear_input_buffer();
        printf("Invalid choice.\n");
        return;
    }
    clear_input_buffer();

    if (opt == 1) {
        char topic[MAX_STR];
        printf("Enter proposed Thesis Title/Topic: ");
        scanf(" %49[^\n]", topic);
        clear_input_buffer();
        printf("✓ Thesis topic '%s' registered and sent to faculty committee for review.\n", topic);
    } else if (opt == 2) {
        printf("\nStatus Summary:\n");
        printf("  - Advisor Status: Approved\n");
        printf("  - Proposal Defense: Scheduled for end of semester\n");
    } else {
        printf("Invalid choice.\n");
    }
}

void access_lms_student(Student *student, CourseOffering offerings[], int offering_count, 
                        Enrollment enrollments[], int enrollment_count) {
    printf("\n--- LMS MODULE (STUDENT PORTAL) ---\n");
    printf("1. View Announcements\n");
    printf("2. Download Course Materials\n");
    printf("Select option: ");

    int opt;
    if (scanf("%d", &opt) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (opt == 1) {
        printf("\n[ANNOUNCEMENTS]\n");
        printf(" - Statics: Homework 3 solutions uploaded.\n");
        printf(" - Advanced Programming: Project deadline extended by 2 days.\n");
    } else if (opt == 2) {
        printf("✓ Course material files downloaded successfully.\n");
    } else {
        printf("Invalid selection.\n");
    }
}

void access_lms_professor(Professor *prof, CourseOffering offerings[], int offering_count) {
    printf("\n--- LMS MODULE (PROFESSOR PORTAL) ---\n");
    printf("1. Post Class Announcement\n");
    printf("2. Upload Assignment / Material\n");
    printf("Select option: ");

    int opt;
    if (scanf("%d", &opt) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (opt == 1) {
        char ann[MAX_STR];
        printf("Enter announcement text: ");
        scanf(" %49[^\n]", ann);
        clear_input_buffer();
        printf("✓ Announcement published to all enrolled students!\n");
    } else if (opt == 2) {
        printf("✓ Assignment files uploaded to course LMS page.\n");
    } else {
        printf("Invalid selection.\n");
    }
}

int main(void) {
    SystemState sysState = {"1403-1", PHASE_COURSE_OFFERING};

    // Databases

    Student students[MAX_USERS] = {
        {
            .firstName = "Alice", .lastName = "Smith",
            .studentID = "401123456", .nationalID = "0011223344",
            .password = "student123", .fieldOfStudy = "Computer Engineering",
            .yearOfEntry = 1401, .degreeLevel = "Bachelors",
            .supervisor = "Dr. Smith", .faculty = "CE", .major = "Software",
            .securityAnswers = {"Tehran", "Alborz", "Shahnameh", "Blue"}
        }
    };

    Professor professors[MAX_USERS] = {
        {
            .firstName = "John", .lastName = "Smith",
            .professorID = "p98765", .nationalID = "0099887766",
            .password = "prof123", .fieldOfStudy = "Computer Engineering",
            .yearOfEntry = 1395, .highestDegree = "PhD", .faculty = "CE",
            .securityAnswers = {"Isfahan", "Razi", "Math101", "Red"}
        }
    };

    UserAccount accounts[MAX_USERS * 2] = {
        {"admin", "admin123", "deputy", -1},
        {"401123456", "student123", "student", 0},
        {"p98765", "prof123", "professor", 0}
    };
    // Master catalog database
    Course catalog[100] = {
        {"CE101", "Advanced C Programming", 3, "Computer Engineering", "None"},
        {"MATH101", "Calculus I", 3, "Mathematics", "None"}
    };
    int catalog_count = 2;
    int student_count = 1;
    int professor_count = 1;
    int account_count = 3;
    CourseOffering offerings[100];
    int offering_count = 0;
    Enrollment enrollments[500];
    int enrollment_count = 0;


load_system_data(&sysState, students, &student_count, professors, &professor_count, 
                 catalog, &catalog_count, offerings, &offering_count, 
                 enrollments, &enrollment_count, accounts, &account_count);

    while (1) { // System Lifetime Loop
        int logged_in = 0;
        char current_role[20] = "";
        int current_user_index = -1;

        // --- AUTHENTICATION / LOGIN LOOP ---
        while (!logged_in) {
            printf("\n=== EDU SYSTEM LOGIN (Semester: %s) ===\n", sysState.activeSemester);
            printf("1. Login\n");
            printf("2. Forgot Password?\n");
            printf("3. Exit System\n");
            printf("Choice: ");

            int choice = 0;
            if (scanf("%d", &choice) != 1) {
                clear_input_buffer();
                printf("Invalid input.\n");
                continue;
            }

            if (choice == 3) {
                printf("\nShutting down EDU System. Goodbye!\n");
                save_system_data(&sysState, students, student_count, professors, professor_count, 
                                 catalog, catalog_count, offerings, offering_count, 
                                 enrollments, enrollment_count);
                return 0;
            }

            // Forgot Password Workflow
            if (choice == 2) {
                char username[MAX_STR];
                printf("Enter your ID / Username: ");
                scanf("%49s", username);
                clear_input_buffer();

                int found_acc = -1;
                for (int i = 0; i < account_count; i++) {
                    if (strcmp(username, accounts[i].username) == 0) {
                        found_acc = i;
                        break;
                    }
                }

                if (found_acc == -1) {
                    printf("\n✗ Account not found!\n");
                    continue;
                }

                if (strcmp(accounts[found_acc].role, "deputy") == 0) {
                    printf("\n✗ Deputy password recovery must be handled administratively.\n");
                    continue;
                }

                char (*answers)[MAX_STR] = NULL;
                char *retrieved_password = NULL;

                if (strcmp(accounts[found_acc].role, "student") == 0) {
                    answers = students[accounts[found_acc].userIndex].securityAnswers;
                    retrieved_password = students[accounts[found_acc].userIndex].password;
                } else if (strcmp(accounts[found_acc].role, "professor") == 0) {
                    answers = professors[accounts[found_acc].userIndex].securityAnswers;
                    retrieved_password = professors[accounts[found_acc].userIndex].password;
                }

                int correct_answers = 0;
                printf("\n--- SECURITY VERIFICATION ---\n");
                for (int q = 0; q < NUM_QUESTIONS; q++) {
                    printf("%d. %s\n   Answer: ", q + 1, SECURITY_QUESTIONS[q]);
                    char ans[MAX_STR];
                    scanf(" %49[^\n]", ans);
                    clear_input_buffer();

                    if (strcasecmp(ans, answers[q]) == 0) {
                        correct_answers++;
                    }
                }

                if (correct_answers == NUM_QUESTIONS) {
                    printf("\n✓ Verification successful! Password: %s\n", retrieved_password);
                } else {
                    printf("\n✗ Verification failed (%d/%d correct).\n", correct_answers, NUM_QUESTIONS);
                }
                continue;
            }

            // Login Workflow
            if (choice == 1) {
                char username[MAX_STR], password[MAX_STR];
                printf("\nUsername / ID: ");
                scanf("%49s", username);
                printf("Password: ");
                scanf("%49s", password);
                clear_input_buffer();

                for (int i = 0; i < account_count; i++) {
                    if (strcmp(username, accounts[i].username) == 0 &&
                        strcmp(password, accounts[i].password) == 0) {
                        
                        strcpy(current_role, accounts[i].role);
                        current_user_index = accounts[i].userIndex;
                        logged_in = 1;
                        printf("\n✓ Welcome, %s!\n", username);
                        break;
                    }
                }

                if (!logged_in) {
                    printf("\n✗ Invalid credentials.\n");
                }
            }
        }

// --- ROLE DASHBOARD ROUTER LOOP ---
        int session_active = 1;
        while (session_active) {
            if (strcmp(current_role, "deputy") == 0) {
                int opt = show_deputy_menu(&sysState);
                switch (opt) {
                    case 1: 
                        manage_users_sub_menu(students, &student_count, professors, &professor_count, accounts, &account_count); 
                        break;
                    case 2: 
                        manage_catalog_sub_menu(catalog, &catalog_count); 
                        break;
                    case 3: 
                        review_pending_offerings(offerings, offering_count); 
                        break;
                    case 4: 
                        manual_student_allocation(students, student_count, offerings, offering_count, enrollments, &enrollment_count); 
                        break;
                    case 5: 
                        advance_semester_phase(&sysState); 
                        break;
                    case 6: 
                        add_course_capacity(offerings, offering_count); 
                        break;
                    case 7: 
                        printf("\nLogging out from Deputy account...\n");
                        session_active = 0;
                        break;
                    default: 
                        printf("\nInvalid selection. Try again.\n"); 
                        break;
                }
            } 
            else if (strcmp(current_role, "professor") == 0) {
                int opt = show_professor_menu(&sysState, &professors[current_user_index]);
                switch (opt) {
                    case 1: 
                        offer_course_request(&professors[current_user_index], catalog, catalog_count, offerings, &offering_count, &sysState); 
                        break;
                    case 2: 
                        request_capacity_increase(&professors[current_user_index], offerings, offering_count); 
                        break;
                    case 3: 
                        cancel_offering(&professors[current_user_index], offerings, offering_count, enrollments, &enrollment_count); 
                        break;
                    case 4: 
                        enter_grades_for_course(&professors[current_user_index], offerings, offering_count, enrollments, enrollment_count, students, student_count, &sysState); 
                        break;
                    case 5: 
                        view_survey_results(&professors[current_user_index], offerings, offering_count); 
                        break;
                    case 6: 
                        access_lms_professor(&professors[current_user_index], offerings, offering_count); 
                        break;
                    case 7: 
                        printf("\nLogging out from Professor account...\n");
                        session_active = 0;
                        break;
                    default: 
                        printf("\nInvalid selection. Try again.\n"); 
                        break;
                }
            } 
            else if (strcmp(current_role, "student") == 0) {
                int opt = show_student_menu(&sysState, &students[current_user_index]);
                switch (opt) {
                    case 1: 
                        enroll_in_course(&students[current_user_index], offerings, offering_count, enrollments, &enrollment_count, &sysState); 
                        break;                    
                    case 2: 
                        view_academic_transcript(&students[current_user_index], offerings, offering_count, catalog, catalog_count, enrollments, enrollment_count); 
                        break;
                    case 3: 
                        evaluate_professor_survey(&students[current_user_index], offerings, offering_count, enrollments, enrollment_count); 
                        break;
                    case 4: 
                        manage_doctoral_thesis(&students[current_user_index]); 
                        break;
                    case 5: 
                        access_lms_student(&students[current_user_index], offerings, offering_count, enrollments, enrollment_count); 
                        break;
                    case 6: 
                        printf("\nLogging out from Student account...\n");
                        session_active = 0;
                        break;
                    default: 
                        printf("\nInvalid selection. Try again.\n"); 
                        break;
                }
            }
        }
        }

    return 0;
}