/*
 * Google Code Jam 2014
 * Round 1B Problem x
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int average(int *nums, int len)
{
    int i, ret = 0;
    for(i = 0; i < len; i++)
        ret += nums[i];
    ret /= len;
    return ret;
}

static inline int run_length(char *p, char c) 
{
    int i = 0;
    char d;
    while((d = p[i++]) == c) {}
    return i-1;
}

int solve(char **strings, int num)
{
    int ret = 0;
    int j;
    int runs[num];
    int ptr[num];
    int avg_run;
    char c;

    memset(ptr, 0, num * sizeof(int));
    while((c = strings[0][ptr[0]]) != '\0') {
        for(j = 0; j < num; j++) {
            runs[j] = run_length(&strings[j][ptr[j]], c);
            if(runs[j] == 0) {
                ret = -1;
                goto out;
            }
            ptr[j] += runs[j];
        } 
        avg_run = average(runs, num);
        for(j = 0; j < num; j++) {
            if(runs[j] > avg_run)
                ret += runs[j] - avg_run;
            else
                ret += avg_run - runs[j];
        }
    }

out:
    return ret;
}

int main(int argc, char **argv)
{
    int num_tests, test = 0, ret = 1;
    int i, j, n, round;
    size_t this_len;
    char *strings[100];
     
    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 100) {
        printf("Illegal number of tests (1 <= T <= 100)\n");
        goto out;
    }

top:
    while(test++ < num_tests) {
        scanf("%d", &n);
        if(n < 2 || n > 100) {
            printf("ERROR\n");
            continue;
        }

        for(i = 0; i < n; i++) {
            scanf("%ms",&strings[i]);
            if(!strings[i]) {
                printf("ERROR\n");
                for(j = 0; j < i; j++) free(strings[j]);
                goto top;
            }
            this_len = strlen(strings[i]);
            if(this_len > 100) {
                printf("1 <= len <= 100\n");
                for(j = 0; j < i; j++) free(strings[j]);
                goto top;
            }
            strings[i][this_len] = 0;
        }

        round = solve(strings, n);
        printf("Case #%d: ",test);
        if(round < 0)
            printf("Fegla Won\n");
        else 
            printf("%d\n",round);

        for(i = 0; i < n; i++) free(strings[i]);
    }

    ret = 0;
out:
    return ret;
}

