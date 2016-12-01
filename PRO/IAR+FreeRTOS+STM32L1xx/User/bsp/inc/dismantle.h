#ifndef __DISMANTLE_H__
#define __DISMANTLE_H__

#define IS_DISMANTLE_HIGH()   ((GPIO_PORT_DISMANTLE->ODR & GPIO_PIN_DISMANTLE) == GPIO_PIN_DISMANTLE)

typedef enum
{
    rising_edge,
    falling_edge,
    none_edge
}dismantle_trig_edge_t;

extern dismantle_trig_edge_t dismantle_trig_edge;

void dismantle_int_init(void);

#endif