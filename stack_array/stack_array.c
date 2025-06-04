#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("otusru");
MODULE_DESCRIPTION("Стек на массиве");
MODULE_VERSION("0.1");

#define STACK_SIZE 5
static int stack[STACK_SIZE];
static int top = -1;

static void stack_push(int val) {
    if (top >= STACK_SIZE - 1) {
        pr_warn("stack_array: overflow\n");
        return;
    }
    stack[++top] = val;
    pr_info("stack_array: pushed %d\n", val);
}

static void stack_pop(void) {
    if (top < 0) {
        pr_warn("stack_array: underflow\n");
        return;
    }
    pr_info("stack_array: popped %d\n", stack[top--]);
}

static int __init stack_array_init(void) {
    pr_info("stack_array: init\n");
    stack_push(100);
    stack_push(200);
    stack_push(300);
    stack_pop();
    return 0;
}

static void __exit stack_array_exit(void) {
    pr_info("stack_array: exit, cleaning stack\n");
    while (top >= 0)
        stack_pop();
}

module_init(stack_array_init);
module_exit(stack_array_exit);
