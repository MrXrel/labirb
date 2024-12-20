#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "functions.h"

char* get_current_time() {
    time_t now;
    time(&now);

    struct tm* local = localtime(&now);
    char* buffer = (char*)malloc(20 * sizeof(char)); 
    if (buffer == NULL) {
        return NULL;
    }

    strftime(buffer, 20, "%d:%m:%Y %H:%M:%S", local);
    return buffer;
}

int validate_idx(char* index) {
    if (strlen(index) == 6 || strlen(index) == 14) {
        for (int i = 0; i < (int)strlen(index); i++) {
            if (!isdigit(index[i])) {
                return ERROR;
            }
        }
        return SUCCESS;
    }
    return ERROR;
}

int strtotime(char* dateStr, struct tm* timeStruct) {
    if (sscanf(dateStr, "%d:%d:%d %d:%d:%d", 
               &timeStruct->tm_mday, &timeStruct->tm_mon, &timeStruct->tm_year,
               &timeStruct->tm_hour, &timeStruct->tm_min, &timeStruct->tm_sec) != 6) {
        return ERROR;
    }

    timeStruct->tm_mon -= 1;
    
    timeStruct->tm_year -= 1900;

    return SUCCESS;
}

int time_cmp(const void* a, const void* b) {
    mail_t* m1 = (mail_t*)a;
    mail_t* m2 = (mail_t*)b;
    struct tm t1 = {0};
    struct tm t2 = {0};
    strtotime(m1->createdTime.str, &t1);
    strtotime(m2->createdTime.str, &t2);
    
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    
    if (time1 < time2) {
        return -1;
    } else if (time1 > time2) {
        return 1;
    }
    return 0;
}


//Функции String
string_t* str_init(char* str) {
    string_t* s = (string_t*)malloc(sizeof(string_t));
    if (s == NULL) {
        return NULL;
    }
    char* ptr = (char*)malloc((strlen(str) + 1) * sizeof(char));
    if (ptr == NULL) {
        return NULL;
    }
    strcpy(ptr, str);
    s->str = ptr;
    s->symbols = strlen(str);
    return s;
}

int string_delete(string_t* str) {
    str->str[0] = '\0';
    str->symbols = 0;
    return SUCCESS;
}

int string_cmp(string_t* s1, string_t* s2) {
    if (s1->symbols > s2->symbols) {
        return 1;
    } else if (s1->symbols == s2->symbols) {
        return strcmp(s1->str, s2->str);
    } else {
        return 0;
    }
}

int string_cpy(string_t* s1, string_t* s2) {
    if (s1->str == NULL || s2->str == NULL) {
        return ERROR;
    }

    strcpy(s2->str, s1->str);
    s2->symbols = s1->symbols;
    return SUCCESS;
}


string_t* string_icpy(string_t* s) {
    if (s == NULL) {
        return NULL;
    }
    string_t* new_s = str_init(s->str);
    if (new_s == NULL) {
        return NULL;
    }
    return new_s;
}

int string_eq(string_t* s1, string_t* s2) {
    return (s1->symbols == s2->symbols) && (strcmp(s1->str,s2->str) == 0);
}

int string_concatenate(string_t* s1, string_t* s2) {
    if (s1->str == NULL || s2->str == NULL) {
        return ERROR;
    }
    strcat(s1->str, s2->str);
    s1->symbols += s2->symbols;
    return SUCCESS;
}

//Функции Mail
mail_t* mail_init(address_t* address, float weight, 
string_t* mailId, string_t* createdTime, string_t* deliveryTime) {
    mail_t* mail = (mail_t*)malloc(sizeof(mail_t));
    if (mail == NULL) {
        return NULL;
    }
    mail->address = *address;
    mail->weight = weight;
    mail->mailID = *mailId;
    mail->createdTime = *createdTime;
    mail->deliveryTime = *deliveryTime;
    return mail;
}

int mail_print(mail_t* m) {
    mail_t mail = *m;
    printf("------------------\n");
    printf("Town: %s\n", mail.address.town.str);
    printf("Street: %s\n", mail.address.street.str);
    printf("House: %d\n", mail.address.house);
    printf("Corpus: %s\n", mail.address.corpus.str);
    printf("ApartNumber: %d\n", mail.address.apartNumber);
    printf("Index: %s\n", mail.address.index.str);
    printf("Weight: %f\n", mail.weight);
    printf("MailID: %s\n", mail.mailID.str);
    printf("Create: %s\n", mail.createdTime.str);
    printf("Delivery: %s\n", mail.deliveryTime.str);
    printf("----------------------------------\n");
    return SUCCESS;
}

mail_t* mail_search(Post* p, char* id) {
    for (int i = 0; i < p->count; i++) {
        if (strcmp(p->mails[i].mailID.str, id) == 0) {
            return &p->mails[i];
        }
    }
    return NULL;
}

int mail_cmp(const void* a, const void* b) {
    mail_t* m1 = (mail_t*)a;
    mail_t* m2 = (mail_t*)b;
    if (string_cmp(&m1->address.index, &m2->address.index) != 0) {
        return string_cmp(&m1->address.index, &m2->address.index);
    }

    return string_cmp(&m1->mailID, &m2->mailID);
}

int mail_sort(mail_t* mails, int size) {
    qsort(mails, size, sizeof(mail_t), mail_cmp);
    return SUCCESS;
}


