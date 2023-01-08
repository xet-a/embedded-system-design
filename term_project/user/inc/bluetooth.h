#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "common.h"

typedef struct _BT_CONFIG {
  bool isValid;
  bool isATscan;
}BTConfig;

void BT_init(BTConfig *config);
int BT_send_data(const char *data, BTConfig *config);

#endif