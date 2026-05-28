/*
  backup-dos.c -- Write directly to Ultiamte DOS.
  Copyright (C) 2020 Dieter Baron

  This file is part of Tachyon RL, a backup program for your RAMLink.
  The authors can be contacted at <tachyon-rl@tpau.group>.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. The names of the authors may not be used to endorse or promote
     products derived from this software without specific prior
     written permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "tachyon.h"

#include <conio.h>
#include <stdio.h>

#include <c64.h>
#include <accelerator.h>

bool backup_dos(void) {
    static unsigned long address;
    unsigned int iPrint = 0;
    if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(1);}
    printf("Saving RAMLink to disk:    0 of %4u", (unsigned int)(ramlink_size >> 16));
    for (address = 0; address < ramlink_size; address += BUFFER_SIZE) {
        if ((iPrint==3)){
            iPrint=0;
            gotox(0);
            printf("Saving RAMLink to disk: %4u", (unsigned int)(address>>16));}
        else {iPrint++;}
            #if ENABLE_RAMLINK
                if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(0);}
                ramlink_reu_copy(address, buffer, BUFFER_SIZE, REU_COMMAND_REU_TO_C64);
                if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(1);}
            #endif
            #if ENABLE_DOS
                if (ultimate_dos_write_data(1, buffer, BUFFER_SIZE) != 0) {
                    if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(0);}
                    printf("Write error: %s\n", ultimate_ci_status);
                    return false;
                }
            #endif
    }
    if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(0);}
    gotox(0);
    printf("Saving RAMLink to disk: %4u\n", (unsigned int)(address>>16));
        
    return true;
}