//Функции Address
address_t* adress_init(string_t* town, string_t* street, int house, 
string_t* corpus, int apartNumber, string_t* index) {
    address_t* a = (address_t*)malloc(sizeof(address_t));
    if (a == NULL) {
        return NULL;
    }
    a->town = *town;
    a->street = *street;
    a->house = house;
    a->corpus = *corpus;
    a->apartNumber = apartNumber;
    a->index = *index;
    return a;
}

//Функции Post
Post* post_init(address_t* address) {
    Post* p = (Post*)malloc(sizeof(Post));
    if (p == NULL) {
        return NULL;
    }
    p->addres= address;
    mail_t* mails = (mail_t*)malloc(10 * sizeof(mail_t));
    if (mails == NULL) {
        return NULL;
    }
    p->mails = mails;
    p->count = 0;
    p->capacity = 10;
    return p;
}

int mail_add(Post* p, mail_t* m) {
    if (p->count >= p->capacity) {
        p->capacity *= 2;
        p->mails = (mail_t*)realloc(p->mails, p->capacity * sizeof(mail_t));
        if (p->mails == NULL) {
            return ERROR;
        }
    }
    p->mails[p->count++] = *m;
    mail_sort(p->mails, p->count);
    return SUCCESS;
}

int mail_delete(Post* p, mail_t* m) {
    int index = -1;

    for (int i = 0; i < p->count; i++) {
        if (string_eq(&p->mails[i].mailID, &m->mailID)) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return ERROR;
    }

    for (int i = index; i < p->count - 1; i++) {
        p->mails[i] = p->mails[i + 1];
    }

    p->count--;
    mail_sort(p->mails, p->count);

    return SUCCESS;
}

int mail_deliver(mail_t* m) {
    m->deliveryTime = *str_init(get_current_time());
    return SUCCESS;
}

int post_print(Post* p) {
    for (int i = 0; i < p->count; i++) {
        mail_t mail = p->mails[i];
        printf("Town: %s\n", mail.address.town.str);
        printf("Street: %s\n", mail.address.street.str);
        printf("House: %d\n", mail.address.house);
        printf("Corpus: %s\n", mail.address.corpus.str);
        printf("ApartNumber: %d\n", mail.address.apartNumber);
        printf("Index: %s\n", mail.address.index.str);
        printf("Weight: %f\n", mail.weight);
        printf("MailID: %s\n", mail.mailID.str);
        printf("Create: %s\n", mail.createdTime.str);
        printf("Delivery: %s\n", mail.deliveryTime.str);
        printf("----------------------------------\n");
    }
    return SUCCESS;
}


int delivery_print(Post* p) {
    mail_t* m = (mail_t*)malloc(p->count * sizeof(mail_t));
    int n = 0;
    for (int i = 0; i < p->count; i++) {
        mail_t mail = p->mails[i];
        if (strlen(mail.deliveryTime.str) == 19) {
            m[n] = mail;
            n++;
        }
    }
    
    qsort(m, n, sizeof(mail_t), time_cmp);
    for (int i = 0; i < n; i++) {
        mail_t mail = m[i];
        printf("Town: %s\n", mail.address.town.str);
        printf("Street: %s\n", mail.address.street.str);
        printf("House: %d\n", mail.address.house);
        printf("Corpus: %s\n", mail.address.corpus.str);
        printf("ApartNumber: %d\n", mail.address.apartNumber);
        printf("Index: %s\n", mail.address.index.str);
        printf("Weight: %f\n", mail.weight);
        printf("MailID: %s\n", mail.mailID.str);
        printf("Create: %s\n", mail.createdTime.str);
        printf("Delivery: %s\n", mail.deliveryTime.str);
        printf("----------------------------------\n");
    }
    free(m);
    return SUCCESS;
}

int not_delivered_print(Post* p) {
    mail_t* m = (mail_t*)malloc(p->count * sizeof(mail_t));
    int n = 0;
    for (int i = 0; i < p->count; i++) {
        mail_t mail = p->mails[i];
        if (strlen(mail.deliveryTime.str) == 0) {
            m[n] = mail;
            n++;
        }
    }
    
    qsort(m, n, sizeof(mail_t), time_cmp);
    for (int i = 0; i < n; i++) {
        mail_t mail = m[i];
        printf("Town: %s\n", mail.address.town.str);
        printf("Street: %s\n", mail.address.street.str);
        printf("House: %d\n", mail.address.house);
        printf("Corpus: %s\n", mail.address.corpus.str);
        printf("ApartNumber: %d\n", mail.address.apartNumber);
        printf("Index: %s\n", mail.address.index.str);
        printf("Weight: %f\n", mail.weight);
        printf("MailID: %s\n", mail.mailID.str);
        printf("Create: %s\n", mail.createdTime.str);
        printf("Delivery: %s\n", mail.deliveryTime.str);
        printf("----------------------------------\n");
    }
    free(m);
    return SUCCESS;
}

int post_free(Post* p) {
    for (int i = 0; i < p->count; i++) {

        free(p->mails[i].address.town.str);
        free(p->mails[i].address.street.str);
        free(p->mails[i].address.corpus.str);
        free(p->mails[i].address.index.str);

        free(p->mails[i].mailID.str);
        free(p->mails[i].createdTime.str);
        free(p->mails[i].deliveryTime.str);
    }

    free(p->mails);

    free(p->addres->town.str);
    free(p->addres->street.str);
    free(p->addres->corpus.str);
    free(p->addres->index.str);

    free(p->addres);

    free(p);

    return SUCCESS;
}


