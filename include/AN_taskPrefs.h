#ifndef _AN_TASK_PREFERENCE_
#define _AN_TASK_PREFERENCE_


#include "Arduino.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "define_types.h"
#include "define.h"
#include "AN_objects.h"

class AN_taskPrefs{
private:

public:
  AN_taskPrefs(/* args */);
  ~AN_taskPrefs();

  static void run(void *param);
  static void setParam(char *param, BYTE val);
  static void setParam(char *param, uint64_t val);
  static void setGroupId(BYTE groupId);
  static void setDevId(uint64_t id);
  static void setDevType(BYTE type);
  static void setDevRange(BYTE range);
  static void setAddrEsp(BYTE addr);
  static void setAddrRm(BYTE addrRm1, BYTE addrRm2);
  static void setPwr(BYTE pwr1, BYTE pwr2);
  static void printAddresses();
  static void getDevParam();
  static void init();
};

#endif