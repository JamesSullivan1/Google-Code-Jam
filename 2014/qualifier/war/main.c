/*
 * GOOGLE CODE JAM - 2014
 * Qualifier D
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
 *
 * =====================================================================
 * 
 * Problem
 *
 * Naomi and Ken sometimes play games together. Before they play, each
 * of them gets N identical-looking blocks of wood with masses between
 * 0.0kg and 1.0kg (exclusive). All of the blocks have different
 * weights. There are lots of games they could play with those blocks,
 * but they usually play something they call War. Here is how War works: 
 *
 * 1. Each player weighs each of his or her own blocks, so each player
 *    knows the weights of all of his or her own blocks, but not the
 *    weights of the other player's blocks. 
 *
 * 2. They repeat the following process N times:
 *
 *     1. Naomi chooses one of her own blocks, with mass ChosenNaomi.
 *     2. Naomi tells Ken the mass of the block she chose.
 *     3. Ken chooses one of his own blocks, with mass ChosenKen.
 *     4. They each put their block on one side of a balance
 *          scale, and the person whose block is heavier gets one
 *          point.
 *     5. Both blocks are destroyed in a fire.
 *
 * Naomi has realized three things about War. First, she has
 * realized that she loses a lot. Second, she has realized that
 * there is a unique strategy that Ken can follow to maximize
 * his points without assuming anything about Naomi's strategy,
 * and that Ken always uses it. Third, she has realized that
 * she hates to lose. Naomi has decided that instead of playing
 * War, she will play a game she calls Deceitful War. The great
 * thing about Deceitful War is that Ken will think they're
 * playing War!
 *
 * Here is how Deceitful War works, with differences between
 * Deceitful War and War in bold: 
 *
 * 1.  Each player weighs each of his or her own blocks. _Naomi also
 *     weighs Ken's blocks while he isn't looking, so Naomi knows the
 *     weights of all blocks_ and Ken only knows the weights of his own
 *     blocks. 
 *
 * 2. They repeat the following process N times: 
 *
 *     1. Naomi chooses one of her own blocks, with mass ChosenNaomi.
 *     2. Naomi tells Ken _a number, ToldNaomi, betweem 0.0kg and 1.0kg
 *        exclusive_, Ken, who thinks they're playing War, thinks the
 *        number Naomi just told him is ChosenNaomi.
 *     3. Ken chooses one of his own blocks, with mass ChosenKen.
 *     4. They each put their block on one side of a balance
 *          scale, and the person whose block is heavier gets one
 *          point.
 *     5. Both blocks are destroyed in a fire.
 *
 * Naomi doesn't want Ken to know that she isn't playing War; so when
 * she is choosing which block to play, and what mass to tell Ken, she
 * must make sure that the balance scale won't reveal that ChosenNaomi ≠
 * ToldNaomi. In other words, she must make decisions so that: 
 *
 * -ChosenNaomi > ChosenKen if, and only if, ToldNaomi > ChosenKen, and 
 *
 * It might seem like Naomi won't win any extra points by being
 * deceitful, because Ken might discover that she wasn't playing War;
 * but Naomi knows Ken thinks both players are playing War, and she
 * knows what he knows, and she knows Ken will always follow his
 * unique optimal strategy for War, so she can always predict what he
 * will play.
 *
 * You'll be given the masses of the blocks Naomi and Ken started with.
 * Naomi will play Deceitful War optimally to gain the maximum number
 * of points. Ken will play War optimally to gain the maximum number of
 * points assuming that both players are playing War. What will Naomi's
 * score be? What would it have been if she had played War optimally
 * instead? 
 *
 * =====================================================================
*/
    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define EPSILON 0.000005

typedef struct hand {
    double *elems;
    int num;
} hand;

typedef struct play {
    double claim;
    double actual;
} play;

/*
 * Remove the given index from the hand. Returns 0 on success or -1
 * on failure.
 */
int remove_index(hand *hand, int index)
{
    int ret = -1;
    if(index < 0 || index >= hand->num)
        goto out;
    ret = 0;
    if(index == hand->num - 1)
        goto dec;
    /* Shift the rest of the elements back */ 
    memmove(&hand->elems[index], &hand->elems[index+1], 
            sizeof(double) * (hand->num - index - 1));
dec:
    hand->num--;
out:
    return ret;
}

int max(hand *hand)
{
    int ind = 0;
    double max = INT_MIN;
    int len = hand->num;
    while(--len >= 0)
        if(max < hand->elems[len]) {
            max = hand->elems[len];
            ind = len;
        }   
    return ind;
}

int min(hand *hand)
{
    int ind = 0;
    double min = INT_MAX;
    int len = hand->num;
    while(--len >= 0)
        if(min > hand->elems[len]) {
            min = hand->elems[len];
            ind = len;
        }   
    return ind;
}

/* 
 * Returns the index of the smallest element in the hand that will
 * defeat the given played block, or -1 otherwise.
 */
int smallest_winner(hand *hand, double played)
{
    int ret = -1, num = hand->num;
    double smallest_distance = INT_MAX;
    double distance;
    while(--num >= 0) {
        distance = hand->elems[num] - played;
        if(distance > 0 && distance < smallest_distance) {
            ret = num;
            smallest_distance = distance; 
        }
    }
    return ret;
}

