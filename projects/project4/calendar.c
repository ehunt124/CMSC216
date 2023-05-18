/* Name: Ethan Hunt  UID: 117171051  DirID: ehunt124 */
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "calendar.h"

int init_calendar(const char *name, int days,
        int (*comp_func) (const void *ptr1, const void *ptr2),
        void (*free_info_func) (void *ptr), Calendar ** calendar) {
    Calendar *new_cal;
    int i;
    /* NULL checks */
    if(calendar == NULL || name == NULL || days < 1) {
        return FAILURE;
    }
    /* Memory allocation */
    new_cal = malloc(sizeof(Calendar));
    if(new_cal == NULL) {
        return FAILURE;
    }
    new_cal->name = malloc(strlen(name) + 1);
    if(new_cal->name == NULL) {
        return FAILURE;
    }
    new_cal->events = calloc(days, sizeof(Event *));
    if(new_cal->events == NULL) {
        return FAILURE;
    }
    /* Initializing every head to NULL for easier implementation */
    for(i = 0; i < days; i++) { 
        new_cal->events[i] = NULL;
    }
    /* Initializing the new Calendar */
    strcpy(new_cal->name, name);
    new_cal->days = days;
    new_cal->total_events = 0;
    new_cal->comp_func = comp_func;
    new_cal->free_info_func = free_info_func;
    /* Returning the new calender */
    *calendar = new_cal;
    return SUCCESS;
}

int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    Event *current = NULL;
    int i;
    /* NULL checks */
    if(output_stream == NULL || calendar == NULL) {
        return FAILURE;
    } else if(print_all) {
        fprintf(output_stream, 
            "Calendar's Name: \"%s\"\nDays: %d\nTotal Events: %d\n\n",
            calendar->name, calendar->days, calendar->total_events);
    }
    fprintf(output_stream, "**** Events ****\n");
    if(calendar->total_events > 0) {
        for(i = 0; i < calendar->days; i++) {
            fprintf(output_stream, "Day %d\n", i + 1);
            current = calendar->events[i];
            while(current != NULL) {
                fprintf(output_stream,
                    "Event's Name: \"%s\", Start_time: %d, Duration: %d\n", 
                    current->name, current->start_time, current->duration_minutes);
                current = current->next;
            }
        }
    }
    return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time,
            int duration_minutes, void *info, int day) {
    Event *new, *current = NULL, *previous = NULL;
    int bool_inserted = 0;
    /* NULL and fail checks */
    if(calendar == NULL || name == NULL || start_time < 0 || start_time > 2400
            || duration_minutes < 1 || day < 1) {
        return FAILURE;
    } else if(day > calendar->days || calendar->comp_func == NULL) {
        return FAILURE;
    } else if(find_event(calendar, name, NULL) == SUCCESS) {
        return FAILURE;
    }
    /* Memory allocation */
    new = malloc(sizeof(Event));
    if(new == NULL) {
        return FAILURE;
    }
    new->name = malloc(strlen(name) + 1);
    if(new->name == NULL) {
        return FAILURE;
    }
    /* Initializing new Event */
    strcpy(new->name, name);
    new->next = NULL;
    new->start_time = start_time;
    new->duration_minutes = duration_minutes;
    new->info = info;
    /* Insertion implementation */
    current = calendar->events[day - 1];
    if(current == NULL) { /* Inserting at the beginning */ 
        calendar->events[day - 1] = new;
        new->next = current;
    } else {
        while (current != NULL) {
            /* Moves forward if it is > current in list */
            if(calendar->comp_func(current, new) < 0) {
                previous = current;
                current = current->next;
            /* Else insert */ 
            } else if(calendar->comp_func(current, new) >= 0 
                    && !bool_inserted) {
                if(previous == NULL) { /* 1 element in list */
                    new->next = current;
                    calendar->events[day - 1] = new;
                } else { /* Normal insert */
                    new->next = current;
                    previous->next = new;
                }
                bool_inserted = 1;
            } else { /* After we insert once, we break to avoid infinte loop */
                break;
            }
        }
        /* If the value is the largest it doesn't insert, 
         * so insert at the end of the list */
        if(!bool_inserted) { 
            previous->next = new;
            new->next = NULL;
        }
    }
    calendar->total_events++;
    return SUCCESS;
}

int find_event(Calendar *calendar, const char *name, Event **event) {
    int i;
    /* NULL checks */
    if(calendar == NULL || name == NULL) {
        return FAILURE;
    }
    /* Go through days and try to find the event, if it isn't found we reach
     * the failure return */
    for(i = 0; i < calendar->days; i++) {
        if(find_event_in_day(calendar, name, i + 1, event) == SUCCESS) {
            return SUCCESS;
        }
    }
    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name, int day,
            Event **event) {
    Event *current = NULL;
    /* NULL checks */
    if(calendar == NULL || name == NULL) {
        return FAILURE;
    } else if(day < 1 || day > calendar->days) {
        return FAILURE;
    }
    current = calendar->events[day - 1];
    while (current != NULL) {
        if(strcmp(current->name, name) == 0) { 
            /* event != NULL so I can search without returning a pointer */
            if(event != NULL) {
                *event = current;
            }
            return SUCCESS;
        }
        current = current->next;
    }
    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {
    Event *current = NULL, *previous = NULL;
    int i;
    /* NULL and failure checks */
    if(calendar == NULL || name == NULL
        || find_event(calendar, name, NULL) == FAILURE) { 
        return FAILURE;
    }
    /* Remove */
    for(i = 0; i < calendar->days; i++) { /* Searching through each day */
        current = calendar->events[i];
        while (current != NULL) { /* Searching through each event */
            if(strcmp(current->name, name) == 0) { /* If we find, remove */
                if(previous == NULL) { /* If we remove the head */
                    calendar->events[i] = current->next;
                } else {
                    previous->next = current->next;
                }
                /* Memory deallocation */
                free(current->name);
                if(calendar->free_info_func != NULL && current->info != NULL) {
                    calendar->free_info_func(current->info);
                }
                free(current);
                calendar->total_events--;
                return SUCCESS;
            } else { /* Move forward in the list */
                previous = current;
                current = current->next;
            }
        }
    }
    /* If not found we return failure */
    return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {
    /* Using pointer to NULL pointer so it can work with the 
     * event != NULL check in find_event */
    Event *nulp = NULL;
    Event **info_event = &nulp;
    if(find_event(calendar, name, info_event) == FAILURE) {
        return NULL;
    }
    return (*info_event)->info;
}

int clear_calendar(Calendar *calendar) {
    int i;
    /* NULL checks */
    if(calendar == NULL) {
        return FAILURE;
    }
    /* Looks through each day and clear */ 
    for(i = 0; i < calendar->days; i++) {
        clear_day(calendar, i + 1);
    }
    return SUCCESS;
}

int clear_day(Calendar *calendar, int day) {
    Event *current = NULL;
    /* NULL checks */
    if(calendar == NULL) {
        return FAILURE;
    } else if(day < 1 || day > calendar->days) {
        return FAILURE;
    }
    /* Goes through the day and removes each event */
    current = calendar->events[day - 1];
    while(current != NULL) {
        remove_event(calendar, current->name);
        current = calendar->events[day - 1];
    }
    return SUCCESS;
}

int destroy_calendar(Calendar *calendar) {
    /* Same function as clear_calendar but this one deallocates the calendar */
    if(calendar == NULL) {
        return FAILURE;
    }
    clear_calendar(calendar);
    free(calendar->events);
    free(calendar->name);
    free(calendar);
    return SUCCESS;
}
