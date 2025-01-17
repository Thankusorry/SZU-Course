#include <stdio.h>
#include <string.h>

#define MAX_LEN 1000

void replace_you_with_we(char *str)
{
    char buffer[MAX_LEN];
    char *pos = str;
    char *next_pos;
    buffer[0] = '\0';

    while ((next_pos = strstr(pos, "you")) != NULL)
    {

        strncat(buffer, pos, next_pos - pos);

        strcat(buffer, "we");

        pos = next_pos + 3;
    }

    strcat(buffer, pos);

    strcpy(str, buffer);
}

int main()
{
    char line[MAX_LEN];

    while (fgets(line, sizeof(line), stdin))
    {

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        if (strcmp(line, "-==-") == 0)
        {
            break;
        }

        replace_you_with_we(line);
        printf("%s\n", line);
    }
    return 0;
}