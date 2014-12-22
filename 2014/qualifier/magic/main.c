/*
 * GOOGLE CODE JAM - 2014
 * Qualifier
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
 * =====================================================================
 *
 * Problem
 *
 * Recently you went to a magic show. You were very impressed by one of
 * the tricks, so you decided to try to figure out the secret behind it!
 *
 * The magician starts by arranging 16 cards in a square grid: 4 rows of
 * cards, with 4 cards in each row. Each card has a different number
 * from 1 to 16 written on the side that is showing. Next, the magician
 * asks a volunteer to choose a card, and to tell him which row that
 * card is in.
 *
 * Finally, the magician arranges the 16 cards in a square grid again,
 * possibly in a different order. Once again, he asks the volunteer
 * which row her card is in. With only the answers to these two
 * questions, the magician then correctly determines which card the
 * volunteer chose. Amazing, right?
 *
 * You decide to write a program to help you understand the magician's
 * technique. The program will be given the two arrangements of the
 * cards, and the volunteer's answers to the two questions: the row
 * number of the selected card in the first arrangement, and the row
 * number of the selected card in the second arrangement. The rows are
 * numbered 1 to 4 from top to bottom.
 *
 * Your program should determine which card the volunteer chose; or if
 * there is more than one card the volunteer might have chosen (the
 * magician did a bad job); or if there's no card consistent with the
 * volunteer's answers (the volunteer cheated).
 *
 *
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve(int first, int second, int grid[][4], int newgrid[][4])
{
    int ret = -1, i, j, k = 0;
    int candidates[4] = { 0 };

    if(grid == NULL || newgrid == NULL)
        goto out;
    if(first < 0 || first > 16)
        goto out;
    if(second < 0 || second > 16)
        goto out;

    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(grid[first-1][i] == newgrid[second-1][j])
               candidates[k++] = grid[first-1][i]; 
        }
    }

    if(k == 0)
        ret = -1;
    else if(k == 1)
        ret = candidates[0];
    else
        ret = 0;

out:
    return ret;
}

int main(void)
{
    int num_tests, test = 0, first, second, round;
    int grid_first[4][4] = { { 0 } };
    int grid_second[4][4] = { { 0 } };
    int i, ret = 1;

    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 100) {
        printf("Illegal number of tests (1 <= T <= 100)\n");
        goto out;
    }

    while(test++ < num_tests) {
        scanf("%d",&first);
        for(i = 0; i < 4; i++) {
            scanf("%d %d %d %d", &grid_first[i][0], &grid_first[i][1],     
                    &grid_first[i][2], &grid_first[i][3]);
        }
        scanf("%d",&second);
        for(i = 0; i < 4; i++) {
            scanf("%d %d %d %d", &grid_second[i][0], &grid_second[i][1],     
                    &grid_second[i][2], &grid_second[i][3]);
        }

        round = solve(first, second, grid_first, grid_second);

        printf("Case #%d: ",test);
        switch(round) {
        case 0:
            printf("Bad magician!\n");
            break;
        case -1:
            printf("Volunteer cheated!\n");
            break;
        default:
            printf("%d\n",round);
        }     
    }

    ret = 0;
out:
    return ret;
}   

