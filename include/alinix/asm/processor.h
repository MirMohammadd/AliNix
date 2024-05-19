/**
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * @abstraction:
 * 	- Implements processor util in header file.
*/
#ifndef __ALINIX_KERNEL_PROCESSOR_H
#define __ALINIX_KERNEL_PROCESSOR_H

static inline void local_irq_disable(void)
{
	#if defined(__i386__) && defined(CONFIG_X86)
	asm volatile("msr daifset, #3" : : : "memory");
	#endif //  i386
}


#endif