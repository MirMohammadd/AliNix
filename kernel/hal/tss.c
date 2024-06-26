/**
 * @author Ali Mirmohammad
 * @file tss.c
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
 *  - Kernel TSS implementations.
*/

#include <alinix/module.h>

MODULE_AUTHOR("Ali Mirmohammad")
MODULE_DESCRIPTION("Kernel TSS implementations")
MODULE_LICENSE("AGPL-3.0")

/**
 * @brief Flush the Task State Segment (TSS).
 *
 * This function flushes the Task State Segment (TSS) by loading the segment selector 0x2B into the task register (TR).
 * The TSS is a data structure used by the x86 processor to store information about a task's execution environment.
 *
 * @return void
 *
 * @throws None
 */
void flush_tss() {
    asm volatile("mov $0x2B, %ax; \
                  ltr %ax;");
}