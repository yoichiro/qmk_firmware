// Copyright 2022 Yoichiro Tanaka (@yoichiro)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <mcuconf.h>

#undef RP_SIO_USE_UART0
#define RP_SIO_USE_UART0 TRUE
#undef RP_SIO_USE_UART1
#define RP_SIO_USE_UART1 FALSE

#undef RP_PWM_USE_PWM4
#define RP_PWM_USE_PWM4 TRUE
