/*
 * logger.h
 *
 *  Created on: Aug 7, 2020
 *      Author: steph
 */

#ifndef LOGGER_H_
#define LOGGER_H_

typedef struct {
    const char* name;
    const char* info;
    uint32_t occurences;
}error_t;

error_t* create_error(const char* name,
                      const char* info);

void set_error(error_t* error);

void init_logger(void);

void logger_drawpage(void);
void logger_drawdata(void);
void logger_drawinput(int character);

#endif /* LOGGER_H_ */
