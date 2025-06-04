#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("otusru");
MODULE_DESCRIPTION("Стек с использованием list_head");
MODULE_VERSION("0.1");

struct stack_node {
    int data;
    struct list_head list;
};

static LIST_HEAD(stack);

static void stack_push(int value) {
    struct stack_node *node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node)
        return;
    node->data = value;
    list_add(&node->list, &stack);
    pr_info("stack_list: pushed %d\n", value);
}

static void stack_pop(void) {
    struct stack_node *node;

    if (list_empty(&stack))
        return;

    node = list_first_entry(&stack, struct stack_node, list);
    list_del(&node->list);
    pr_info("stack_list: popped %d\n", node->data);
    kfree(node);
}

static void stack_clear(void) {
    struct stack_node *node, *tmp;
    list_for_each_entry_safe(node, tmp, &stack, list) {
        list_del(&node->list);
        pr_info("stack_list: clearing %d\n", node->data);
        kfree(node);
    }
}

static int __init stack_list_init(void) {
    pr_info("stack_list: init\n");
    stack_push(1);
    stack_push(2);
    stack_push(3);
    stack_pop();
    return 0;
}

static void __exit stack_list_exit(void) {
    pr_info("stack_list: exit\n");
    stack_clear();
}

module_init(stack_list_init);
module_exit(stack_list_exit);
