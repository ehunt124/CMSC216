#include <stdio.h>

/* Not sure if reusing names of variables for arguments in functions is good 
   style, not in style guide. */

/* Checks if a given char is valid to print (valid are %,#,*) */
int valid_character(char symbol) {
    if(symbol == '#' || symbol == '%' || symbol == '*') {
        return 1;
    } else {
        return 0;
    }
}

/* Draws a rectangle given a specific symbol (#,%,*), width, and length (>0) */
int draw_rectangle(char symbol, int width, int length) {
    int i, j;
    if(valid_character(symbol) && (length > 0) && (width > 0)) {
        for(i = 0; i < width; i++) {
            for(j = 0; j < length; j++) {
                printf("%c", symbol);
            }
            printf("\n");
        }
        return 1;
    } else {
        printf("Invalid data provided.\n");
        return 0;
    }
}

/* Draws a triangle given a specific symbol (#,%,*) and size (>0) */
int draw_triangle(char symbol, int size) {
    if(valid_character(symbol) && (size > 0)) {
        int spaces = size, c_num = 0, i = 0;
        while(spaces > 0) {
            printf("%*c", spaces, symbol); /* Padding instead of using loop */
            for(i = 0; i < c_num; i++) {
                printf("%c", symbol);
            }
            printf("\n");
            spaces--;
            c_num += 2;
        }
        return 1;
    } else {
        printf("Invalid data provided.\n");
        return 0;
    }
}

/* Main asks user to choose a number of a shape to draw or 0 to quit 
   the program */
int main() {
   int running = 1, choice, width, length, size;
   char symbol;

   while(running) {
      printf("Enter 1(rectangle), 2(triangle), 3(other), 0(quit): ");
      scanf(" %d", &choice);
      switch(choice) {
         case 0:
            running = 0;
            printf("Bye Bye.");
            break;
         case 1:
            printf("Enter character, width and length: ");
            scanf(" %c %d %d", &symbol, &width, &length);
            draw_rectangle(symbol, width, length);
            break;
         case 2:
            printf("Enter character and size: ");
            scanf(" %c %d", &symbol, &size);
            draw_triangle(symbol, size);
            break;
         case 3: /* No 3rd option */
            break;
         default:
            printf("Invalid choice.\n");
      }
   }
   return 0;
}