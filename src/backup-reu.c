/*
  backup-reu.c -- Copy to REU, then write to DOS.
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

#include <accelerator.h>
#include "timer.h"

bool backup_reu(void) {
    unsigned long address = 0;
    unsigned long block_start = 0;
    unsigned int reu_block = 0;
    unsigned int nblocks = (ramlink_size + reu_size - 1)/reu_size;
    unsigned int iPrint = 0;
    
    printf("Copying RAMLink to REU:    0 of %4u\n", (unsigned int)(ramlink_size >> 16));
    printf("Saving REU:   0 of %3u", nblocks);
    printf("\nDur: 00:00:00:0");
        gotox(21);
        printf("ETC: ");
        timer_output();
    gotoxy(0, wherey() - 2);
    while(reu_block < nblocks){
        for (address = block_start; address < block_start + reu_size && address < ramlink_size; address += BUFFER_SIZE) {
            if ((iPrint==3)){
                iPrint=0;
                gotox(0);
                printf("Copying RAMLink to REU: %4u", (unsigned int)(address>>16));}
            else {iPrint++;}
        #if ENABLE_RAMLINK
            if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(0);}
            ramlink_reu_copy(address, buffer, BUFFER_SIZE, REU_COMMAND_REU_TO_C64);
            if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(1);}
        #endif
        #if ENABLE_REU
            if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(0);}
            reu_copy(address - block_start, buffer, BUFFER_SIZE, REU_COMMAND_C64_TO_REU);
            if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(1);}
        #endif
        }
        gotox(0);
        printf("\nSaving REU: %3u", reu_block + 1);
        printf("\nDur: ");
        timer_output();
        gotox(21);
        printf("ETC: ");
        timer_print_hrsminsec((timer_seconds() / (reu_block + 1)) * nblocks);
        gotoxy(0,wherey()-1);

        #if ENABLE_DOS
            if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(1);}
            if (ultimate_dos_save_reu(1, 0, reu_size) != NULL) {
                if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(0);}
                printf("Can't save REU: %s\n", ultimate_ci_status);
                return false;
            }
        #endif
        gotoxy(0, wherey() - 1);
        ++reu_block;
        block_start += reu_size;
    }
    if(cpu == CPU_C128 && cpu_speed != 1) {set_c128_speed(0);}
    printf("Copying RAMLink to REU: %4u", (unsigned int)(address>>16));
    gotoxy(0, wherey() + 2);
    return true;
}
