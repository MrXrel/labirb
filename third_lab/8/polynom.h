typedef struct node polynom;
typedef struct node node;

struct node {
    int coeff;
    int power;
    node *next;
};

/*create polynom at *targ*/
int poly_init(polynom **targ);

/*delete polynom at *targ*/
int poly_delete(node **targ);

/*reset polynom at *targ to 0*/
int poly_clear(polynom **targ);

/*set coefficients to polynom at *targ*/
int poly_set(polynom **targ, int power, ...);

/*set coefficients to polynom at *targ using string representation*/
int spoly_set(polynom **targ, char *repr);

/*create polynom at *targ and set its coefficients using string representation*/
int spoly_init(polynom **targ, char *repr);

/*print polynom representation*/
int poly_print(polynom *targ);

/*copy polynom from source to *targ*/
int poly_cpy(polynom *source, polynom **targ);


/*add polynoms (a+b)*/
polynom *poly_add(polynom *a, polynom *b);

/*subtract polynoms (a-b)*/
polynom *poly_sub(polynom *a, polynom *b);

/*multiply polynoms (a*b)*/
polynom *poly_mult(polynom *a, polynom *b);

/*divide polynoms (a/b)
Note: if some coeffs in result are supposed to be double, result might be not fully accurate*/
polynom *poly_div(polynom *a, polynom *b);

/*find a mod(b)
Note: if some coeffs in a/b are supposed to be double, result might be negative*/
polynom *poly_mod(polynom *a, polynom *b);

/*evaluate polynom a at x*/
int poly_eval(int x, polynom *a);

/*find d(a)/dx*/
polynom *poly_diff(polynom *a);

/*find polynom that represents composition a(b)*/
polynom *poly_cmps(polynom *a, polynom *b);