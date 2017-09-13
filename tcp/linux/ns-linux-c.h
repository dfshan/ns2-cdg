/* 
 * TCP-Linux module for NS2 
 *
 * May 2006
 *
 * Author: Xiaoliang (David) Wei  (DavidWei@acm.org)
 *
 * NetLab, the California Institute of Technology 
 * http://netlab.caltech.edu
 *
 * Module: linux/ns-linux-c.h
 *      This is the header file of shortcuts for Linux source codes (in C)
 *	We shortcut most of the Linux system calls which are not related to congestion control.
 *
 * See a mini-tutorial about TCP-Linux at: http://netlab.caltech.edu/projects/ns2tcplinux/
 *
 */

#ifndef NS_LINUX_C_H
#define NS_LINUX_C_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "ns-linux-util.h"
//For sharing Reno

extern u32 tcp_reno_ssthresh(struct sock *sk);
extern void tcp_reno_cong_avoid(struct sock *sk, u32 ack, u32 rtt, u32 in_flight, int flag);
extern u32 tcp_reno_min_cwnd(const struct sock *sk);

#define tcp_is_cwnd_limited(sk, in_flight)  ((in_flight) >= (sk)->snd_cwnd)

#define tcp_in_slow_start(tp) ((tp)->snd_cwnd < (tp)->snd_ssthresh)
/*
 * The next routines deal with comparing 32 bit unsigned ints
 * and worry about wraparound (automatic with unsigned arithmetic).
 */


//from kernel.h

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
#define max3(x, y, z) max((typeof(x))max(x, y), z)
#define before(seq1,seq2) ((seq2)>(seq1))
#define after(seq1,seq2) ((seq2)<=(seq1))

#define DIV_ROUND_CLOSEST(x, divisor)(			\
{							\
	typeof(x) __x = x;				\
	typeof(divisor) __d = divisor;			\
	(((typeof(x))-1) > 0 ||				\
	 ((typeof(divisor))-1) > 0 ||			\
	 (((__x) > 0) == ((__d) > 0))) ?		\
		(((__x) + ((__d) / 2)) / (__d)) :	\
		(((__x) - ((__d) / 2)) / (__d));	\
}							\
)

#define HZ 1000

// Shortcuts
#define THIS_MODULE 0
#define EXPORT_SYMBOL_GPL(symbol)
#define __init
#define __exit

typedef int (*registerFunctionPointer)(void);

#ifdef NS_PROTOCOL
// for new source codes that defines NS_PROTOCOL, we support module parameters
// set the file name of last_added file
extern void set_linux_file_name(const char*);
extern void record_linux_param(const char*, const char*, const char*, void*);
extern void record_linux_param_description(const char*, const char*, const char*);
#define module_init(x) \
	static void module_register(void) __attribute__((constructor));\
	static void module_register(void) { \
		x(); \
		set_linux_file_name(NS_PROTOCOL);\
	};

#define module_exit(x) \
	static void module_unregister_fake(void) __attribute__((constructor));\
	static void module_unregister_fake(void) {\
		if (0) x();\
	};

#define module_param(name, type, mode) \
	static void module_param_##name(void) __attribute__((constructor));\
	static void module_param_##name(void) { record_linux_param(NS_PROTOCOL, #name, #type, &name); };

#define MODULE_PARM_DESC(name, exp) \
	static void module_param_desc_##name(void) __attribute__((constructor));\
	static void module_param_desc_##name(void) { record_linux_param_description(NS_PROTOCOL, #name, exp); };

#else
// for old source codes that do not define NS_PROTOCOL, we do not support module parameters
#define module_init(x) \
	static void module_register(void) __attribute__((constructor));\
	static void module_register(void) { \
		x(); \
	 };
#define module_param(a,b,c)
#define MODULE_PARM_DESC(a,b)
#endif


#define module_put(x)
#define try_module_get(x) 0
#define MODULE_AUTHOR(x)
#define MODULE_LICENSE(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_VERSION(a)

