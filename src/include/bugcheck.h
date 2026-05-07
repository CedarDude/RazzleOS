

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

#ifndef BUGCHECK_H
#define BUGCHECK_H

// test bugcheck.c
void check_if_bugcheck_function_exists(void);
void tripletest_and_test_idt(void);
void bugcheck(void* error_message);
#endif