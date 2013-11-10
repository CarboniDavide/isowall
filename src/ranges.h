#ifndef RANGE_H
#define RANGE_H
#include <stdint.h>

/**
 * A range of either IP addresses or ports
 */
struct Range
{
    unsigned begin;
    unsigned end; /* inclusive */
};

struct RangeList
{
    struct Range *list;
    unsigned count;
    unsigned max;
};

void rangelist_add_range(struct RangeList *task, unsigned begin, unsigned end);
void rangelist_remove_range(struct RangeList *task, unsigned begin, unsigned end);
void rangelist_remove_range2(struct RangeList *task, struct Range range);
int rangelist_is_contains(const struct RangeList *task, unsigned number);

struct Range range_parse_ipv4(const char *line, unsigned *inout_offset, unsigned max);

/**
 * Remove things from the target list. The primary use of this is the 
 * "exclude-file" containing a list of IP addresses that we should 
 * not scan
 * @param targets
 *      Our array of target IP address (or port) ranges that we'll be
 *      scanning.
 * @param excludes
 *      A list, probably read in from --excludefile, of things that we
 *      should not be scanning, that will override anything we otherwise
 *      try to scan.
 * @return
 *      the total number of IP addresses or ports removed.
 */
uint64_t
rangelist_exclude(  struct RangeList *targets, 
              const struct RangeList *excludes);

/**
 * Counts the total number of IP addresses or ports in the target list. This
 * iterates over all the ranges in the table, summing up the count within
 * each range.
 * @param targets
 *      A list of IP address or port ranges.
 * @return
 *      The total number of address or ports.
 */
uint64_t
rangelist_count(const struct RangeList *targets);

/**
 * Given an index in a continous range of [0...count], pick a corresponding
 * number (IP address or port) from a list of non-continuous ranges (not
 * necessarily starting from 0). In other words, given the two ranges
 *    10-19 50-69
 * we'll have a total of 30 possible numbers. Thus, the index goes from 
 * [0..29], with the values 0..9 picking the corresponding values from the
 * first range, and the values 10..29 picking the corresponding values
 * from the second range.
 *
 * NOTE: This is a fundamental part of this program's design, that the user
 * can specify non-contiguous IP and port ranges, but yet we iterate over
 * them using a monotonicly increasing index variable.
 *
 * @param targets
 *      A list of IP address ranges, or a list of port ranges (one or the
 *      other, but not both).
 * @param index
 *      An integer starting at 0 up to (but not including) the value returned
 *      by 'rangelist_count()' for this target list.
 * @return
 *      an IP address or port corresponding to this index.
 */
unsigned rangelist_pick(const struct RangeList *targets, uint64_t i);



void rangelist_free(struct RangeList *list);


unsigned *rangelist_pick2_create(struct RangeList *targets);
void rangelist_pick2_destroy(unsigned *picker);

unsigned rangelist_pick2(const struct RangeList *targets, uint64_t index, const unsigned *picker);

/**
 * Does a regression test of this module
 * @return
 *      0 if the regression test succeeds, or a positive value on failure
 */
int
ranges_selftest(void);


#endif
