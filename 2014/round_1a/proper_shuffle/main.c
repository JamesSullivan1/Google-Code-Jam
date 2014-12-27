/*
 * GOOGLE CODE JAM - 2014
 * Round 1A C 
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
 */

#include <stdio.h>
#include <stdlib.h>

#define GOOD    1
#define BAD     0

#define THRESHOLD 0.0275 /* Found by guess and check to be reasonably
                          * fair to both good and bad guesses. */

/* Returns GOOD if and only if the sequence is deemed to be a normal
 * permutation, and BAD otherwise.
 *
 * Determined by computing the number of elements that were shifted
 * forward, which the bad algorithm heavily favours. If this is less
 * than the average of 1/2 of the elements plus a small deviation, then 
 * the sequence is probably fair. If the sequence shifts more than this
 * number of elements forward, it is probably not fair.
 */
int solve2(int *sequence, int num)
{
    int ret = -1;
    int i;
    double num_inc;
    double norm_inc = num / 2;
    double threshold_inc = norm_inc + (norm_inc * THRESHOLD);

    if(!sequence)
        goto out;

    num_inc = 0;
    for(i = 0; i < num; i++) {
        if(sequence[i] > i)
            num_inc++;
    }
    printf("%.2f inc, %.2f threshold\n",num_inc, threshold_inc);
    if(num_inc <= threshold_inc)
        ret = GOOD;
    else
        ret = BAD;
     
out:
    return ret;
}

/* Returns GOOD if and only if the sequence is deemed to be a normal
 * permutation, and BAD otherwise.
 *
 * Determined by computing the average distance that an element was
 * offset from its original position. The bad algorithm favours a
 * smaller shift forward (and thus a smaller delta), while the good
 * algorithm has no such favouring. If the average is less than a normal
 * average, minus a small deviation, then the sample is probably bad.
 */
int solve(int *sequence, int num)
{
    int ret = -1;
    double avg_distance, total_distance;
    double this_distance, norm_distance = num / 2;
    double acceptable_distance = norm_distance - (norm_distance * THRESHOLD);
    int is_good;
    int i;

    if(!sequence)
        goto out;

    total_distance = 0;
    for(i = 0; i < num; i++) {
        if(sequence[i] <= i) {
            this_distance = (num - i) + sequence[i];
        } else {
            this_distance = sequence[i] - i;
        }
        total_distance += this_distance;
    }
    avg_distance = total_distance / (double)num;
    printf("Acc: %f Avg: %f\n",acceptable_distance, avg_distance);
    if(avg_distance < acceptable_distance)
        is_good = 0;
    else
        is_good = 1;
     
    ret = is_good;
out:
    return ret;
}

int main(int argc, char **argv)
{
    int num_tests, test = 0;
    int n, i, good;
    int seq[1000];
    int ret = 1;
    int total_good, total_bad;

    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 120) {
        printf("Illegal number of tests (1 <= T <= 120)\n");
        goto out;
    }

    total_good = total_bad = 0;
    while(test++ < num_tests) {
        scanf("%d",&n);
        if(n < 1 || n > 1000) {
            printf("Illegal sequence size (1 <= N <= 1000)\n");
            continue;
        }

        for(i = 0; i < n; i++)
           scanf("%d",&seq[i]); 

        good = solve2(seq, n);
        if(good) {
            printf("GOOD\n");
            total_good++;
        } else {
            printf("BAD\n");
            total_bad++;
        }
    }
    printf("GOOD: %d\n",total_good);
    printf("BAD: %d\n",total_bad);

    ret = 0;
out:
    return ret;
}
