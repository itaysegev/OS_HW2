#include <linux/kernel.h>
#include <linux/sched.h>


asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage long sys_set_weight(int weight) {
	if (weight < 0)
		return -EINVAL;
		
	current->weight = weight;
	return 0;
}

asmlinkage long sys_get_weight(void) {
	return current->weight;
}

static long recursive_calculate(struct task_struct* curr, bool is_first) {
	
	int sum = 0;
	if((!is_first)&&(list_empty(&(curr->children)))) {
		sum = curr->weight;
	}
	struct list_head* node_iter;
	struct task_struct* task;
	list_for_each(node_iter, &(curr->children)) {
		task = list_entry(node_iter, struct task_struct, sibling);
		sum+=recursive_calculate(task, false);
	}
	return sum;
}

asmlinkage long sys_get_leaf_children_sum(void) {
	if(list_empty(&(current->children))) {
		return -ECHILD;
	}
	return recursive_calculate(current, true);
}

asmlinkage pid_t sys_get_heaviest_ancestor(void) {
	int max_weight = current->weight;
	pid_t max_pid = current->pid;
	struct task_struct	*ancestor = current->parent;
	while (ancestor != ancestor->parent)
	{
		if (ancestor->weight > max_weight)
		{
			max_weight = ancestor->weight;
			max_pid = ancestor->pid;
		}
		ancestor = ancestor->parent;
	}
	if (ancestor->weight > max_weight)
	{
		max_weight = ancestor->weight;
		max_pid = ancestor->pid;
	}
	return max_pid;
}

