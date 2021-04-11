/*
 * board_select.h
 *
 *  Created on: Apr 11, 2021
 *      Author: steph
 */

#ifndef BOARD_SELECT_H_
#define BOARD_SELECT_H_

typedef enum {
    MIDI_CONTROLLER,
    LOOPER,
}board_type_e;

void init_board_select(void);

board_type_e get_board_type(void);



#endif /* BOARD_SELECT_H_ */
