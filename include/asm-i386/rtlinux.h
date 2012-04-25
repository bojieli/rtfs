
/* RTLinux support for patching the running kernel.
	* (c) Victor Yodaiken 1999
*/

#define label_ack_APIC() xlabel_code(0x12344321)
#define xlabel_code(x) asm __volatile__(".section rtlinux_patch,\"a\"\n .align 4\n"\
                             ".long 1f,"#x"\n" ".previous\n 1:\n" );
#define RTLINUX_LABEL(x)\
  ".section rtlinux_patch,\"a\"\n .align 4\n .long 1f,"#x"\n .previous\n 1:\n"

#define RTLINUX_EXPORT(x)\
  asm (".section rtlinux_funcs,\"a\"\n .align 4\n"\
                             ".long "#x"\n .previous\n" );
