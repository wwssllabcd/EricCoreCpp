#pragma once

// DIV 
#define E_BIT_OP_2                (1)
#define E_BIT_OP_4                (2)
#define E_BIT_OP_8                (3)
#define E_BIT_OP_16               (4)
#define E_BIT_OP_32               (5)
#define E_BIT_OP_64               (6)
#define E_BIT_OP_128              (7)
#define E_BIT_OP_256              (8)
#define E_BIT_OP_512              (9)
#define E_BIT_OP_1024             (10)
#define E_BIT_OP_2048             (11)
#define E_BIT_OP_4096             (12)
#define E_BIT_OP_8192             (13)
#define E_BIT_OP_16384            (14)
#define E_BIT_OP_32768            (15)


#define SHIFT_LEFT           <<
#define SHIFT_RIGHT          >>

#define SHIFT_DIV(VAL, NUMBER)  (VAL SHIFT_RIGHT E_BIT_OP_##NUMBER)
#define SHIFT_MUL(VAL, NUMBER)  (VAL SHIFT_LEFT E_BIT_OP_##NUMBER)


