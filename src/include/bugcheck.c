/*
                                                                                
                                             ,,                                 
`7MM"""Yp,                                 `7MM                       `7MM      
  MM    Yb                                   MM                         MM      
  MM    dP `7MM  `7MM  .P"Ybmmm      ,p6"bo  MMpMMMb.  .gP"Ya   ,p6"bo  MM  ,MP'
  MM"""bg.   MM    MM :MI  I8       6M'  OO  MM    MM ,M'   Yb 6M'  OO  MM ;Y   
  MM    `Y   MM    MM  WmmmP"       8M       MM    MM 8M"""""" 8M       MM;Mm   
  MM    ,9   MM    MM 8M            YM.    , MM    MM YM.    , YM.    , MM `Mb. 
.JMMmmmd9    `Mbod"YML.YMMMMMb       YMbmd'.JMML  JMML.`Mbmmd'  YMbmd'.JMML. YA.
                      6'     dP                                                 
                      Ybmmmd'                                                   

COPYRIGHT C JAD 2026 MAY 2

All rights reserved.
This file is part of Razzle.
Razzle is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

BUGCHECK HANDLER - ROBUST AND DETAILED AND USER FRIENDLY BLUE SCREEN OF DEATH HANDLER FOR RAZZLE
*/

#include "bugcheck.h"
#include "../drivers/VGAf/vgaf.h"
#include "../drivers/KEYdriver/key.h"
/*
Copyright (c) 2026 Jad. All rights reserved.
*/

void check_if_bugcheck_function_exists(void){
    printv("Bugcheck handler driver exists.\n");
    printv("copyright (c) jad 2026 Driver Tester (DriTest)\n");
}


void tripletest_and_test_idt(void){
    printv("Testing the Interrupt Descriptor Table...\n");
    __asm__ __volatile__("int $0x03"); // trigger a breakpoint exception to test the idt shit
    // hope it dont delete my files or smt *_*
}

void bugcheck(void* error_message) {
    clear();
    colorscreen(0x1F); 
    colortext(0x1F);
    __asm__ __volatile__("cli");

    printv_color("Razzle\n", 0x1F);
    printv("Your System has encountered a critical error and needs to restart.\nThis is a critical error and the system showed this bugcheck to\nstop damages on the hardware/software.\n");
    printv("\nError code: ");
    printv((const char*)error_message);
}