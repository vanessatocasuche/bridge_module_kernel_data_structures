#ifndef CLIENT_BRIDGE_H
#define CLIENT_BRIDGE_H
#include <linux/ioctl.h>

// cmd ‘BRIDGE_DATA_VAR’ to send a message to the module

#define BRIDGE_CREATE_Q _IO('p', 1) 			//Create a queue
#define BRIDGE_W_HIGH_PRIOR_Q _IOW('p', 2, char *)
#define BRIDGE_W_MIDDLE_PRIOR_Q _IOW('p', 3, char *)
#define BRIDGE_W_LOW_PRIOR_Q _IOW('p', 4, char *)
#define BRIDGE_R_HIGH_PRIOR_Q _IOR('p', 5, char *)
#define BRIDGE_R_MIDDLE_PRIOR_Q _IOR('p', 6, char *)
#define BRIDGE_R_LOW_PRIOR_Q _IOR('p', 7, char *)
#define BRIDGE_STATE_Q _IO('p', 8)
#define BRIDGE_DESTROY_Q _IO('p', 9)

#define BRIDGE_CREATE_S _IO('p', 10) 			//Create a stack
#define BRIDGE_W_S _IOW('p', 11, char *)
#define BRIDGE_R_S _IOR('p', 12, char *)
#define BRIDGE_STATE_S _IO('p', 13)
#define BRIDGE_DESTROY_S _IO('p', 14)

#define BRIDGE_CREATE_L _IO('p', 15)			//Create a list
#define BRIDGE_W_L _IOW('p', 16, char *)
#define BRIDGE_R_L _IOR('p', 17, char *)
#define BRIDGE_INVERT_L _IO('p', 18)
#define BRIDGE_ROTATE_L _IOW('p', 19, int *)
#define BRIDGE_CLEAN_L _IO('p', 20)
#define BRIDGE_GREATER_VAL_L _IOR('p', 21, char *)
#define BRIDGE_END_L _IO('p', 22)
#define BRIDGE_CONCAT_L _IO('p', 23)
#define BRIDGE_STATE_L _IO('p', 24)
#define BRIDGE_DESTROY_L _IO('p', 25)

#define BRIDGE_W_CS _IOW('p', 26, unsigned long*)

struct complex_struct{
	int value;
	char messages[3][100];
};

//List--------------------------------
struct list_head {
    struct list_head *next, *prev;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
        list->next = list;
        list->prev = list;
}

static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
        __list_add(new, head, head->next);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
        next->prev = prev;
        prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
}

#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER)  __compiler_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)


#define list_for_each_entry(pos, head, member)              \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
         &pos->member != (head);                    \
         pos = list_next_entry(pos, member))


#endif