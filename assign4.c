#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name[84];
    char sched[4];
    unsigned size;
    unsigned hours;
    char padding[20];
} COURSE;

int readCourse(FILE *file, const int course_num) {
    const int seek_pos = course_num * sizeof(COURSE);
    COURSE course;
    const COURSE empty_course = {0};
    fseek(file, seek_pos, SEEK_SET);
    const size_t read = fread(&course, sizeof(COURSE), 1, file);
    if (read != 1 || memcmp(&course, &empty_course, sizeof(COURSE)) == 0) {
        printf("ERROR: course not found\n");
        return 1;
    }
    printf("Course number: %d\n", course_num);
    printf("Course name: %s\n", course.name);
    printf("Scheduled days: %s\n", course.sched);
    printf("Credit hours: %u\n", course.hours);
    printf("Enrolled students: %u\n", course.size);
	return 0;
    
}

int createCourse(FILE *file, const COURSE *course, const int course_num) {
    int course_pos = course_num * sizeof(COURSE);
    COURSE temp = {0};
    fseek(file, course_pos, SEEK_SET);
    size_t read = fread(&temp, sizeof(COURSE), 1, file);
    if(read == 1 &&  temp.name[0] != '\0') {
        printf("ERROR: course already exists\n");
        return 1;
    }
    fseek(file, course_pos, SEEK_SET);
    fwrite(course, sizeof(COURSE), 1, file);
	return 0;
}

int updateCourse(FILE *file, const COURSE *course, const int course_num) {
    COURSE old_course;
    COURSE new_course = {0};
    const int seek_pos = course_num * sizeof(COURSE);
    fseek(file, seek_pos, SEEK_SET);
    const size_t read = fread(&old_course, sizeof(COURSE), 1, file);
    if (read != 1 || memcmp(&old_course, &new_course, sizeof(COURSE)) == 0) {
        printf("ERROR: course not found\n");
        return 1;
    }
    new_course = old_course;

    if (course->size != 0) {
        new_course.size = course->size;
    }
    if (course->hours != 0) {
        new_course.hours = course->hours;
    }
    if (course->name[0] != '\0') {
        snprintf(new_course.name, sizeof(new_course.name), "%s", course->name);
    }
    if (course->sched[0] != '\0') {
        snprintf(new_course.sched, sizeof(new_course.sched), "%s", course->sched);
    }

    fseek(file, seek_pos, SEEK_SET);
    fwrite(&new_course, sizeof(COURSE), 1, file);
	return 0;
}

int deleteCourse(FILE *file, const int course_num) {
    const COURSE empty_course = {0};
    COURSE temp;
    const int seek_pos = course_num * sizeof(COURSE);
    fseek(file, seek_pos, SEEK_SET);
    const size_t read = fread(&temp, sizeof(COURSE), 1, file);

    if(read != 1 || memcmp(&temp, &empty_course, sizeof(COURSE)) == 0) {
        printf("ERROR: course does not exist\n");
        return 1;
    }
    fseek(file, seek_pos, SEEK_SET);
    fwrite(&empty_course, sizeof(COURSE), 1, file);
	return 0;
}

int readInt(void* value, const char* format) {
    char buffer[20];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (buffer[0] == '\0') {
            return 1;
        }
        if(sscanf(buffer, format, value) != 1) {
            return 1;
        }
        return 0;
    }
    return 1;
}

void printMenu() {
    printf("Enter one of the following actions or press CTRL-D to exit\n");
    printf("C - create a new course record\n");
    printf("U - update an existing course record\n");
    printf("R - read an existing course record\n");
    printf("D - delete an existing course record\n");
}

int main(int argc, char *argv[]) {
    const char *out;
    if (argc == 2) {
        out = argv[1];
    } else {
        out = "courses.dat";
    }
    
    int option;
	int d;

    printMenu();

    while((option = getchar()) != EOF) {
		char line[20];
		FILE *file = fopen(out, "rb+");
        if (!file) {
            file = fopen(out, "wb+");
            if (!file) {
                perror("Error opening or creating file");
                return 1;
            }
        }

        option = toupper(option);
        while((d = getchar()) != '\n' && d != EOF) {}
		int course_num;
        COURSE course = {0};

        switch(option) {
            case 'C':
                printf("Course number: ");
                readInt(&course_num, "%d");
                printf("Course name: ");
                fgets(course.name, sizeof(course.name), stdin);
                course.name[strcspn(course.name, "\n")] = '\0';
                printf("Course schedule: ");
                fgets(line, sizeof(line), stdin);
                line[strcspn(line, "\n")] = '\0';
                strncpy(course.sched, line, sizeof(course.sched) - 1);
                course.sched[sizeof(course.sched) - 1] = '\0';
                printf("Course credit hours: ");
                readInt(&(course.hours), "%u");
        		printf("Course enrollment: ");
        		readInt(&(course.size), "%u");
                createCourse(file, &course, course_num);
                break;
            case 'U':
            	printf("Course number: ");
        		readInt(&course_num, "%d");
        		printf("Course name: ");
        		fgets(course.name, sizeof(course.name), stdin);
        		course.name[strcspn(course.name, "\n")] = '\0';
        		printf("Course schedule: ");
        		fgets(line, sizeof(line), stdin);
        		line[strcspn(line, "\n")] = '\0';
        		strncpy(course.sched, line, sizeof(course.sched) - 1);
        		course.sched[sizeof(course.sched) - 1] = '\0';
        		printf("Course credit hours: ");
        		readInt(&(course.hours), "%u");
        		printf("Course enrollment: ");
        		readInt(&(course.size), "%u");
                updateCourse(file, &course, course_num);
                break;
            case 'R':
                printf("Enter course number to read: ");
                readInt(&course_num, "%d");
                readCourse(file, course_num);
                break;
            case 'D':
                printf("Enter course number to delete: ");
                readInt(&course_num, "%d");
                deleteCourse(file, course_num);
                break;
            default:
            printf("Invalid option. Please try again.\n");
                break;
        }
        fclose(file);
        printMenu();
    }
    printf("Exiting program.\n");
    

    return 0;
}
