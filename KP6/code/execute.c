#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "student.h"

void print_students_need_upgrade(FILE *in, int upgrade_threshold) {
    Student s;

    while (student_read_bin(&s, in)) {
        if (s.externalDevices > upgrade_threshold) {
            student_print(&s);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage:\n\t./execute DB_FILE UPGRADE_THRESHOLD\n");
        exit(0);
    }

    int upgrade_threshold = atoi(argv[2]);
    FILE *in = fopen(argv[1], "rb");
    if (!in) {
        printf("I/O Error: can't open file.\n");
        exit(1);
    }

    print_students_need_upgrade(in, upgrade_threshold);

    fclose(in);

    return 0;
}
