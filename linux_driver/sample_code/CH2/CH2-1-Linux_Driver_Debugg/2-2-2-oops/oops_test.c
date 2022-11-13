/* 
 * Copy from Terence
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

static int oops_init(void)
{
    char *reg = (char *)0x0;
    printk("oops driver installed.\n");

    printk("%x\n", *reg);
    return 0;
}

void oops_exit(void)
{
    printk("oops driver removed.\n");
}

module_init(oops_init);
module_exit(oops_exit);

MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is oops module");
MODULE_VERSION("V1");
