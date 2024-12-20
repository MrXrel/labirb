#include "mail.h"

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

typedef struct Address {
    String city;    // not empty
    String street;  // not empty
    unsigned int house;
    String corpus;  // any
    unsigned int flat_number;
    String index;  // 6 numbers
} Address;

typedef struct Mail {
    Address address;
    double weight;         // >0
    String mail_id;        // 14 numbers
    String time_creation;  // “dd:MM:yyyy hh:mm:ss
    String time_arriving;  // “dd:MM:yyyy hh:mm:ss
} Mail;

typedef struct Post {
    Address* address;
    int mail_count;
    Mail* mails;
} Post;


int transfer_to_uint(const char* str, unsigned int* result) {
    if (!str || !result) {
        return BAD_POINTER_ERROR;
    }
    int len = strlen(str);
    *result = 0;
    for (int i = 0; i < len; i++) {
        if ((i == 0) && str[i] == '-') {
            return BAD_INPUT_ERROR;
        }
        if (!(str[i] >= '0' && str[i] <= '9')) {
            return NOT_AN_INTEGER_ERROR;
        }
        if (*result > (INT_MAX - (str[i] - '0')) / 10) {
            return OVERFLOW_ERROR;
        }

        *result *= 10;
        *result += (str[i] - '0');
    }
    return OK;
}

enum return_code write_address(Address* addr, char const* city, char const* street, char const* house, char const* corpus, char const* flat, char const* index) {
    if (!addr || !city || !street || !house || !corpus || !flat || !index) {
        return BAD_POINTER_ERROR;
    }
    if (strlen(city) == 0) {
        return BAD_CITY_ERROR;
    }
    if (strlen(street) == 0) {
        return BAD_STREET_ERROR;
    }
    String city_string, street_string;
    int res1 = create_string_object(city, &city_string);
    if (res1 != OK) {
        return res1;
    }
    int res2 = create_string_object(street, &street_string);
    if (res2 != OK) {
        clear_string(&city_string);
        return res2;
    }
    int house_humber;
    int res3 = transfer_to_uint(house, &house_humber);
    if (res3 != OK) {
        clear_string(&city_string);
        clear_string(&street_string);
        return BAD_HOUSE_ERROR;
    }
    String corpus_string;
    int res4 = create_string_object(corpus, &corpus_string);
    if (res4 != OK) {
        clear_string(&city_string);
        clear_string(&street_string);
        return res4;
    }

    int flat_number;
    int res5 = transfer_to_uint(flat, &flat_number);
    if (res4 != OK) {
        clear_string(&city_string);
        clear_string(&street_string);
        clear_string(&corpus_string);
        return BAD_FLAT_ERROR;
    }

    if (strlen(index) != 6) {
        clear_string(&city_string);
        clear_string(&street_string);
        clear_string(&corpus_string);
        return BAD_INDEX_ERROR;
    }
    String index_string;
    int res6 = create_string_object(index, &index_string);
    if (res6 != OK) {
        clear_string(&city_string);
        clear_string(&street_string);
        clear_string(&corpus_string);
        return res6;
    }

    addr->city = city_string;
    addr->street = street_string;
    addr->house = house_humber;
    addr->corpus = corpus_string;
    addr->flat_number = flat_number;
    addr->index = index_string;

    return OK;
}

int is_double_overflow(double to_check) {
    if ((fabs(to_check) > DBL_MAX) || to_check == HUGE_VAL || to_check == -HUGE_VAL || isnan(to_check) || isinf(to_check)) {
        return 1;
    }
    return 0;
}

int transfer_to_double(const char* str, double* num) {
    if (!str || !num) {
        return BAD_POINTER_ERROR;
    }
    char* endptr;
    double value = strtod(str, &endptr);

    if (*endptr != '\0') {
        return NOT_A_DOUBLE_ERROR;
    }
    if (is_double_overflow(value)) {
        return OVERFLOW_ERROR;
    }
    *num = value;
    return 0;
}

