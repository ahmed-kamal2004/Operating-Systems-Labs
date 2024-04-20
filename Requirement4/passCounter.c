#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

// the parent for all of them takes the students_pre_ta + overflow

int main(int argc, char *argv[])
{
    char *file_path = argv[1];
    int N = atoi(argv[2]);
    int Min_grade = atoi(argv[3]);

    FILE *file = fopen(file_path, "r");
    int num_of_students = 0;
    fscanf(file, "%d", &num_of_students);

    int students_per_ta = num_of_students / N;
    int overflow = students_per_ta % N;

    int grades[num_of_students + 1][2];
    int parent_pid = getpid();

    // reading the grades from the file
    for (int i = 0; i < num_of_students; i++)
    {
        fscanf(file, "%d %d", &grades[i][0], &grades[i][1]);
    }
    int result = 0;

    fclose(file);
    int count = 1;
    for (int m = 0; m < N - 1; m++)
    {
        result = 0;
        int pin = fork();
        count = m;
        if (pin != 0)
        {
            int start = num_of_students - (overflow + students_per_ta);
            for (int counter = (N-1) * students_per_ta; counter < num_of_students; counter++)
            {
                if ((grades[counter][1] + grades[counter][0]) >= Min_grade)
                {
                    result++;
                }
            }
            char message[100];
            sprintf(message, "output%d.txt", N);

            FILE *write_file = fopen(message, "w");
            fprintf(write_file, " %d ", result);
            fclose(write_file);
        }
        else
        {
            for (int counter = count * students_per_ta; counter < (count + 1) * students_per_ta; counter++)
            {
                if ((grades[counter][1] + grades[counter][0]) >= Min_grade)
                {
                    result++;
                }
            }

            char message[100];
            sprintf(message, "output%d.txt", count + 1);

            FILE *write_file = fopen(message, "w");
            fprintf(write_file, " %d ", result);
            fclose(write_file);
            exit(0);
        }
    }

    wait(NULL);
    char message[100];
    // FILE *write_file = fopen("output.txt", "w");
    for (int i = 1; i <= N; i++)
    {
        int data = 0;
        sprintf(message, "output%d.txt", i);
        FILE *open_file = fopen(message, "r");
        fscanf(open_file, "%d", &data);
        // fprintf(write_file, "%d ", data);
        printf("%d ",data);
        fclose(open_file);
        // remove(message);
    }
    // fclose(write_file);
}
