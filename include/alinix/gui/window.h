#ifndef __ALINIX_KERNEL_WINDOW_H
#define __ALINIX_KERNEL_WINDOW_H

#include <alinix/types.h>

#define WINDOW_EDGE_COLOR               0xA3A29E
#define WINDOW_BACKGROUND_COLOR         0xE8E6E3
#define TEXT_AREA_COLOR                 0xFFFFF6


#define TYPE_TEXT_AREA                  1

#define TITLE_SIZE 256

typedef struct components{
    uint32_t *component;
    struct components *next;
}components_list_t;


typedef struct window{
    char title[TITLE_SIZE];
    int x;
    int y;
    int h;
    int w;
    components_list_t *components;
}window_t;


typedef struct wlist {
    window_t *window;
    struct wlist *next;
} window_list_t;



typedef struct text_area {
    uint32_t type;
    int x;
    int y;
    int h;
    int w;
    int cursorx;
    int cursory;
    char content[1024];
} text_area_t;




void windows_list_init();
window_t *window_create(char *title, int x, int y, int w, int h);
void add_component(window_t *window, void *component);
void paint_windows();
void paint_window(window_t *window);
text_area_t *create_text_area(int x, int y, int w, int h);
void text_area_set_text(text_area_t *text_area, char *text);
void text_area_append(text_area_t *text_area, char *text);
void draw_text_area(window_t *window, text_area_t *text_area);





#endif // __ALINIX_KERNEL_WINDOW_H