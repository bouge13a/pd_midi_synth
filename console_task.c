/*
 * console_uart.c
 *
 *  Created on: Aug 3, 2020
 *      Author: steph
 */

#include "console_task.h"
#include "text_controls.h"
#include "console_uart.h"

#include <assert.h>
#include <uartstdio.h>

// RX Queues
static QueueHandle_t uart_rx_q = NULL;

// Index tp keep track of how many items in console_pages
static uint32_t num_console_pages = 0;

// Index to keep track of current page
static uint32_t current_page = 0;
static uint32_t last_page = 0;

// Width of the Menu bar
static const uint32_t MENU_BAR_WIDTH = 70;

typedef struct  {
    const char* name;
    void (*pfnDrawPage)(void);
    void (*pfnDrawData)(void);
    void (*pfnDrawInput)(int);
    uint32_t refresh_rate;
    bool on_screen;
    bool write_to_page;
} console_page_t;

static void start_draw_menu(void);
static void draw_menu(void);

// Array of console_page
static console_page_t console_pages[] = {
    {NULL, NULL, NULL, NULL, 0},
    {NULL, NULL, NULL, NULL, 0},
    {NULL, NULL, NULL, NULL, 0},
    {NULL, NULL, NULL, NULL, 0},
    {NULL, NULL, NULL, NULL, 0},
    {NULL, NULL, NULL, NULL, 0},
    {NULL, NULL, NULL, NULL, 0},
};

void init_console(QueueHandle_t uart_rx_queue) {

    uart_rx_q = uart_rx_queue;

} // End init_console

uint32_t add_page(const char* name,
                  void (*pfnDrawPage)(void),
                  void (*pfnDrawData)(void),
                  void (*pfnDrawInput)(int),
                  uint32_t refresh_rate,
                  bool write_to_page) {

    uint32_t page_number;

    assert(name);
    assert(pfnDrawPage);
    assert(pfnDrawData);
    assert(pfnDrawInput);

    console_pages[num_console_pages].name          = name;
    console_pages[num_console_pages].pfnDrawPage   = pfnDrawPage;
    console_pages[num_console_pages].pfnDrawData   = pfnDrawData;
    console_pages[num_console_pages].pfnDrawInput  = pfnDrawInput;
    console_pages[num_console_pages].refresh_rate  = refresh_rate;
    console_pages[num_console_pages].write_to_page = write_to_page;
    console_pages[num_console_pages].on_screen     = false;

    page_number = num_console_pages;

    num_console_pages++;

    return page_number;

} // End add_page

bool is_on_screen(uint32_t page_number) {

    if (true == console_pages[page_number].on_screen) {
        return true;
    } else {
        return false;
    }
} // is_on_screen

static void start_draw_menu(void) {

    // loop index
    uint32_t idx;

    // Place the cursor in the middle of the menu bar
    // minus half the number of console pages
    cursor_pos(0, MENU_BAR_WIDTH/2);

    // Reverse the backround color and print an *
    set_text_mode(mode_reverse);
    UARTprintf("*");
    set_text_mode(mode_reset);

    //draw the rest of the asterisks
    for(idx=0; idx<num_console_pages-1; idx++){
        UARTprintf("*");
    }

    // Set the cursor position to the next line and print name
    cursor_pos(2, MENU_BAR_WIDTH/2);
    UARTprintf("%s", console_pages[0].name);

    // set the cursor position to next line and print menu bar
    cursor_pos(3, 0);
    for(idx=0; idx<MENU_BAR_WIDTH; idx++) {
        UARTprintf("-");
    }

} // End draw_menu

static void draw_menu(void) {

    cursor_pos(0, MENU_BAR_WIDTH/2 + last_page);
    set_text_mode(mode_reset);
    UARTprintf("*");

    cursor_pos(0, MENU_BAR_WIDTH/2 + current_page);

    // Reverse the backround color and print an *
    set_text_mode(mode_reverse);
    UARTprintf("*");
    set_text_mode(mode_reset);

    cursor_pos(2, 0);
    clear_line();
    cursor_pos(2, MENU_BAR_WIDTH/2);
    UARTprintf("%s", console_pages[current_page].name);

    cursor_pos(4,0);
    clear_below_line();

} // End draw_menu

static uint8_t rx_char = 0;

void console_task(void* parm) {

    clear_terminal();

    set_text_mode(mode_concealed);

    start_draw_menu();

    draw_menu();

    console_pages[0].pfnDrawPage();

    while(1) {

        rx_char = 0;

        xQueueReceive(uart_rx_q, &rx_char, console_pages[current_page].refresh_rate);

        if (9 == rx_char) {

            console_pages[last_page].on_screen = false;

            last_page = current_page;

            current_page = (current_page + 1) % num_console_pages;

            console_pages[current_page].on_screen = true;

            draw_menu();

            last_page = current_page;

            console_pages[current_page].pfnDrawPage();

            set_text_mode(mode_concealed);

        } else if (ESCAPE_KEY == rx_char) {

            if ('[' == get_char()) {
                // Control sequence detected
                // Perform the control sequence
                switch(get_char()) {
                case 'A':
                    console_pages[current_page].pfnDrawInput(UP_KEY);
                    break;

                case 'B':
                    console_pages[current_page].pfnDrawInput(DOWN_KEY);
                    break;

                case 'C':
                    console_pages[current_page].pfnDrawInput(RIGHT_KEY);
                    break;

                case 'D':
                    console_pages[current_page].pfnDrawInput(LEFT_KEY);
                    break;

                default:
                    break;

                }

            } else {
                    // Escape key was pressed
                    console_pages[current_page].pfnDrawInput(ESCAPE_KEY);

            }
        }

        if (console_pages[current_page].write_to_page) {
            cursor_pos(6, 0);
        } else {
            console_pages[current_page].pfnDrawData();
        }

        set_text_mode(mode_concealed);

    }

} // End console_task