enum return_code write_weight(double* dest, char const* str) {
    if (!dest || !str) {
        return BAD_POINTER_ERROR;
    }
    double number;
    int res = transfer_to_double(str, &number);
    if (res != OK) {
        return res;
    }
    if (number <= 0) {
        return BAD_INPUT_ERROR;
    }
    *dest = number;
    return OK;
}

enum return_code write_mail_id(String* dest, char const* str) {
    if (!str || !dest) {
        return BAD_POINTER_ERROR;
    }
    int len = strlen(str);
    if (len != 14) {
        return BAD_INPUT_ERROR;
    }
    for (int i = 0; i < 14; ++i) {
        if (!isdigit(str[i])) {
            return BAD_INPUT_ERROR;
        }
    }

    int res = create_string_object(str, dest);
    return res;
}

enum return_code write_date_to_String(String* dest, char const* str) {
    if (!str || !dest) {
        return BAD_POINTER_ERROR;
    }

    int is_valid = 0;
    is_valid_date(str, &is_valid);
    if (!is_valid) {
        return BAD_INPUT_ERROR;
    }
    int res = create_string_object(str, dest);
    return res;
}

enum return_code is_valid_date(const char* str, int* is_valid) {
    if (!str || !is_valid) {
        return MEMORY_ALLOC_ERROR;
    }
    *is_valid = 0;

    if (strlen(str) != 19) {
        return OK;
    }

    for (int i = 0; i < 19; i++) {
        if (i == 2 || i == 5 || i == 13 || i == 16) {
            if (str[i] != ':') {
                return OK;
            }
        } else if (i == 10) {
            if (str[i] != ' ') {
                return OK;
            }
        } else {
            if (!isdigit(str[i])) {
                return OK;
            }
        }
    }

    int day = (str[0] - '0') * 10 + (str[1] - '0');
    int month = (str[3] - '0') * 10 + (str[4] - '0');
    int year = (str[6] - '0') * 1000 + (str[7] - '0') * 100 + (str[8] - '0') * 10 + (str[9] - '0');
    int hour = (str[11] - '0') * 10 + (str[12] - '0');
    int minute = (str[14] - '0') * 10 + (str[15] - '0');
    int second = (str[17] - '0') * 10 + (str[18] - '0');

    if (day < 1 || day > 31 || month < 1 || month > 12 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return OK;
    }

    int isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    int daysInMonth[] = {31, (isLeap ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day > daysInMonth[month - 1]) {
        return OK;
    }
    *is_valid = 1;
    return OK;
}

int compare_mails(const void* a, const void* b) {
    const Mail* mail1 = (const Mail*)a;
    const Mail* mail2 = (const Mail*)b;

    // Сравниваем индекс получателя
    int index_cmp = strcmp(mail1->address.index, mail2->address.index);
    if (index_cmp != 0) {
        return index_cmp;
    }

    // Если индексы совпадают, сравниваем идентификаторы посылки
    return strcmp(mail1->mail_id, mail2->mail_id);
}

// Функция для сортировки посылок в почтовом отделении
void sort_mails(Post* post) {
    qsort(post->mails, post->mail_count, sizeof(Mail), compare_mails);
}

/* Реализуйте интерактивный диалог с пользователем, предоставляющий
 функционал для добавления и удаления объектов структур типа Mail в объект
 структуры типа Post, информативный вывод данных об отправлении при поиске
 объекта типа Mail по идентификатору. Объекты структуры Mail должны быть
 отсортированы по индексу получателя (первично) и идентификатору посылки
 (вторично) в произвольный момент времени. Также в интерактивном диалоге
 реализуйте опции поиска всех доставленных отправлений, а также всех отправлений,
 срок доставки которых на текущий момент времени (системное время) истёк.
 Информацию о доставленных/недоставленных отправлениях выводите в порядке
 времени создания по возрастанию (от старых к новым). Для хранения строковых
 данных используйте структуру String из п. 1.
*/