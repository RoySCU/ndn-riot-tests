/*
 * Copyright (C) 2016 Wentao Shang
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Minimum NDN consumer
 *
 * @author      Wentao Shang <wentaoshang@gmaiil.com>
 *
 * @}
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "shell.h"
#include "msg.h"


extern int ndn_controller(int argc, char **argv);

static const shell_command_t shell_commands[] = {
    { "control-access", "start access controller", ndn_controller },
    { NULL, NULL, NULL }
};

int main(void)
{
    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
