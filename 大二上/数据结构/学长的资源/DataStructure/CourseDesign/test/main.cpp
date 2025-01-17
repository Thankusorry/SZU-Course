#include <string.h>
#include <stdio.h>

void s1(char str[], int l) {
    int num = 0, j = 0, i, max = 0, b[1000] = {0}, flag = 0, qq = 0;
    for (i = 0; i < l; i++)/*记录每个单词的下标*/
    {
        if (str[i] == ' ') {
            b[num + 1] = i + 1;
            num++;
        }
    }
    b[num + 1] = l + 1;
    max = 0;
    for (i = 0; i <= num; i++)/*找最大*/
    {
        if (max < (b[i + 1] - b[i] - 1)) {
            max = b[i + 1] - b[i] - 1;
        }
    }
    for (i = 0; i <= num; i++)/*输出*/
    {
        qq = b[i + 1] - b[i] - 1;
        if (qq == max && flag == 0) {
            for (j = b[i]; j < b[i + 1] - 1; j++) {
                printf("%c", str[j]);
                flag = 1;
            }
        } else if (qq == max && flag == 1) {
            printf(" ");
            for (j = b[i]; j < b[i + 1] - 1; j++) {
                printf("%c", str[j]);
            }
        }
    }
    printf("\n");
}

int main() {
    int i, t, j, n, flag = 0, num, l;
    char c;
    int ii = 0;
    char str[1000];
    scanf("%d", &t);
    getchar();
    for (i = 0; i < t; i++) {
        int ii = 0;
        char c;

        memset(str, ' ', l);
        while ((c = getchar()) != '\n') {
            str[ii++] = c;
        }
        str[ii] = '\0';

        l = strlen(str);
        s1(str, l);
        memset(str, ' ', sizeof(str));
    }
    return 0;
}