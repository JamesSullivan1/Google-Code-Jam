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
 *  In this problem, you start with 0 cookies. You gain cookies at a
 *  rate of 2 cookies per second, by clicking on a giant cookie. Any
 *  time you have at least C cookies, you can buy a cookie farm. Every
 *  time you buy a cookie farm, it costs you C cookies and gives you an
 *  extra F cookies per second.
 *
 *  Once you have X cookies that you haven't spent on farms, you win!
 *  Figure out how long it will take you to win if you use the best
 *  possible strategy. 
 *
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

double solve(double rate, double farm_cost, double farm_rate, double cap)
{
    double time_no_buy;
    double time_buy;
    double ret = 0;

    if(rate < 2) 
        goto out;
    if(farm_cost < 1 || farm_cost > 10000)
        goto out;
    if(farm_rate < 1 || farm_rate > 100)
        goto out;
    if(cap < 1 || cap > 100000)
        goto out;

    time_no_buy = cap / rate; 
    time_buy = farm_cost / rate;
    time_buy += (cap / (rate + farm_rate));
    /* Buy new farms until it would take longer than just waiting */
    while(time_no_buy > time_buy) {
        ret += farm_cost / rate;
        rate += farm_rate;
        time_no_buy = cap / rate;
        time_buy = farm_cost / rate;
        time_buy += (cap / (rate + farm_rate));
    }
    ret += time_no_buy;
out: 
    return ret;
}

int main(void)
{
    int num_tests, test = 0, ret = -1;
    double round, farm_cost, farm_rate, cap;
     
    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 100) {
        printf("Illegal number of tests (1 <= T <= 100)\n");
        goto out;
    }

    while(test++ < num_tests) {
        scanf("%lf %lf %lf",&farm_cost, &farm_rate, &cap);
        round = solve(2, farm_cost, farm_rate, cap);
        printf("Case #%d: %.7f\n",test, round);
    }

    ret = 0;
out:
    return ret;
}
