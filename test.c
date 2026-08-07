#include <stdio.h>

void create_system_txt() {
    FILE *fp = fopen("system.txt", "w");
    if (fp) {
        fprintf(fp, "1406-1\n2\n");
        fclose(fp);
        printf("[✓] Generated system.txt (Active Semester: 1406-1, Phase: 2)\n");
    } else {
        printf("[X] Error creating system.txt\n");
    }
}

void create_users_txt() {
    FILE *fp = fopen("users.txt", "w");
    if (fp) {
        fprintf(fp, "8 4\n");
        
        fprintf(fp, "S 404101234 pass123 Ali Rezaei Mechanical 1404 Engineering\n");
        fprintf(fp, "S 404102345 pass123 Mohammad Hosseini Mechanical 1404 Engineering\n");
        fprintf(fp, "S 402103456 pass123 Amir Karimi Mechanical 1402 Engineering\n");
        fprintf(fp, "S 401105678 pass456 Sara Ahmadi Computer 1401 Engineering\n");
        fprintf(fp, "S 402106789 pass456 Neda Hassani Computer 1402 Engineering\n");
        fprintf(fp, "S 400107890 pass456 Kaveh Rahimi Computer 1400 Engineering\n");
        fprintf(fp, "S 401108901 pass789 Reza Taghavi Electrical 1401 Engineering\n");
        fprintf(fp, "S 402109012 pass789 Maryam Pour Electrical 1402 Engineering\n");

        fprintf(fp, "P 990101 prof123 Hosein Mohammadi 1395 Engineering\n");
        fprintf(fp, "P 990102 prof123 Maryam Shafa 1398 Engineering\n");
        fprintf(fp, "P 990103 prof123 Babak Rostami 1392 Engineering\n");
        fprintf(fp, "P 990104 prof123 Zahra Tehrani 1400 Basic_Sciences\n");

        fclose(fp);
        printf("[✓] Generated users.txt (8 Students, 4 Professors)\n");
    } else {
        printf("[X] Error creating users.txt\n");
    }
}

void create_catalog_txt() {
    FILE *fp = fopen("catalog.txt", "w");
    if (fp) {
        fprintf(fp, "10\n");

        fprintf(fp, "MATH101;Calculus_I;3;Mathematics;NONE\n");
        fprintf(fp, "MATH102;Calculus_II;3;Mathematics;MATH101\n");
        fprintf(fp, "PHYS101;General_Physics_I;3;Physics;NONE\n");
        fprintf(fp, "PHYS102;General_Physics_II;3;Physics;PHYS101\n");
        fprintf(fp, "ME101;Statics;3;Mechanical;PHYS101\n");
        fprintf(fp, "ME102;Dynamics;3;Mechanical;ME101\n");
        fprintf(fp, "ME201;Thermodynamics_I;3;Mechanical;PHYS101\n");
        fprintf(fp, "CS101;Fundamentals_of_Programming;3;Computer;NONE\n");
        fprintf(fp, "CS102;Advanced_Programming;3;Computer;CS101\n");
        fprintf(fp, "CS201;Data_Structures;3;Computer;CS102\n");

        fclose(fp);
        printf("[✓] Generated catalog.txt (10 Courses in Course Catalog)\n");
    } else {
        printf("[X] Error creating catalog.txt\n");
    }
}

void create_offerings_txt() {
    FILE *fp = fopen("offerings.txt", "w");
    if (fp) {
        fprintf(fp, "7\n");

        fprintf(fp, "101;ME101;Statics;990101;Dr_Hosein_Mohammadi;35;3;Mon/Wed_08:00-09:30;1406-10-15;1\n");
        fprintf(fp, "102;ME102;Dynamics;990101;Dr_Hosein_Mohammadi;30;2;Sun/Tue_10:00-11:30;1406-10-18;1\n");
        fprintf(fp, "103;CS102;Advanced_Programming;990102;Dr_Maryam_Shafa;30;3;Sat/Mon_13:30-15:00;1406-10-20;1\n");
        fprintf(fp, "104;CS201;Data_Structures;990102;Dr_Maryam_Shafa;25;2;Sun/Tue_08:00-09:30;1406-10-22;1\n");
        fprintf(fp, "105;MATH101;Calculus_I;990104;Dr_Zahra_Tehrani;50;3;Sat/Mon_10:00-11:30;1406-10-12;1\n");
        fprintf(fp, "106;MATH102;Calculus_II;990104;Dr_Zahra_Tehrani;45;2;Sun/Tue_13:30-15:00;1406-10-14;1\n");
        fprintf(fp, "107;PHYS101;General_Physics_I;990103;Dr_Babak_Rostami;40;2;Mon/Wed_15:30-17:00;1406-10-16;1\n");

        fclose(fp);
        printf("[✓] Generated offerings.txt (7 Course Offerings)\n");
    } else {
        printf("[X] Error creating offerings.txt\n");
    }
}

void create_enrollments_txt() {
    FILE *fp = fopen("enrollments.txt", "w");
    if (fp) {
        fprintf(fp, "17\n");

        fprintf(fp, "404101234;101;18.50\n");
        fprintf(fp, "404101234;102;-1.00\n");
        fprintf(fp, "404101234;105;16.00\n");

        fprintf(fp, "404102345;101;14.25\n");
        fprintf(fp, "404102345;106;-1.00\n");

        fprintf(fp, "402103456;101;-1.00\n");
        fprintf(fp, "402103456;107;-1.00\n");

        fprintf(fp, "401105678;103;19.75\n");
        fprintf(fp, "401105678;104;-1.00\n");

        fprintf(fp, "402106789;103;-1.00\n");
        fprintf(fp, "402106789;105;-1.00\n");

        fprintf(fp, "400107890;103;17.00\n");
        fprintf(fp, "400107890;104;20.00\n");

        fprintf(fp, "401108901;105;15.50\n");
        fprintf(fp, "401108901;107;-1.00\n");

        fprintf(fp, "402109012;106;-1.00\n");

        fclose(fp);
        printf("[✓] Generated enrollments.txt (17 Enrollment Records)\n");
    } else {
        printf("[X] Error creating enrollments.txt\n");
    }
}

int main() {
    printf("===========================================\n");
    printf("   GENERATING RICH TEST DATA FOR SYSTEM    \n");
    printf("===========================================\n\n");

    create_system_txt();
    create_users_txt();
    create_catalog_txt();
    create_offerings_txt();
    create_enrollments_txt();

    printf("\n[SUCCESS] All rich dataset text files generated successfully!\n");
    return 0;
}