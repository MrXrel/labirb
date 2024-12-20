#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>

#include "string.h"
#include "return_code.h"

typedef struct Address
{
    String city, street, building, index;
    unsigned int house_number, apartment_number;
} Address;

typedef struct Mail
{
    Address address;
    double weight;
    String postal_index;
    String create_time, getting_time;
    char delivered;
} Mail;

typedef struct Post
{
    Mail *mail_arr;
    Address *address_ptr;
    size_t mail_capacity;
} Post;

void print_menu();
enum return_code get_address(Address *address);
enum return_code post_init(Post *post);
enum return_code add_mail(Post *post, int *count_mails);
enum return_code delete_mail(Post *post, int *count_mails);
enum return_code search_mail(Post *post, int *count_mails);
int compare_mails(const void *a, const void *b);
void print_mail_info(Mail *mail);
enum return_code get_postal_index(String *str);
enum return_code set_delivered(Post *post, int *count_mails);
enum return_code search_delivered(Post *post, int count_mails, int *count_delivered, Mail **arr_delivered);
void clear_address(Address *address);
void clear_mail(Mail *mail);
enum return_code search_overdue(Post *post, int count_mails, int *count_overdue, Mail **arr_overdue);
void clear_post(Post *post, int count_mails);
int compare_dates(const char *time_1, const char *time_2);

#endif