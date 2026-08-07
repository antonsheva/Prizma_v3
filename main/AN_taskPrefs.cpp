#include "AN_taskPrefs.h"




void AN_taskPrefs::init(){
 
	nvs_handle_t nvsHandle;
  
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
  }

  ESP_ERROR_CHECK(nvs_open("prefData", NVS_READWRITE, &nvsHandle));


	uint8_t  val8;
	uint64_t val64;
	err = nvs_get_u8(nvsHandle, PARAM_ADDR_ESP, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.esp32Addr = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_ADDR_ESP, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.esp32Addr = val8;		
	}

	err = nvs_get_u8(nvsHandle, PARAM_ADDR_RM_1, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.rebMod[0].address = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_ADDR_RM_1, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.rebMod[0].address = val8;		
	}

	err = nvs_get_u8(nvsHandle, PARAM_ADDR_RM_2, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.rebMod[1].address = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_ADDR_RM_2, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.rebMod[1].address = val8;		
	}

	err = nvs_get_u8(nvsHandle, PARAM_PWR_1, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.rebMod[0].pwr = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_PWR_1, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.rebMod[0].pwr = val8;		
	}

	err = nvs_get_u8(nvsHandle, PARAM_PWR_2, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.rebMod[1].pwr = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_PWR_2, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.rebMod[1].pwr = val8;		
	}
 
	err = nvs_get_u64(nvsHandle, PARAM_DEV_ID, &val64);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.devId = 0;
		ESP_ERROR_CHECK(nvs_set_u64(nvsHandle, PARAM_DEV_ID, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.devId = val64;		
	}

 
	err = nvs_get_u8(nvsHandle, PARAM_DEV_TYPE, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.devType = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_DEV_TYPE, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.devType = val8;		
	}

	err = nvs_get_u8(nvsHandle, PARAM_DEV_RANGE, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.devRange = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_DEV_RANGE, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.devRange = val8;		
	} 

	err = nvs_get_u8(nvsHandle, PARAM_GROUP_ID, &val8);
  if (err == ESP_ERR_NVS_NOT_FOUND){
		G_lJmrStt.groupId = 0;
		ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_GROUP_ID, 0));
		ESP_ERROR_CHECK(nvs_commit(nvsHandle));
  }else{
		G_lJmrStt.groupId = val8;		
	}  

  nvs_close(nvsHandle);
}
 

void AN_taskPrefs::setParam(char* param, BYTE val){
	nvs_handle_t nvsHandle;
	ESP_ERROR_CHECK(nvs_open("prefData", NVS_READWRITE, &nvsHandle));
	ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, param, val));
	ESP_ERROR_CHECK(nvs_commit(nvsHandle)); 
 	nvs_close(nvsHandle);
}

 

void AN_taskPrefs::setParam(char* param, uint64_t val){
	nvs_handle_t nvsHandle;
	ESP_ERROR_CHECK(nvs_open("prefData", NVS_READWRITE, &nvsHandle));
	ESP_ERROR_CHECK(nvs_set_u64(nvsHandle, param, val));
	ESP_ERROR_CHECK(nvs_commit(nvsHandle)); 
 	nvs_close(nvsHandle); 
}


void AN_taskPrefs::setGroupId(BYTE groupId){
	char param[] = PARAM_GROUP_ID;
	if((groupId > 0) && (groupId < 127)){
		G_lJmrStt.groupId = groupId;
		setParam(param, groupId);
	}
}

void AN_taskPrefs::setDevId(uint64_t id){
	char param[] = PARAM_DEV_ID;
	if((id > 0) && (id < 0x7FFFFFFFFFFFFFFF)){
		G_lJmrStt.devId = id;
		setParam(param, id);
	}
}

void AN_taskPrefs::setDevType(BYTE type){
	char param[] = PARAM_DEV_TYPE;
  BYTE tp = (type == DEV_TYPE_B) ? DEV_TYPE_B : DEV_TYPE_A;
	G_lJmrStt.devType = type;
  setParam(param, tp);
}

