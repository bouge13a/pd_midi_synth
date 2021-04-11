

/**
 * main.c
 */


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdbool.h>

#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "board_select.h"
#include "midi_ctrl_board.h"
#include "looper_board.h"


int main(void){


    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    MAP_FPUEnable();
    MAP_FPULazyStackingEnable();

    // Set the system clock to 50 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 |
                       SYSCTL_USE_PLL |
                       SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    init_board_select();

    if (MIDI_CONTROLLER == get_board_type()) {
        init_midi_ctrl_board();
    } else {
        init_looper_board();
    }


    // Enable processor interrupts.
    IntMasterEnable();

    ///////////////////////////////////////////////////////
    //               Start Scheduler
    ///////////////////////////////////////////////////////
    vTaskStartScheduler();
}


