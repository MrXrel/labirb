#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "polynom.h"

int is_num(char c) {
    return (c >= '0') && (c <= '9');
}

int bin_pow(double base, int ind, double *res) {
    if (ind == 0) {
        *res = 1;
        return 0;
    }
    int _ind = ind;
    if (ind < 0) {
        _ind = ~ind + 1;
    }

    *res = 1.;

    double rec_res;
    bin_pow(base, _ind >> 1, &rec_res);
    *res = rec_res * rec_res;
    if (_ind & 1) {
        *res *= base;
    }

    if (ind < 0) {
        *res = 1 / *res;
    }
    return 0;
}

int create_node(node **_node) {
    *_node = (node *)malloc(sizeof(node));
    if (*_node == NULL) {
        printf("memory error\n");
        return -1;
    }

    (*_node)->coeff = 0;
    (*_node)->power = 0;
    (*_node)->next = NULL;

    return 0;
}

int poly_init(polynom **poly) {
    return create_node(poly);
}

int delete_node(node **node) {
    if (((*node)->next) != NULL) {
        delete_node(&((*node)->next));
    }
    free(*node);
    node = NULL;
    return 0;
}

int poly_delete(polynom **poly) {
    return delete_node(poly);
}

int poly_clear(polynom **poly) {
    if (((*poly)->next) != NULL) {
        delete_node(&((*poly)->next));
    }
    (*poly)->coeff = 0;
    (*poly)->power = 0;
    (*poly)->next = NULL;
    return 0;
}

int poly_add_node(polynom **poly, int _coeff, int _pow) {
    node *new = NULL;
    if (create_node(&new)) {
        return -1;
    }
    new->coeff = _coeff;
    new->power = _pow;

    if (_pow > ((*poly)->power)) {
        new->next = &(**poly);
        *poly = new;
    } else {
        node **w = poly;

        while ((*w) != NULL) {
            if (_pow == (*w)->power) {
                (*w)->coeff += _coeff;
                free(new);
                return 0;
            }

            if (_pow > (*w)->power) {
                break;
            }
            w = &((*w)->next);
        }

        new->next = *w;
        *w = new;
    }
    return 0;
}

int poly_set(polynom **poly, int pow, ...) {
    poly_clear(poly);

    va_list coeffs;
    va_start(coeffs, pow);

    (*poly)->coeff = va_arg(coeffs, int);
    (*poly)->power = pow;

    node *w;
    w = *poly;

    for (int i = pow - 1; i > -1; --i) {
        if (create_node(&(w->next))) {
            va_end(coeffs);
            return -1;
        }
        w = w->next;
        w->coeff = va_arg(coeffs, int);
        w->power = i;
    }

    va_end(coeffs);
    return 0;
}

int spoly_set(polynom **poly, char *repr) {
    poly_clear(poly);

    int coeff = 1;
    int pow = 0;
    int sign = 1;
    int pow_sign = 1;
    int buff;

    char *ptr = repr;
    if (*ptr == '-') {
        sign = -1;
        ++ptr;
    }

    while (1) {
        switch (*ptr) {
            case 'x':
                pow += 1;
                break;

            case '^':
                buff = pow - 1;
                pow = 0;
                if (ptr[1] == '-') {
                    pow_sign = -1;
                    ++ptr;
                }

                while (is_num(ptr[1])) {
                    ++ptr;
                    pow *= 10;
                    pow += *ptr - '0';
                }
                pow += buff;
                break;

            case '+':
                if (coeff > 0) {
                    if (poly_add_node(poly, sign * coeff, pow_sign * pow)) {
                        return -1;
                    }
                }
                pow = 0;
                sign = 1;
                coeff = 1;
                pow_sign = 1;

                break;

            case '-':
                if (coeff > 0) {
                    if (poly_add_node(poly, sign * coeff, pow_sign * pow)) {
                        return -1;
                    }
                }
                pow = 0;
                sign = -1;
                coeff = 1;
                pow_sign = 1;

                break;

            case 0:
                if (coeff > 0) {
                    if (poly_add_node(poly, sign * coeff, pow_sign * pow)) {
                        return -1;
                    }
                }
                return 0;

            default:
                if (is_num(*ptr)) {
                    buff = coeff;
                    coeff = 0;
                    while (is_num(*ptr)) {
                        coeff *= 10;
                        coeff += *ptr - '0';
                        ++ptr;
                    }
                    coeff *= buff;
                    --ptr;

                } else {
                    printf("invalid arg\n");
                    return -1;
                }
                break;
        }
        ++ptr;
    }
    return 0;
}

