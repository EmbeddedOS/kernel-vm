#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/sched.h>

#define __pr_info(fmt, ...) printk(KERN_INFO "[%s:%d] %s(): " fmt "\n", \
    __FILE__, __LINE__, __func__ , ##__VA_ARGS__)

    int my_proc_pid_readdir(struct file *file, struct dir_context *ctx)
    {
        __pr_info("my_proc_pid_readdir");
        return 0;
    }

    static void __naked hook_trampoline(void)
    {
        asm volatile (
            /* Create a minimal frame (push x29, x30) */
            "stp    x29, x30, [sp, #-16]!\n"
            "mov    x29, sp\n"
    
            /* Call the C helper. x0..x7 at this point contain the original regs. */
            "bl     my_proc_pid_readdir\n"
    
            /* Restore frame */
            "mov    sp, x29\n"          /* unwind to frame pointer */
            "ldp    x29, x30, [sp], #16\n"
    
            /* Branch to continuation address stored in x19 */
            "br     x19\n"
            ::: "memory"
        );
    }

static int proc_pid_readdir_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
    __pr_info("proc_pid_readdir_pre_handler");
    regs->regs[19] = (unsigned long)p->addr + 4;
    regs->pc = (unsigned long)hook_trampoline;
    return 1;
}

static int proc_pid_lookup_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
    struct dentry *dentry = (struct dentry *)regs->regs[0];
    __pr_info("Dentry name: %s", dentry->d_name.name);
    if (dentry->d_parent) {
        __pr_info("Parent: %s", dentry->d_parent->d_name.name);
    }

    return 0;
}

static struct {
    struct kprobe kp;
} kprobe_handlers[] = {
//    { .kp = { .symbol_name = "proc_pid_lookup", .pre_handler = proc_pid_lookup_pre_handler }, },
    { .kp = { .symbol_name = "proc_pid_readdir", .pre_handler = proc_pid_readdir_pre_handler }, },
};

static void unregister_kprobe_handlers(void)
{
    for (int i = 0; i < ARRAY_SIZE(kprobe_handlers); i++) {
         unregister_kprobe(&kprobe_handlers[i].kp);
    }
}

static int __init vm_init(void)
{
    int ret;

    for (int i = 0; i < ARRAY_SIZE(kprobe_handlers); i++) {
        ret = register_kprobe(&kprobe_handlers[i].kp);
        if (ret < 0) {
            __pr_info("Failed to register kprobe %s: %d", kprobe_handlers[i].kp.symbol_name, ret);
            unregister_kprobe_handlers();
            return ret;
        }
    }

    return 0;
}

static void __exit vm_exit(void)
{
    unregister_kprobe_handlers();
}

module_init(vm_init);
module_exit(vm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeddedOS");
MODULE_DESCRIPTION("Virtual Machine Module");
