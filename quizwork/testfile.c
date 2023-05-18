#include <stdio.h>

float calc_refund(int print_values);

float calc_refund(int print_values) {
    char choice = 'z';
    float price = 0;
    int amount_read = 0;

    amount_read = scanf("%c%*c%f", &choice, &price);

    if(amount_read =! 3 || (choice != 'O' && choice != 'N')) {
        printf("Invalid input");
        return -1;
    }

    if(print_values) {
        printf("Values provided: %c, %lf\n", choice, price);
    }

    if(choice = 'N') {
        return price;
    } else if(choice = 'O'){
        return price / 8;
    }
}

int main() {
    printf("%f\n", calc_refund(1));
    return 0; 
}
