#ifndef POST_H
#define POST_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


enum errors {
    ERROR = -1,
    SUCCESS,
};

// Структура для хранения строки
typedef struct string_t {
    char* str;
    int symbols;
} string_t;

// Структура для хранения адреса
typedef struct address_t {
    string_t town;
    string_t street;
    int house;
    string_t corpus;
    int apartNumber;
    string_t index;
} address_t;

// Структура для хранения почтового отправления
typedef struct mail_t {
    address_t address;
    float weight;
    string_t mailID;
    string_t createdTime;
    string_t deliveryTime;
} mail_t;

// Структура для хранения почтового отделения
typedef struct Post {
    address_t* addres;
    mail_t* mails;
    int count;
    int capacity;
} Post;

// Функции для работы со временем
char* get_current_time();
int validate_idx(char* index);
int strtotime(char* dateStr, struct tm* timeStruct);
int time_cmp(const void* a, const void* b);

// Функции для работы со строками
string_t* str_init(char* str);
int string_delete(string_t* str);
int string_cmp(string_t* s1, string_t* s2);
int string_cpy(string_t* s1, string_t* s2);
string_t* string_icpy(string_t* s);
int string_eq(string_t* s1, string_t* s2);
int string_concatenate(string_t* s1, string_t* s2);

// Функции для работы с почтовыми отправлениями
mail_t* mail_init(address_t* address, float weight, string_t* mailId, string_t* createdTime, string_t* deliveryTime);
int mail_print(mail_t* m);
mail_t* mail_search(Post* p, char* id);
int mail_cmp(const void* a, const void* b);
int mail_sort(mail_t* mails, int size);
int mail_add(Post* p, mail_t* m);
int mail_delete(Post* p, mail_t* m);
int mail_deliver(mail_t* m);

// Функции для работы с адресами
address_t* adress_init(string_t* town, string_t* street, int house, string_t* corpus, int apartNumber, string_t* index);

// Функции для работы с почтовыми отделениями
Post* post_init(address_t* address);
int post_print(Post* p);
int delivery_print(Post* p);
int not_delivered_print(Post* p);
int post_free(Post* p);

#endif
