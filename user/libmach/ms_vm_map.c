/* 
 * Mach Operating System
 * Copyright (c) 1993-1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * $Log:	ms_vm_map.c,v $
 * Revision 2.3  93/01/24  14:24:37  danner
 * 	Corrected include of mach/mach.h to mach.h.
 * 	[93/01/16            mrt]
 * 
 * Revision 2.2  92/01/03  20:36:45  dbg
 * 	Split into individual files so that user can substitute for any
 * 	one routine (e.g. vm_allocate).
 * 	[91/09/05            dbg]
 * 
 * Revision 2.8  91/06/25  10:34:48  rpd
 * 	Check against MACH_SEND_INTERRUPTED instead of KERN_SUCCESS.
 * 	[91/05/20            rpd]
 * 
 * Revision 2.5  90/09/09  14:34:31  rpd
 * 	Added mach_port_allocate_name since it didn't exist because of
 * 	the makefile sed.
 * 	[90/09/03            rwd]
 * 
 * Revision 2.4  90/06/19  23:03:50  rpd
 * 	Added mach_port_allocate, mach_port_deallocate, mach_port_insert_right.
 * 	[90/06/02            rpd]
 * 
 * Revision 2.3  90/06/02  15:12:36  rpd
 * 	Try kernel call whenever syscall fails.
 * 	[90/05/31            rpd]
 * 
 * 	Updated for new IPC.
 * 	Removed vm_allocate_with_pager.
 * 	[90/05/31            rpd]
 * 
 * Revision 2.2  90/05/29  18:40:03  rwd
 * 	New file from rfr to try traps then mig.
 * 	[90/04/20            rwd]
 * 
 */

#include <mach.h>
#include <mach/message.h>

kern_return_t vm_map
	(target_task, address, size, mask, anywhere, memory_object, offset, copy, cur_protection, max_protection, inheritance)
	vm_task_t target_task;
	vm_address_t *address;
	vm_size_t size;
	vm_address_t mask;
	boolean_t anywhere;
	memory_object_t memory_object;
	vm_offset_t offset;
	boolean_t copy;
	vm_prot_t cur_protection;
	vm_prot_t max_protection;
	vm_inherit_t inheritance;
{
	kern_return_t result;

	result = syscall_vm_map(target_task, address, size, mask, anywhere,
				memory_object, offset, copy,
				cur_protection, max_protection, inheritance);
	if (result == MACH_SEND_INTERRUPTED)
		result = mig_vm_map(target_task, address, size, mask, anywhere,
				memory_object, offset, copy,
				cur_protection, max_protection, inheritance);
	return(result);
}

