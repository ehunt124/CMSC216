#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* init_calendar test:
 * Checks NULL and if days are out of bounds */
static int test1() {
   Calendar *test1;
   int result = SUCCESS;
   /* NULL name */
   if(init_calendar(NULL, 5, comp_minutes, NULL, &test1) != FAILURE) {
      result = FAILURE;  
   }
   /* NULL calendar */
   if(init_calendar("test1", 5, comp_minutes, NULL, NULL) != FAILURE) {
      result = FAILURE; 
   }
   /* Days < 1 */
   if(init_calendar("test1", 0, comp_minutes, NULL, &test1) != FAILURE) {
      result = FAILURE; 
   }
   if(init_calendar("test1", -1, comp_minutes, NULL, &test1) != FAILURE) {
      result = FAILURE; 
   }
   destroy_calendar(test1);
   return result;
}

/* print_calendar test:
 * NULL check */
static int test2() {
   if(print_calendar(NULL, stdout, 1) != FAILURE) {
      return FAILURE;
   }
   return SUCCESS;
}

/* add_event test:
 * NULL check, int checks, event with same name, no comparison function*/
static int test3() {
   Calendar *test3, *test3_no_compare;
   int result = SUCCESS;
   init_calendar("test3", 10, comp_minutes, NULL, &test3);
   /* NULL check */
   if(add_event(NULL, "event", 100, 100, NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   if(add_event(NULL, NULL, 100, 100, NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   if(add_event(test3, NULL, 100, 100, NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   /* Int checks: */
   /* Start time < 0 or > 2400 (inclusive) */
   if(add_event(test3, "start_time0", -1, 100, NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   if(add_event(test3, "start_time1", 2401, 100, NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   if(add_event(test3, "start_time2", 0, 100, NULL, 1) != SUCCESS) {
      result = FAILURE;
   }
   if(add_event(test3, "start_time3", 2400, 100, NULL, 1) != SUCCESS) {
      result = FAILURE;
   }
   /* Duration <= 0 */
   if(add_event(test3, "duration0", 100, 0, NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   if(add_event(test3, "duration1", 100, -1, NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   /* Event with same name */
   add_event(test3, "same_name", 100, 1, NULL, 2);
   if(add_event(test3, "same_name", 100, 1, NULL, 2) != FAILURE) {
      result = FAILURE;
   }
   /* No compare function */
   init_calendar("test3_no_compare", 10, NULL, NULL, &test3_no_compare);
   if(add_event(test3_no_compare, "no_compare", 100, 1, NULL, 2) != FAILURE) {
      result = FAILURE;
   }
   destroy_calendar(test3);
   destroy_calendar(test3_no_compare);
   return result;
}

/* find_event test:
 * NULL checks, event not in calendar */
static int test4() {
   Calendar *test4;
   int result = SUCCESS;
   init_calendar("test4", 10, comp_minutes, NULL, &test4);
   add_event(test4, "crime", 100, 100, NULL, 1);
   /* NULL checks */
   if(find_event(NULL, "crime", NULL) != FAILURE) {
      result = FAILURE;
   }
   if(find_event(test4, NULL, NULL) != FAILURE) {
      result = FAILURE;
   }
   /* Event not in calendar */
   if(find_event(test4, "world_peace", NULL) != FAILURE) {
      result = FAILURE;
   }
   destroy_calendar(test4);
   return result;
}

/* find_event_in_day test:
 * NULL checks, day out of bounds, event not in day */
static int test5() {
   Calendar *test5;
   int result = SUCCESS;
   init_calendar("test5", 10, comp_minutes, NULL, &test5);
   add_event(test5, "crime", 100, 100, NULL, 1);
   /* NULL checks */
   if(find_event_in_day(test5, NULL, 1, NULL) != FAILURE) {
      result = FAILURE;
   }
   if(find_event_in_day(NULL, "crime", 1, NULL) != FAILURE) {
      result = FAILURE;
   }
   /* Day out of bounds */
   if(find_event_in_day(test5, "crime", 50, NULL) != FAILURE) {
      result = FAILURE;
   }
   if(find_event_in_day(test5, "crime", 0, NULL) != FAILURE) {
      result = FAILURE;
   }
   /* Event not in day */
   if(find_event_in_day(test5, "crime", 2, NULL) != FAILURE) {
      result = FAILURE;
   }
   destroy_calendar(test5);
   return result;
}

/* remove_event test:
 * NULL checks, event not in calendar */
static int test6() {
   Calendar *test6;
   int result = SUCCESS;
   init_calendar("test6", 10, comp_minutes, NULL, &test6);
   add_event(test6, "nope", 100, 100, NULL, 1);
   /* NULL check */
   if(remove_event(NULL, "nope") != FAILURE) {
      result = FAILURE;
   }
   if(remove_event(test6, NULL) != FAILURE) {
      result = FAILURE;
   }
   /* event not in calendar */
   if(remove_event(test6, "yep") != FAILURE) {
      result = FAILURE;
   }
   destroy_calendar(test6);
   return result;
}

/* get_event_info test:
 * Checks if name not found */
static int test7() {
   Calendar *test7;
   init_calendar("test7", 3, comp_minutes, NULL, &test7);
   if(get_event_info(test7, "dog_water") == NULL) {
      destroy_calendar(test7);
      return SUCCESS;
   }
   destroy_calendar(test7);
   return FAILURE;
}

/* clear_calendar test:
 * Checks NULL */
static int test8() {
   if(destroy_calendar(NULL) == FAILURE) {
      return SUCCESS;
   }
   return FAILURE;
}

/* clear_day test:
 * Checks NULL and if the days are in the bounds */
static int test9() {
   Calendar *test9;
   int result = SUCCESS;
   init_calendar("test9", 3, comp_minutes, NULL, &test9);
   /* Testing NULL */
   if(clear_day(NULL, 1) != FAILURE) {
      result = FAILURE;
   }
   /* Testing days out of bounds */
   if(clear_day(test9, 0) != FAILURE) {
      result = FAILURE;
   }
   if (clear_day(test9, 10) != FAILURE) {
      result = FAILURE;
   }
   destroy_calendar(test9);
   return result;
}

/* destroy_calendar test:
 * Checks NULL */
static int test10() {
   if(destroy_calendar(NULL) == FAILURE) {
      return SUCCESS;
   }
   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;
   if (test6() == FAILURE) result = FAILURE;
   if (test7() == FAILURE) result = FAILURE;
   if (test8() == FAILURE) result = FAILURE;
   if (test9() == FAILURE) result = FAILURE;
   if (test10() == FAILURE) result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
