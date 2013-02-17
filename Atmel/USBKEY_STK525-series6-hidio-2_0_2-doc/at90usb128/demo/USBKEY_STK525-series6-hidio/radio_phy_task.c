#include "pal.h"
#include "tal.h"
#include "ieee_const.h"

void radio_phy_task_init(void)
{
  uint8_t channel = 26;

  tal_init();

  tal_pib_set(phyCurrentChannel, (pib_value_t *)&channel);
  tal_rx_enable(PHY_RX_ON);
}

void radio_phy_task(void)
{
  tal_task();
  pal_task();
}
