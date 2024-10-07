#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "return_code.h"

typedef double (*function)(double x);
typedef double (*method_to_calculate)(function f, double a, double b);

int is_double_overflow(double to_check);
enum return_code transfer_to_double(const char* str, double* num);

double func_a(double x);
double func_b(double x);
double func_c(double x); 
double func_d(double x);

double method_sympson(function f, double a, double b);
double method_rect_center(function f, double a, double b);
double method_rect_left(function f, double a, double b);
double method_rect_right(function f, double a, double b);


enum return_code calculate_integral(function f, double a, double b, double eps, method_to_calculate method_to_calculate, double* result) ;
enum return_code calculate_point(function f, double eps, double* result);


#endif