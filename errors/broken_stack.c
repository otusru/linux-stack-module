#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("otusru");
MODULE_DESCRIPTION("Ошибочный стек с багами");
MODULE_VERSION("0.1");

struct stack_node {
    int data;
    struct list_head list;
};

static LIST_HEAD(broken_stack);

static void stack_push_broken(int val) {
    struct stack_node *node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node)
        return;
    node->data = val;
    list_add(&node->list, &broken_stack);
    pr_info("broken_stack: pushed %d (but never freed)\n", val);
}

static void stack_pop_broken(void) {
    struct stack_node *node;
    if (list_empty(&broken_stack))
        return;

    node = list_first_entry(&broken_stack, struct stack_node, list);
    pr_info("broken_stack: popped %d (but list not updated)\n", node->data);
    kfree(node);
}

static int __init broken_stack_init(void) {
    pr_info("broken_stack: init\n");
    stack_push_broken(111);
    stack_push_broken(222);
    stack_pop_broken();
    return 0;
}

static void __exit broken_stack_exit(void) {
    pr_info("broken_stack: exit (memory leak and broken list remains)\n");
}

module_init(broken_stack_init);
module_exit(broken_stack_exit);
