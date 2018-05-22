#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>	//init_task定义位置
#include <linux/sched.h> //1270行task结构体
#include <linux/unistd.h>
#include <linux/list.h>		 //系统自带双向链表
#include <linux/slab.h>		 //内核内存分配
#include <linux/uaccess.h> //用户访问内核
MODULE_LICENSE("Dual BSD/GPL");
#define __NR_ptreecall 287
#define MAXTASKNUM 2048

struct prinfo
{
	pid_t parent_pid;				// process id of parent
	pid_t pid;							// process id
	pid_t first_child_pid;	// pid of youngest child
	pid_t next_sibling_pid; // pid of older sibling
	long state;							// current state of process
	long uid;								// user id of process owner
	char comm[64];					// name of program executed
	int depth;							// 进程在树中的深度
};
//task2prinfo函数用于转化系统中的task_struct结构数据到我们定义的prinfo数据
void task2prinfo(struct task_struct *task, struct prinfo *buf, int *depth)
{
	buf->parent_pid = (task->parent) ? task->parent->pid : 0;
	buf->pid = task->pid;
	//如果task_struct结构体中的子进程链表为空，意味着我们的结构中的pid要置零，否则调用list_entry()取到子进程读取其pid
	buf->first_child_pid = list_empty(&(task->children)) ? 0 : list_entry(task->children.next, struct task_struct, sibling)->pid;
	buf->next_sibling_pid = (list_empty(&(task->sibling))) ? 0 : list_entry(task->sibling.next, struct task_struct, sibling)->pid;
	buf->state = task->state;
	buf->uid = task->cred->uid;
	get_task_comm(buf->comm, task); //调用系统内实现的提取进程名字的函数
	buf->depth = *depth;
}
//dfs函数用于返回所有进程的深度优先搜索的顺序
void dfs(struct task_struct *task, struct prinfo *buf, int *nr, int *depth)
{
	struct list_head *pos;
	struct task_struct *real_child;
	task2prinfo(task, &buf[*nr], depth); //转换存储形式，提取出数据
	++(*nr);
	++(*depth); //向下访问访问一层，深度加一
	list_for_each(pos, &(task->children))
	{
		real_child = list_entry(pos, struct task_struct, sibling);
		dfs(real_child, buf, nr, depth);
		--(*depth); //访问过儿子回退深度减一
	}
}

static int (*oldcall)(void);

int ptree(struct prinfo *buf, int *nr)
{
	struct prinfo *kernelBuf = (struct prinfo *)kmalloc(sizeof(struct prinfo) * MAXTASKNUM, GFP_KERNEL);
	int kernelNr = 0, depth = 0;
	read_lock(&tasklist_lock);
	dfs(&init_task, kernelBuf, &kernelNr, &depth); //从内核取出数据
	read_unlock(&tasklist_lock);
	//转换到用户层面，返回值大于0说明复制失败，返回的是剩余未被复制的的数量
	if (copy_to_user(buf, kernelBuf, sizeof(struct prinfo) * MAXTASKNUM) != 0)
	{
		printk(KERN_ERR "Copy kernel buf error!\n");
		return 1;
	}
	if (copy_to_user(nr, &kernelNr, sizeof(int)) != 0)
	{
		printk(KERN_ERR "Copy kernel nr error!\n");
		return 1;
	}
	kfree(kernelBuf);
	return 0;
}

static int ptree_init(void)
{
	long *syscall = (long *)0xc000d8c4;
	oldcall = (int (*)(void))(syscall[__NR_ptreecall]); // preserve the former system call
	syscall[__NR_ptreecall] = (unsigned long)ptree;			// assign the new system call
	printk(KERN_INFO "Module has been loaded!\n");
	return 0;
}

static void ptree_exit(void)
{
	long *syscall = (long *)0xc000d8c4;
	syscall[__NR_ptreecall] = (unsigned long)oldcall; // restore the old system call
	printk(KERN_INFO "Module has been removed!\n");
}

module_init(ptree_init);
module_exit(ptree_exit);