int spoly_init(polynom **poly, char *repr) {
    if (poly_init(poly)) {
        return -1;
    }
    return spoly_set(poly, repr);
}

int poly_print(polynom *poly) {
    node *r = poly;
    int is_begin = 1;

    if ((r->next == NULL) && (r->coeff == 0)) {
        printf("0\n");
        return 0;
    }

    while (r != NULL) {
        if (r->coeff) {
            is_begin = 0;
            if (r->power) {
                switch (r->coeff) {
                    case 1:
                        putchar('x');
                        break;
                    case -1:
                        printf("-x");
                        break;
                    default:
                        printf("%dx", r->coeff);
                        break;
                }

                if (r->power != 1) {
                    printf("^%d", r->power);
                }
            } else {
                printf("%d", r->coeff);
            }
        }

        if (!is_begin) {
            if ((r->next != NULL) && (r->next->coeff > 0)) {
                putchar('+');
            }
        }
        r = r->next;
    }
    return 0;
}

int poly_cpy(polynom *source, polynom **targ) {
    if (poly_init(targ)) {
        return -1;
    }
    polynom *r = source;
    while (r != NULL) {
        if (poly_add_node(targ, r->coeff, r->power)) {
            poly_delete(targ);
            return -1;
        }
        r = r->next;
    }
    return 0;
}

polynom *poly_add(polynom *a, polynom *b) {
    polynom *result;
    if (poly_init(&result)) {
        return NULL;
    }

    polynom *left = a;
    polynom *right = b;

    while (left != NULL) {
        if (poly_add_node(&result, left->coeff, left->power)) {
            poly_delete(&result);
            return NULL;
        }
        left = left->next;
    }
    while (right != NULL) {
        if (poly_add_node(&result, right->coeff, right->power)) {
            poly_delete(&result);
            return NULL;
        }
        right = right->next;
    }

    return result;
}

polynom *poly_sub(polynom *a, polynom *b) {
    polynom *result;
    if (poly_init(&result)) {
        return NULL;
    }

    polynom *left = a;
    polynom *right = b;

    while (left != NULL) {
        if (poly_add_node(&result, left->coeff, left->power)) {
            poly_delete(&result);
            return NULL;
        }
        left = left->next;
    }
    while (right != NULL) {
        if (poly_add_node(&result, -right->coeff, right->power)) {
            poly_delete(&result);
            return NULL;
        }
        right = right->next;
    }

    return result;
}

polynom *poly_mult(polynom *a, polynom *b) {
    polynom *result;
    if (poly_init(&result)) {
        return NULL;
    }

    polynom *left = a;
    polynom *right = b;

    while (left != NULL) {
        right = b;

        while (right != NULL) {
            if (poly_add_node(&result, (left->coeff * right->coeff), (left->power + right->power))) {
                poly_delete(&result);
                return NULL;
            }
            right = right->next;
        }

        left = left->next;
    }

    return result;
}

polynom *poly_div(polynom *a, polynom *b) {
    polynom *result;
    if (poly_init(&result)) {
        return NULL;
    }

    polynom *numerator;
    if (poly_cpy(a, &numerator)) {
        return NULL;
    }
    polynom *r = numerator;
    polynom *buff;
    polynom *denominator = b;

    while (denominator->coeff == 0) {
        denominator = denominator->next;
        if (denominator == NULL) {
            poly_delete(&result);
            poly_delete(&numerator);
            printf("zero division error\n");
            return NULL;
        }
    }

    while (r != NULL && r->power >= denominator->power) {
        if (r->coeff / denominator->coeff == 0) {
            r = r->next;
        } else {
            if (poly_add_node(&result, r->coeff / denominator->coeff, r->power - denominator->power)) {
                poly_delete(&result);
                poly_delete(&numerator);
                return NULL;
            }
            poly_delete(&numerator);

            buff = poly_mult(denominator, result);
            numerator = poly_sub(a, buff);
            poly_delete(&buff);

            r = numerator;
        }
    }

    poly_delete(&numerator);
    return result;
}