void AN_taskPrefs::setDevRange(BYTE range){
	char param[] = PARAM_DEV_RANGE;
	if((range > 0) &&(range < 13)){
		G_lJmrStt.devRange = range;
		setParam(param, range);
	}
}

void AN_taskPrefs::setAddrEsp(BYTE addr){
	char param[] = PARAM_ADDR_ESP;	
	if((addr > 0) && (addr < 127)){
		G_lJmrStt.esp32Addr = addr;
		setParam(param, addr);
	}
}

void AN_taskPrefs::setAddrRm(BYTE addrRm1, BYTE addrRm2){
	nvs_handle_t nvsHandle;
	ESP_ERROR_CHECK(nvs_open("prefData", NVS_READWRITE, &nvsHandle));

	ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_ADDR_RM_1, addrRm1));
	ESP_ERROR_CHECK(nvs_commit(nvsHandle)); 

	ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_ADDR_RM_2, addrRm2));
	ESP_ERROR_CHECK(nvs_commit(nvsHandle)); 

 	nvs_close(nvsHandle);

	G_lJmrStt.rebMod[0].address = addrRm1;
	G_lJmrStt.rebMod[1].address = addrRm2;

 
}

void AN_taskPrefs::setPwr(BYTE pwr1, BYTE pwr2){
	nvs_handle_t nvsHandle;	
	BYTE p1 = pwr1 == PWR_OFF ? PWR_OFF : PWR_ON;
	BYTE p2 = pwr2 == PWR_OFF ? PWR_OFF : PWR_ON;
	G_lJmrStt.rebMod[0].pwr = p1;
	G_lJmrStt.rebMod[1].pwr = p2;

	ESP_ERROR_CHECK(nvs_open("prefData", NVS_READWRITE, &nvsHandle));

	ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_PWR_1, pwr1));
	ESP_ERROR_CHECK(nvs_commit(nvsHandle)); 

	ESP_ERROR_CHECK(nvs_set_u8(nvsHandle, PARAM_PWR_2, pwr2));
	ESP_ERROR_CHECK(nvs_commit(nvsHandle)); 

 	nvs_close(nvsHandle);
		
}

void AN_taskPrefs::printAddresses(){
	Serial.println("ad_esp ->  "+String(G_lJmrStt.esp32Addr));
	Serial.println("ad_rm1 ->  "+String(G_lJmrStt.rebMod[0].address));
	Serial.println("ad_rm2 ->  "+String(G_lJmrStt.rebMod[1].address));
}

void AN_taskPrefs::getDevParam(){


	Serial.println("   Параметры устройства   ");
	Serial.println("devId   ->  "+String(G_lJmrStt.devId, HEX));
	Serial.println("groupId ->  "+String(G_lJmrStt.groupId));
	Serial.println("devType ->  "+String(G_lJmrStt.devType));
	Serial.println("devRange->  "+String(G_lJmrStt.devRange));
	
}





void AN_taskPrefs::run(void *param){

  _MSG_PACK msg;
  init();
  for(;;){
    xQueueReceive(QueuePrefs, &msg, portMAX_DELAY);
      
    switch (msg.cmd){
        case CMD_SET_ADDR_ESP   :  setAddrEsp     (msg.addrEsp32);                break;
        case CMD_SET_ADDR_RM    :  setAddrRm      (msg.addrRm1, msg.addrRm2);     break;
        case CMD_SET_PWR        :  setPwr         (msg.pwr1, msg.pwr2);           break;         
        case CMD_SET_DEV_ID     :  setDevId       (msg.devId);                    break;
        case CMD_SET_GROUP_ID   :  setGroupId     (msg.groupId);                  break;
        case CMD_SET_DEV_TYPE   :  setDevType     (msg.devType);                  break;
        case CMD_SET_DEV_RANGE  :  setDevRange    (msg.devRange);                 break;
        case CMD_GET_DEV_PARAM  :  getDevParam    ();                             break;
        case CMD_PRINT_ADDRESSES:  printAddresses ();                             break;
    }  
  }
}

AN_taskPrefs::AN_taskPrefs(/* args */)
{
  
}

AN_taskPrefs::~AN_taskPrefs()
{
}














