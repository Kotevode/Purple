//
// Created by Mark on 23.04.17.
//

#ifndef PURPLE_DISTRIBUTION_CONFIG_H
#define PURPLE_DISTRIBUTION_CONFIG_H

#include <math.h>

#define F_Y0(x) 2.0
#define F_YN(x) 0.0
#define F_X0(y) (2.0 - y)*(2.0 - y) / 2.0
#define F_XN(y) 2.0 - y
#define F(x,y)  (exp(-x) + exp(-y)) / 10.0

#define X0 0.0
#define XN 1.0
#define Y0 0.0
#define YN 1.0

#endif //PURPLE_DISTRIBUTION_CONFIG_H