polynom *poly_mod(polynom *a, polynom *b) {
    polynom *div = poly_div(a, b);
    polynom *mult = poly_mult(div, b);
    polynom *result = poly_sub(a, mult);
    poly_delete(&div);
    poly_delete(&mult);
    return result;
}

int poly_eval(int x, polynom *a) {
    polynom *r = a;
    int result = 0;
    double bu_ispugalsa;

    while (r != NULL) {
        bin_pow((double)x, r->power, &bu_ispugalsa);
        result += (int)bu_ispugalsa * r->coeff;
        r = r->next;
    }
    return result;
}

polynom *poly_diff(polynom *a) {
    polynom *r = a;
    polynom *result;
    if (poly_init(&result)) {
        return NULL;
    }

    while ((r->power != 0) && (r != NULL)) {
        if (poly_add_node(&result, r->coeff * r->power, r->power - 1)) {
            poly_delete(&result);
            return NULL;
        }
        r = r->next;
    }
    return result;
}

polynom *poly_cmps(polynom *a, polynom *b) {
    polynom *outter = a;
    polynom *inner = b;
    polynom *pow;
    polynom *buff;
    if (spoly_init(&pow, "1")) {
        return NULL;
    }
    polynom *result;
    if (poly_init(&result)) {
        poly_delete(&pow);
        return NULL;
    }

    if (spoly_set(&pow, "1")) {
        poly_delete(&pow);
        poly_delete(&result);
        return NULL;
    }
    for (int i = 0; i < outter->power; ++i) {
        poly_cpy(pow, &buff);
        poly_delete(&pow);
        pow = poly_mult(buff, inner);
        poly_delete(&buff);
    }

    while (outter != NULL) {
        for (int i = 0; i < outter->coeff; ++i) {
            buff = poly_add(result, pow);
            poly_delete(&result);
            result = buff;
        }
        for (int i = 0; i > outter->coeff; --i) {
            buff = poly_sub(result, pow);
            poly_delete(&result);
            result = buff;
        }
        poly_cpy(pow, &buff);
        poly_delete(&pow);
        pow = poly_div(buff, inner);
        poly_delete(&buff);
        outter = outter->next;
    }
    poly_delete(&pow);
    return result;
}

typedef polynom *(*callback)(polynom *, polynom *);

int skip_short_comment(FILE *file, char *c) {
    while ((*c) != '\n' && (*c) != EOF) {
        (*c) = getc(file);
    }
    (*c) = getc(file);
    return 0;
}

int skip_long_comment(FILE *file, char *c) {
    while ((*c) != ']' && (*c) != EOF) {
        (*c) = getc(file);
    }
    if ((*c) != ']') {
        printf("syntax error\n");
        return -1;
    }
    (*c) = getc(file);
    return 0;
}

int read_until_char(FILE *file, char *c, char *targ, char *stop, int allow_spaces) {
    int i = 0;
    int n = strlen(stop);
    int f = 0;
    while (1) {
        if ((*c) == EOF) {
            f = 1;
            break;
        }
        for (int j = 0; j < n; ++j) {
            if ((*c) == stop[j]) {
                f = 1;
                break;
            }
        }
        if (f) {
            break;
        }
        if (!allow_spaces || ((*c) != ' ' && (*c) != '\t' && (*c) != '\n')) {
            targ[i] = (*c);
            ++i;
        }
        (*c) = getc(file);
    }
    targ[i] = 0;
    return 0;
}

