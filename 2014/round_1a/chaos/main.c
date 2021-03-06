/*
 * Code Jam 2014
 * Round 1A Problem A
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
 *
 * =====================================================================
 *
 * Problem
 *
 * Shota the farmer has a problem. He has just moved into his newly
 * built farmhouse, but it turns out that the outlets haven't been
 * configured correctly for all of his devices. Being a modern farmer,
 * Shota owns a large number of smartphones and laptops, and even owns a
 * tablet for his favorite cow Wagyu to use. In total, he owns N
 * different devices.
 *
 * As these devices have different specifications and are made by a
 * variety of companies, they each require a different electric flow to
 * charge. Similarly, each outlet in the house outputs a specific
 * electric flow. An electric flow can be represented by a string of 0s
 * and 1s of length L.
 *
 * Shota would like to be able to charge all N of his devices at the
 * same time. Coincidentally, there are exactly N outlets in his new
 * house. In order to configure the electric flow from the outlets,
 * there is a master control panel with L switches. The ith switch flips
 * the ith bit of the electric flow from each outlet in the house. For
 * example, if the electric flow from the outlets is:
 *
 * Outlet 0: 10
 * Outlet 1: 01
 * Outlet 2: 11
 *
 * Then flipping the second switch will reconfigure the electric flow
 * to:
 *
 * Outlet 0: 11
 * Outlet 1: 00
 * Outlet 2: 10
 *
 * If Shota has a smartphone that needs flow "11" to charge, a tablet
 * that needs flow "10" to charge, and a laptop that needs flow "00" to
 * charge, then flipping the second switch will make him very happy!
 *
 * Misaki has been hired by Shota to help him solve this problem. She
 * has measured the electric flows from the outlets in the house, and
 * noticed that they are all different. Decide if it is possible for
 * Shota to charge all of his devices at the same time, and if it is
 * possible, figure out the minimum number of switches that needs to be
 * flipped, because the switches are big and heavy and Misaki doesn't
 * want to flip more than what she needs to. 
 *
 * =====================================================================
 */
