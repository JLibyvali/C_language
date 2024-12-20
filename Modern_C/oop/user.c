#include "debug.h"
#include "dictionary.h"
#include "point.h"
#include "private.h"
#include "vtable.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int                     main()
{

    printf("#################### Basic OOP #################\n");
    dic *disc0 = new_dic();
    dic_add(disc0, "A", (void *)(0x01));
    dic_add(disc0, "B", (void *)(0x02));
    dic_add(disc0, "C", (void *)(0x03));

    void *value = dic_find(disc0, "A");
    printf("Look up result: %p \n", value);
    free_dic(disc0);

    printf("#################### Inheritance #################\n");
    ThreeD threed_0   = {.m_x = 10, .m_y = 11, .m_z = 22};
    double threeD_len = three_length(&threed_0);
    double twoD_len   = length(&threed_0.two_d);
    printf("Anonymous Len: %f, Other len: %f \n", threeD_len, twoD_len);

    printf("#################### Virtual Table Polymorphism #################\n");
    Text_base *song = new_textbase(
        "Love song", "I wrote this song, which a simple song. Don't complicated or confused...\n", print_song
    );
    Text_base *html = new_textbase("<HTML Title>", "<body> Good string make me happy. <body>\n", print_html);
    Text_base *some = new_textbase("Just some", "Some text relaxed", NULL);

    // init  hash table
    print_textbase(song);
    print_textbase(html);
    print_textbase(some);
    Text_base *cpy_song = copy_textbase(song);
    print_textbase(cpy_song);

    delete_textbase(song);
    delete_textbase(html);
    delete_textbase(some);
    delete_textbase(cpy_song);

    printf("#################### Member Private Scope #################\n");
    box *box1 = new_box(11);
    box *box2 = new_box(22);

    print_box(box1);
    print_box(box2);
    delete_box(box1);
    delete_box(box2);

    return 0;
}