int read_command(FILE *file, polynom **a, polynom **b) {
    char c = ' ';
    char cmd[5];
    char poly_repr[64];

    while (c == ' ' || c == '\t' || c == '\n') {  // skip spaces, tabs, \n's and comments
        c = getc(file);
        if (c == '%') {
            if (skip_short_comment(file, &c)) {
                if (*a != NULL) {
                    poly_delete(a);
                }
                if (*b != NULL) {
                    poly_delete(b);
                }
                return -1;
            }
        }
        if (c == '[') {
            if (skip_long_comment(file, &c)) {
                if (*a != NULL) {
                    poly_delete(a);
                }
                if (*b != NULL) {
                    poly_delete(b);
                }
                return -1;
            }
        }
        if (c == EOF) {
            return -2;
        }
    }

    read_until_char(file, &c, cmd, "(", 0);  // read command

    c = getc(file);
    if (c != ')' && c != EOF) {
        read_until_char(file, &c, poly_repr, ",)", 1);  // read first poly

        if (spoly_init(a, poly_repr)) {
            return -1;
        }
    } else {
        *a = NULL;
    }

    if (c == ',') {
        c = getc(file);
        read_until_char(file, &c, poly_repr, ")", 1);  // read second poly

        if (spoly_init(b, poly_repr)) {
            return -1;
        }
    } else {
        *b = NULL;
    }

    c = getc(file);
    while (c != ';') {  // find line end
        if (c != ' ' && c != '\t' && c != '\n') {
            if (*a != NULL) {
                poly_delete(a);
            }
            if (*b != NULL) {
                poly_delete(b);
            }
            printf("syntax error\n");
            return -1;
        }
        c = getc(file);
    }

    if (!strcmp("Add", cmd)) {
        return 0;
    } else if ((!strcmp("Sub", cmd))) {
        return 1;
    } else if ((!strcmp("Mult", cmd))) {
        return 2;
    } else if ((!strcmp("Div", cmd))) {
        return 3;
    } else if ((!strcmp("Mod", cmd))) {
        return 4;
    } else if ((!strcmp("Cmps", cmd))) {
        return 5;
    } else if ((!strcmp("Eval", cmd))) {
        return 6;
    } else if ((!strcmp("Diff", cmd))) {
        return 7;
    } else {
        if (*a != NULL) {
            poly_delete(a);
        }
        if (*b != NULL) {
            poly_delete(b);
        }
        printf("command error\n");
        return -1;
    }
}

int main() {
    FILE *file_in;
    if ((file_in = fopen("test.txt", "r")) == NULL) {
        printf("file error\n");
        return -1;
    }

    callback func[7] = {poly_add, poly_sub, poly_mult, poly_div, poly_mod, poly_cmps, poly_cmps};

    polynom *adder;
    if (poly_init(&adder)) {
        return -1;
    }
    polynom *buff;
    polynom *a;
    polynom *b;

    int cmd = -1;
    while (cmd != -2) {
        cmd = read_command(file_in, &a, &b);
        if (cmd == -2) {
            break;
        }
        if (cmd == -1) {
            poly_delete(&adder);
            if (a != NULL) {
                poly_delete(&a);
            }
            if (b != NULL) {
                poly_delete(&b);
            }
            return -1;
        }

        if (b != NULL) {
            if (cmd < 6) {
                poly_delete(&adder);
                adder = func[cmd](a, b);

            } else if (cmd == 6) {
                buff = func[cmd](a, b);
                printf("P(");
                poly_print(b);
                printf(") = ");
                poly_print(buff);
                printf("; ");
                poly_delete(&buff);

            } else {
                poly_delete(&adder);
                if (a != NULL) {
                    poly_delete(&a);
                }
                if (b != NULL) {
                    poly_delete(&b);
                }
                printf("too many arguments in function call\n");
                return -1;
            }

        } else if (a != NULL) {
            if (cmd < 6) {
                buff = func[cmd](adder, a);
                poly_delete(&adder);
                adder = buff;

            } else if (cmd == 6) {
                buff = func[cmd](adder, a);
                printf("Adder(");
                poly_print(a);
                printf(") = ");
                poly_print(buff);
                printf("; ");
                poly_delete(&buff);

            } else if (cmd == 7) {
                buff = poly_diff(a);
                poly_delete(&adder);
                adder = buff;
            }

        } else if (cmd == 7) {
            buff = poly_diff(adder);
            poly_delete(&adder);
            adder = buff;

        } else {
            poly_delete(&adder);
            if (a != NULL) {
                poly_delete(&a);
            }
            if (b != NULL) {
                poly_delete(&b);
            }
            printf("too few arguments in function call\n");
            return -1;
        }

        if (a != NULL) {
            poly_delete(&a);
        }
        if (b != NULL) {
            poly_delete(&b);
        }
        if (adder == NULL) {
            return -1;
        }
        printf("Adder value: ");
        poly_print(adder);
        putchar('\n');
    }
    poly_delete(&adder);
    return 0;
}