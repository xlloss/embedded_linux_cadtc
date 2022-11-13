#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/moduleparam.h> 
/*
 * insmod hellop.ko count=5 text="slash-test" text_array=4,3,2,1
 */

static int count = 1; 
static char *text = "Terence";
static int text_array[] = {1,2,3,4};
static int textarray_size = 4;

module_param(count, int, S_IRUGO); 
module_param(text, charp, S_IRUGO); 
module_param_array(text_array, int, &textarray_size, S_IRUGO); 

static int hellop_init(void) 
{
	printk("Hello world!!\n"); 
	int i;

	for(i = 0; i < count ; i++) 
		printk("count(%d) text(%s)\n", i, text);

	for(i = 0; i < textarray_size; i++)
		printk("text_array[%d] : %d \n", i, text_array[i]);

	return 0;
}

static void hellop_exit(void)
{
	printk("Goodbye, Hello world!!\n");
} 

module_init(hellop_init);
module_exit(hellop_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is hello parameter module");
MODULE_VERSION("V1");