#define KERN_ERR "<2> "
#define KERN_NOTICE "<1> "
#define KERN_INFO "<0> "
#define printk(fmt, args...) { \
	if (strlen(fmt)<4 || fmt[0]!='<' || fmt[2]!='>' || fmt[3]!=' ') \
		fprintf(stderr, "<Unspecified>" fmt, args);\
	else if (fmt[1]>=debug_level+'0')\
		printf(fmt, args);\
}

#define EINVAL 1
#define EEXIST 2
#define ENOENT 3
#define EBUSY 4
#define ENOMEM 5
#define EPERM 6
#define	ERANGE 34
#define BUG_ON(x) 
#define BUILD_BUG_ON(x)
#define WARN_ON(x)
//please make sure the system can run

#define spin_lock(x)
#define spin_unlock(x)
#define rcu_read_lock()
#define rcu_read_unlock()

#define late_initcall(x)

#define capable(x) 1
#define between(seq1, seq2, seq3) ((seq3) - (seq2) >= (seq1) - (seq2))

#define kstrdup(str, mode) strdup(str)

////////////For delay based protocols:
struct tcpvegas_info {
	__u32	tcpv_enabled;
	__u32	tcpv_rttcnt;
	__u32	tcpv_rtt;
	__u32	tcpv_minrtt;
};
#define INET_DIAG_VEGASINFO 1
struct rtattr {};
#define __RTA_PUT(skb, INFO_FLAG, size) NULL
#define RTA_DATA(rta) NULL

//#define DEFINE_SPINLOCK(x)
//#define LIST_HEAD(name)
#define DEFINE_SPINLOCK(x) int FAKED_LOCK_X
#define LIST_HEAD(name) int FAKED_LIST_HEAD
//The true list head is declared in ns-linux-util.h since it has to be accessible by TCP-Linux's cpp codes.


////////////For bit operations  From include/linux/bitops.h /////////
/*
 * fls: find last bit set.
 */

extern int fls(int x);
extern int fls64(__u64 x);

///// For 64 bit division from include/asm-generic/div64.h ////
#define do_div(n,base) ({                                      \
        uint32_t __base = (base);                               \
        uint32_t __rem;                                         \
        __rem = ((uint64_t)(n)) % __base;                       \
        (n) = ((uint64_t)(n)) / __base;                         \
        __rem;                                                  \
 })


#if BITS_PER_LONG == 64
/**
 * div_u64_rem - unsigned 64bit divide with 32bit divisor with remainder
 *
 * This is commonly provided by 32bit archs to provide an optimized 64bit
 * divide.
 */
static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

#elif BITS_PER_LONG == 32
#ifndef div_u64_rem
static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = do_div(dividend, divisor);
	return dividend;
}
#endif
#else
static inline u64 div_u64_rem(u64 dividend, u32 divisor, u32 *remainder)
{
	*remainder = do_div(dividend, divisor);
	return dividend;
}
#endif


/**
 * div_u64 - unsigned 64bit divide with 32bit divisor
 *
 * This is the most common 64bit divide and should be used if possible,
 * as many 32bit archs can optimize this variant better than a full 64bit
 * divide.
 */
#ifndef div_u64
static inline u64 div_u64(u64 dividend, u32 divisor)
{
	u32 remainder;
	return div_u64_rem(dividend, divisor, &remainder);
}
#endif


#define CONG_NUM 0

#define jiffies tcp_time_stamp

#define pr_debug(args...) {if ((debug_level+'0') <= KERN_NOTICE[1]) printf(args);}

#define __read_mostly 

/* 64bit divisor, dividend and result. dynamic precision */
extern uint64_t div64_64(uint64_t dividend, uint64_t divisor);

#define nla_put(skb, type, len, data)

extern int tcp_register_congestion_control(struct tcp_congestion_ops *ca);
extern void tcp_slow_start(struct tcp_sock *tp);
extern void tcp_unregister_congestion_control(struct tcp_congestion_ops *ca);

extern u32 prandom_u32();

static inline int is_power_of_2(int x)
{
	return (x > 0) && ((x & (x - 1)) == 0);
}

#endif