/* 
 * Respond fairly to the played block, following Ken's optimal strategy-
 * select the cheapest block that will win, or the cheapest block
 * overall if none can win. 
 *
 * Returns 1 if the round was won, and 0 otherwise. 
 * Returns -1 on failure.
 */ 
int respond_fair(hand *hand, double played)
{
    int ret = -1;
    int selection;
    if(!hand || hand->num <= 0)
        goto out;
    if(played <= 0 || played >= 1)
        goto out;

    ret = 0;
    /* First try to select a winning block */
    selection = smallest_winner(hand, played);
    /* No winner found- select the smallest element */
    if(selection < 0)
        selection = min(hand);
    else
        ret = 1;

    /* Burn the element */
    remove_index(hand, selection);
out:
    return ret;
}

/* 
 * Returns the highest value block from the hand, removing it from the
 * hand's list.
 *
 * Returns { -1, -1 } on failure.
 */ 
play play_fair(hand *my_hand)
{
    play ret = { -1, -1 };
    int my_max;

    if(!my_hand || my_hand->num <= 0)
        goto out;

    /* Fix our maximal and their maximal element */
    my_max = max(my_hand);

    ret.actual = my_hand->elems[my_max];
    ret.claim = ret.actual;

    /* Burn the element */
    remove_index(my_hand, my_max);
out:
    return ret;
}
/* 
 * Returns the best play possible by a cheating player, following
 * the optimal strategy as follows:
 *
 *1) Select the smallest element of ours that will beat the smallest
 *   element of theirs. Claim that our element costs EPSILON more than
 *   their heaviest element. They will play their minimal element, and
 *   we will win.
 *2) If no such element exists, just play our smallest element, but claim
 *   that it costs EPSILON less than their most expensive element. They
 *   will win but it will cost them this element.
 *
 * Returns { -1, -1 } on failure.
 */ 
play play_cheating(hand *my_hand, hand *their_hand)
{
    play ret = { -1, -1 };
    int selection, my_min, their_max, their_min;

    if(!my_hand || my_hand->num <= 0)
        goto out;
    if(!their_hand || their_hand->num <= 0)
        goto out;

    my_min = min(my_hand);
    their_max = max(their_hand);
    their_min = min(their_hand);

    selection = smallest_winner(my_hand, their_hand->elems[their_min]);
    if(selection < 0) {
        selection = my_min;
        ret.claim = their_hand->elems[their_max] - EPSILON; 
    } else {
        ret.claim = their_hand->elems[their_max] + EPSILON;
    }
    ret.actual = my_hand->elems[selection];

    /* Burn the element */
    remove_index(my_hand, selection);
out:
    return ret;
}

int main(int argc, char **argv)
{
    int num_tests, test = 0;
    int ret = 1;
    int n, i;
    int naomi_score, naomi_score_cheating;
    hand ken_hand   = { NULL, 0 };
    hand naomi_hand = { NULL, 0 };
    double *ken_bak, *naomi_bak;
    double *ken_orig, *naomi_orig;

    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 50) {
        printf("Illegal number of tests (1 <= T <= 50)\n");
        goto out;
    }

    while(test++ < num_tests) {
        scanf("%d",&n);
        if(n < 1 || n > 1000) {
            printf("Illegal number of blocks (1 <= N <= 1000)\n");
            continue;
        }
        
        ken_hand.elems = malloc(sizeof(double) * 2 * n);
        if(!ken_hand.elems) {
            perror("malloc");
            goto out;
        }
        naomi_hand.elems = malloc(sizeof(double) * 2 * n);
        if(!naomi_hand.elems) {
            perror("malloc");
            free(ken_hand.elems);
            goto out;
        }
        ken_orig = ken_hand.elems;
        naomi_orig = naomi_hand.elems;
        ken_bak = &ken_hand.elems[n];
        naomi_bak = &naomi_hand.elems[n];
        ken_hand.num = n;
        naomi_hand.num = n;

        for(i = 0; i < n; i++)
            scanf("%lf", &naomi_hand.elems[i]);
        for(i = 0; i < n; i++)
            scanf("%lf", &ken_hand.elems[i]);
        memcpy(naomi_bak, naomi_hand.elems, sizeof(double) * n);
        memcpy(ken_bak, ken_hand.elems, sizeof(double) * n);

        naomi_score = 0;
        naomi_score_cheating = 0;
        i = 0;
        while(i++ < n) {
            play p = play_cheating(&naomi_hand, &ken_hand);
            if(!respond_fair(&ken_hand, p.claim))
                naomi_score_cheating++;
        }
        naomi_hand.elems = naomi_bak;
        ken_hand.elems = ken_bak;
        naomi_hand.num = n;
        ken_hand.num = n;
        i = 0;
        while(i++ < n) {
            play p = play_fair(&naomi_hand);
            if(!respond_fair(&ken_hand, p.claim))
                naomi_score++;
        }

        printf("Case #%d: %d %d\n",test, naomi_score_cheating, 
                naomi_score);
         
        free(ken_orig);
        free(naomi_orig);
        ken_hand.elems = NULL;
        naomi_hand.elems = NULL;
    }
out:
    return ret;
}

