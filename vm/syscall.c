#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <lib/lava.h>

static int __sys_lava(enum lava_cmd cmd, union lava_attr __user *uattr, unsigned int size)
{
    return 0;
}

SYSCALL_DEFINE3(lava, int, cmd, union lava_attr __user *, uattr, unsigned int, size)
{
    return __sys_lava(cmd, USER_BPFPTR(uattr), size);
}
