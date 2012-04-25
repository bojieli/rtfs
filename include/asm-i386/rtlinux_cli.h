#ifndef RTLINUX_CLI_H
#define RTLINUX_CLI_H

/* interrupt control.. */
#define rtl_hard_save_flags_kernel(x)		__asm__ __volatile__("pushfl ; popl %0":"=g" (x): /* no input */ :"memory")
#define rtl_hard_restore_flags_kernel(x) 	__asm__ __volatile__("pushl %0 ; popfl": /* no output */ :"g" (x):"memory")
#define rtl_hard_cli_kernel() 		__asm__ __volatile__("cli": : :"memory")
#define rtl_hard_sti_kernel()			__asm__ __volatile__("sti": : :"memory")

/* For spinlocks etc */
#define rtl_hard_local_irq_save_kernel(x)	__asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (x): /* no input */ :"memory")
#define rtl_hard_local_irq_restore_kernel(x)	__asm__ __volatile__("pushl %0 ; popfl": /* no output */ :"g" (x):"memory")
#define rtl_hard_local_irq_disable_kernel()	__asm__ __volatile__("cli": : :"memory")
#define rtl_hard_local_irq_enable_kernel()	__asm__ __volatile__("sti": : :"memory")
struct irq_control_s {
	void (*do_save_flags)(unsigned long *);
	void (*do_restore_flags)(unsigned long);
	void (*do_cli)(void);
	void (*do_sti)(void);
	void (*do_local_irq_save)(unsigned long *);
	void (*do_local_irq_restore)(unsigned long);
};
extern struct irq_control_s irq_control;
#undef __cli
#undef __sti
#undef __save_flags
#undef __restore_flags
#undef local_irq_save
#undef local_irq_restore
#undef local_irq_disable
#undef local_irq_enable
#define __save_flags(x)	irq_control.do_save_flags(&x)
#define __restore_flags(x) irq_control.do_restore_flags(x)
#define __cli()		irq_control.do_cli()
#define __sti()		irq_control.do_sti()
#define local_irq_save(x)	irq_control.do_local_irq_save((&x))
#define local_irq_restore(x)	irq_control.do_local_irq_restore(x)
#define local_irq_disable()	irq_control.do_cli()
#define local_irq_enable()	irq_control.do_sti()

#undef safe_halt
#define safe_halt()	do { __asm__ __volatile__("cli; nop; sti; hlt": : :"memory"); irq_control.do_sti();  } while (0)
#endif /* RTLINUX_CLI_H */
