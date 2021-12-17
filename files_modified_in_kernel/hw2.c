#include <linux/kernel.h>
#include <linux/sched.h>


asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage int sys_set_weight(int weight) {
	if (weight < 0)
		return -EINVAL;
		
	current->weight = weight;
	return 0;
}

asmlinkage int sys_get_weight(void) {
	return current->weight;
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

