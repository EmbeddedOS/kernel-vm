#pragma once

enum lava_cmd
{

};

union lava_attr
{
};

/* Register numbers */
enum {
    LAVA_REG_0 = 0,
    LAVA_REG_1,
    LAVA_REG_2,
    LAVA_REG_3,
    LAVA_REG_4,
    LAVA_REG_5,
    LAVA_REG_6,
    LAVA_REG_7,
    LAVA_REG_8,
    LAVA_REG_9,
    LAVA_REG_10,
    __MAX_LAVA_REG,
};


struct lava_inst
{
    __u8 code;        /* opcode */
    __u8 dst_reg : 4; /* dest register */
    __u8 src_reg : 4; /* source register */
    __s16 off;        /* signed offset */
    __s32 imm;        /* signed immediate constant */
};