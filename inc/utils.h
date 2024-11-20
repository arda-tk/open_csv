//
// Created by qman on 20/11/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define LOG_ERROR(...)                sprintf(stderr, __VA_ARGS__);
#define SYS_MSG(...)                  sprintf(stdout, __VA_ARGS__);

#endif //UTILS_H
