/*
 * Google Code Jam 2014
 * Round 1B Problem x
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
 */

#include <stdio.h>
#include <stdlib.h>

int solve(unsigned int a, unsigned int b, unsigned int k)
{
    int ret = 0;
    int i,j;

    for(i = 0; i < a; i++)
        for(j = 0; j < b; j++)
            if((i & j) < k)
                ret++;

    return ret;
}

int main(int argc, char **argv)
{
    int num_tests, test = 0, ret = 1;
    int round;
    unsigned int a,b,k;
     
    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 100) {
        printf("Illegal number of tests (1 <= T <= 100)\n");
        goto out;
    }

    while(test++ < num_tests) {
        scanf("%d %d %d", &a, &b, &k);
        if(a < 1 || a > 1000) {
            printf("1 <= a <= 1000\n");
            continue;
        }
        if(b < 1 || b > 1000) {
            printf("1 <= b <= 1000\n");
            continue;
        }
        if(k < 1 || k > 1000) {
            printf("1 <= k <= 1000\n");
            continue;
        }

        round = solve(a,b,k);
        printf("Case #%d: ",test);
        if(round < 0)
            printf("ERROR\n");
        else
            printf("%d\n",round);
    }

    ret = 0;
out:
    return ret;
}

