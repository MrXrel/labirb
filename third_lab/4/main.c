#include "functions.h"

int main() {



    //Почта России
    Post* post = post_init(adress_init(str_init("Москва"), str_init("Дубосековская"), 20, str_init("5"), 7204, str_init("734123")));
    address_t* a = adress_init(str_init("Москва"), str_init("Ленина"), 12, str_init("3"), 724, str_init("000000"));
    mail_add(post, mail_init(a, 32.4, str_init("00000000000000"), str_init("10:11:2006 10:22:32"), str_init("01:11:2005 10:02:32")));
    a = adress_init(str_init("Москва"), str_init("Пушкина"), 3, str_init("11"), 24, str_init("000000"));
    mail_add(post, mail_init(a, 3.2, str_init("00000000000000"), str_init("12:10:2005 09:22:32"), str_init("01:11:2006 10:02:32")));

    
    char addMail[15] = "add";
    char printPost[15] = "print";
    char stop[15] = "stop";
    char search[15] = "search";
    char delete[15] = "delete";
    char delivery[15] = "delivery";
    char printDelivery[15] = "printDelivery";
    char printNotdelivery[20] = "printNotDelivery";
    printf("----------------------------------\n");
    printf("add - добавить новую посылку\n");
    printf("print - вывести список всех посылок\n");
    printf("stop - завершить программу\n");
    printf("search - поиск посылки по Id\n");
    printf("delete - удаление посылки по Id\n");
    printf("delivery - доставка посылки по Id\n");
    printf("printDelivery - cписок всех доставленных поссылок\n");
    printf("printNotDelivery - cписок всех не доставленных поссылок\n");
    printf("----------------------------------\n");

    while (1) {
        
        char command[20];
        scanf("%s", command);

        if (strcmp(command, addMail) == 0) {
            printf("Введите данные: <Город> <Улица> <Дом> <Корпус> <Номер квартиры> <Индекс> <Вес посылки> <Почтовый идентификатор>\n");
            char town[50], street[50], corpus[10], index[10], mailid[20];
            int house, apart;
            float weight;

            scanf("%s %s %d %s %d %s %f %s", town, street, &house, corpus, &apart, index, &weight, mailid);
            if ((validate_idx(index) == ERROR) || (validate_idx(mailid) == ERROR)) {
                printf("Ошибка в индексе или почтовом индетификаторе\n");
                return ERROR;
            }
            address_t* a = adress_init(str_init(town), str_init(street), house, str_init(corpus), apart, str_init(index));
            int res = mail_add(post, mail_init(a, weight, str_init(mailid), str_init(get_current_time()), str_init("")));
            if (res == ERROR) {
                printf("Ошибка при добавлении\n");
            } else {
                printf("Посылка добавлена\n");
            }
        } else if (strcmp(command, printPost) == 0) {
            post_print(post);
        } else if (strcmp(command, stop) == 0) {
            post_free(post);
            break;
        } else if (strcmp(command, printDelivery) == 0) {
            delivery_print(post);
        } else if (strcmp(command, printNotdelivery) == 0) {
            not_delivered_print(post);
        }else if (strcmp(command, search) == 0) {
            printf("Введите ID посылки\n");
            char id[14];
            scanf("%s", id);
            mail_t* m = mail_search(post, id);
            if (m == NULL) {
                printf("Такой посылки не существует\n");
                return ERROR;
            } else {
                mail_print(m);
            }
        } else if (strcmp(command, delete) == 0) {
            printf("Введите ID посылки\n");
            char id[14];
            scanf("%s", id);
            mail_t* m = mail_search(post, id);
            if (m == NULL) {
                printf("Такой посылки не существует\n");
                return ERROR;
            } else {
                int res = mail_delete(post, m);
                if (res == ERROR) {
                    printf("Посылка не существует или неправильный id\n");
                    return ERROR;
                } else {
                    printf("Посылка удалена\n");
                }
            }

        } else if (strcmp(command, delivery) == 0) {
            printf("Введите ID посылки\n");
            char id[14];
            scanf("%s", id);
            mail_t* m = mail_search(post, id);
            int res = mail_deliver(m);
            if (res == ERROR) {
                printf("Посылка не существует или неправильный id\n");
            } else {
                printf("Посылка доставлена\n");
            }
        } else {
            printf("Неизвестная команда. Пожалуйста, попробуйте снова.\n");
        }
    }
    post_free(post);
}