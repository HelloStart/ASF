#ifndef __TIMER_HW_H__
#define __TIMER_HW_H__

typedef void (*hw_timer_callback_t)(void);

void hw_timer_init(void);
void hw_timer_register_callback(hw_timer_callback_t cb_ptr);
void hw_timer_start(int dealy);
void hw_timer_stop();

#endif /* __TIMER_HW_H__ */
