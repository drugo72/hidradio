#include "pal.h"
#include "tal.h"

void radio_phy_task_init(void)
{
  tal_init();
}

void radio_phy_task(void)
{
  tal_task();
  pal_task();
}
