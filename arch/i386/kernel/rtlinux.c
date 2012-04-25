#include <linux/config.h>
#include <linux/ptrace.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/timex.h>
#include <linux/malloc.h>
#include <linux/random.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/bitops.h>
#include <asm/pgtable.h>
#include <asm/delay.h>
#include <asm/desc.h>

#include <linux/irq.h>

static void rtl_hard_save_flags_f(unsigned long  *z){
	unsigned long y;
	rtl_hard_save_flags_kernel(y);
	*z=y;
}
static void rtl_hard_restore_flags_f(unsigned long  x) { rtl_hard_restore_flags_kernel(x); }
static void rtl_hard_cli_f(void) { rtl_hard_cli_kernel(); }
static void rtl_hard_sti_f(void) { rtl_hard_sti_kernel(); }
static void rtl_hard_local_irq_save_f(unsigned long  *x)
{ unsigned long y;rtl_hard_local_irq_save_kernel(y);*x=y; }
static void rtl_hard_local_irq_restore_f(unsigned long  x){
	rtl_hard_local_irq_restore_kernel(x);
}
struct irq_control_s irq_control = {
 	rtl_hard_save_flags_f,
 	rtl_hard_restore_flags_f,
 	rtl_hard_cli_f,
 	rtl_hard_sti_f,
 	rtl_hard_local_irq_save_f,
 	rtl_hard_local_irq_restore_f
 };
void *rtl_emulate_iret = 0;
void *rtl_exception_intercept = 0;
void *rtl_syscall_intercept = 0;

extern void *__start_rtlinux_patch,*__stop_rtlinux_patch;
extern asmlinkage void do_IRQ(struct pt_regs );
extern void ret_from_intr(void);
RTLINUX_EXPORT(__start_rtlinux_patch);
RTLINUX_EXPORT(__stop_rtlinux_patch);
RTLINUX_EXPORT(rtl_emulate_iret);
RTLINUX_EXPORT(irq_desc);
RTLINUX_EXPORT(do_IRQ);
RTLINUX_EXPORT(ret_from_intr);
RTLINUX_EXPORT(rtl_exception_intercept);

#ifdef CONFIG_X86_LOCAL_APIC
extern asmlinkage void smp_spurious_interrupt(void);
extern asmlinkage void smp_error_interrupt(void);
extern void smp_apic_timer_interrupt(struct pt_regs *);

RTLINUX_EXPORT(smp_spurious_interrupt);
RTLINUX_EXPORT(smp_error_interrupt);
RTLINUX_EXPORT(smp_apic_timer_interrupt);
#endif

#ifdef CONFIG_SMP
extern void rtl_reschedule(int cpu);
extern asmlinkage void smp_reschedule_interrupt(void);
extern asmlinkage void smp_invalidate_interrupt(void);
extern asmlinkage void smp_call_function_interrupt(void);
RTLINUX_EXPORT(smp_reschedule_interrupt);
RTLINUX_EXPORT(smp_invalidate_interrupt);
RTLINUX_EXPORT(smp_call_function_interrupt);
RTLINUX_EXPORT(rtl_reschedule);
#endif
