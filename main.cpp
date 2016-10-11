#include <gmp.h>
#include <stdlib.h>

void print_division(mpz_t idivisor, mpz_t idivident, unsigned long base, unsigned long d) {
    mpz_t factor;
    mpz_t divisor;
    mpz_t quotient;
    mpz_t remainder;
    mpz_t fracture;

    // in fraction
    unsigned long expected_digits = d;

    mpz_inits(factor, divisor, quotient, remainder, fracture, NULL);

    mpz_ui_pow_ui(factor, base, d); // factor = base ^ d
    mpz_mul(divisor, idivisor, factor); // divisor = idivisor * factor
    mpz_tdiv_q(quotient, divisor, idivident); // quotient = divisor / idivident

    mpz_tdiv_q(fracture, quotient, factor); // fracture = floor( quotient/factor )
    mpz_abs(quotient, quotient);
    mpz_mod(remainder, quotient, factor); // remainder = |quotient| mod factor

    while (mpz_divisible_ui_p(remainder, 10) && mpz_sgn(remainder) != 0) {
        mpz_tdiv_q_ui(remainder, remainder, 10);
        expected_digits--;
    }

    gmp_printf("%Zd", fracture);

    if (mpz_sgn(remainder) != 0) {

        gmp_printf(".%0.*Zd", expected_digits, remainder);
    }

    mpz_clears(factor, divisor, quotient, remainder, fracture, NULL);
}

struct ListElement {
    mpz_t number;
    ListElement* next;
};

ListElement *first = NULL;
ListElement *last = NULL;
unsigned long list_length = 0;

void add_elem(ListElement *elem) {
    if(first == NULL) {
        first = elem;
        last = elem;
    }
    else {
        last->next = elem;
        last = elem;
    }
    last->next = first;
    list_length++;
}

ListElement *next(ListElement *elem) {
    return elem->next;
}

unsigned long get_period(ListElement *first)
{
    ListElement *tortoise;
    ListElement *hare;
    unsigned long mu = 0;
    unsigned long lam = 0;

    tortoise = next(first);
    hare = next(next(first));

    while(mpz_cmp(tortoise->number, hare->number) != 0) {
        tortoise = next(tortoise);
        hare = next(next(hare));
    }

    mu = 0;
    tortoise = first;
    while(mpz_cmp(tortoise->number, hare->number) != 0) {
        tortoise = next(tortoise);
        hare = next(hare);
        mu+=1;
    }

    lam = 1;
    hare = next(tortoise);
    while(mpz_cmp(tortoise->number, hare->number) != 0) {
        hare = next(hare);
        lam++;
    }

    return lam;
}

int main(int argc, char *argv[]) {
    mpz_t sum;
    mpz_t squaresSum;
    mpz_t square;
    mpz_t newVal;
    mpz_t n;

    unsigned long d = strtoul(argv[1], NULL, 0);
    unsigned long count = 0;

    mpz_inits(sum, squaresSum, square, newVal, n, NULL);

#ifdef CLION
    freopen("Zadanie1/0/z1_10.txt", "r", stdin);
#endif

    while (gmp_scanf("%Zd", newVal) > 0) {
#ifdef DEBUG
        gmp_printf("%d: %Zd\n",list_length,  newVal);
#endif
        ListElement *newElem = (ListElement*)malloc(sizeof(ListElement));
        mpz_init(newElem->number);
        mpz_set(newElem->number, newVal);

        add_elem(newElem);

        mpz_add(sum, sum, newVal);

        mpz_mul(square, newVal, newVal);
        mpz_add(squaresSum, squaresSum, square);

        count++;
    }

    // Print mean
    mpz_set_ui(n, count);
    print_division(sum, n, 10, d);

    printf("\n");
    //Print Variance ( Var = ((n* Sum of squares) - sum ^ 2) / n ^2
    mpz_mul(squaresSum, squaresSum, n);
    mpz_mul(sum, sum, sum);
    mpz_sub(squaresSum, squaresSum, sum);
    mpz_mul(n,n,n);
    print_division(squaresSum, n, 10, d);

    // print cycle period
    printf("\n%ld\n", get_period(first));

    return 0;
}