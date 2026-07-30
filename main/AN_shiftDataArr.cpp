#include "../include/AN_shiftDataArr.h"
#include<string>

AN_shiftDataArr::AN_shiftDataArr(/* args */)
{
}

AN_shiftDataArr::~AN_shiftDataArr()
{
}

void AN_shiftDataArr::loadMsgToJmrStt(_MSG_PACK *msg, JammerState *jmmr){
	jmmr->devId							= msg->devId;      
	jmmr->groupId						= msg->groupId; 
	jmmr->devType						= msg->devType;  	
	jmmr->rebMod[0].mc      = msg->modCode1;
	jmmr->rebMod[1].mc      = msg->modCode2;
	jmmr->rebMod[0].mask    = msg->mask1;
	jmmr->rebMod[1].mask    = msg->mask2;
	jmmr->rebMod[0].pwr     = msg->pwr1;
	jmmr->rebMod[1].pwr     = msg->pwr2;
	jmmr->infoLen						= msg->txtLen;

	memset(jmmr->info, 0, TXT_INFO_LEN);
	memccpy(jmmr->info, msg->txt, '\0', TXT_INFO_LEN);
  
  if(msg->addrRm1 && msg->addrRm1 < 128)jmmr->rebMod[0].address = msg->addrRm1;
  if(msg->addrRm2 && msg->addrRm2 < 128)jmmr->rebMod[1].address = msg->addrRm2;
}

void AN_shiftDataArr::loadJmmrStateToMsg(_MSG_PACK *msg, JammerState *jmmr){
	msg->devId      = jmmr->devId;        
	msg->groupId    = jmmr->groupId;             
	msg->devType    = jmmr->devType;  	

	msg->addrRm1   	= jmmr->rebMod[0].address;
  msg->modCode1  	= jmmr->rebMod[0].mc;
	msg->mask1     	= jmmr->rebMod[0].mask;
	msg->pwr1      	= jmmr->rebMod[0].pwr;

	msg->addrRm2   	= jmmr->rebMod[1].address;
  msg->modCode2  	= jmmr->rebMod[1].mc;	
	msg->mask2     	= jmmr->rebMod[1].mask;
	msg->pwr2      	= jmmr->rebMod[1].pwr;         
}

void AN_shiftDataArr::copyJmmr(JammerState *jmmr1, JammerState *jmmr2){
 
	jmmr1->devId   								= jmmr2->devId								;
	jmmr1->groupId 								= jmmr2->groupId							;
	jmmr1->devType 								= jmmr2->devType							;
	jmmr1->esp32Addr 							= jmmr2->esp32Addr						;
	jmmr1->infoLen 					  		= jmmr2->infoLen							;
	 

	memset(jmmr1->info, 0, TXT_INFO_LEN);
  memccpy(jmmr1->info, jmmr2->info, '\0', TXT_INFO_LEN);
  

	for(int i=0; i<2; i++){
		jmmr1->rebMod[i].mc          	= jmmr2->rebMod[i].mc         ;    			
		jmmr1->rebMod[i].mask        	= jmmr2->rebMod[i].mask       ;      				
		jmmr1->rebMod[i].address     	= jmmr2->rebMod[i].address    ;         				
		jmmr1->rebMod[i].echo        	= jmmr2->rebMod[i].echo       ;      				
		jmmr1->rebMod[i].pwr         	= jmmr2->rebMod[i].pwr        ;     				
		jmmr1->rebMod[i].vcpu        	= jmmr2->rebMod[i].vcpu       ;      					
		jmmr1->rebMod[i].temp        	= jmmr2->rebMod[i].temp       ;      					
		// jmmr1->rebMod[i].infoLen 			= jmmr2->rebMod[i].infoLen		; 
		
		// memset(jmmr1->rebMod[i].info, 0, TXT_INFO_LEN);	
		// memccpy(jmmr1->rebMod[i].info, jmmr2->rebMod[i].info, '\0', TXT_INFO_LEN);	
		 

	}
}

void AN_shiftDataArr::printJmmrData(JammerState *jmmr){
		AN_print("----------------");
		AN_print("devId   ->  "+std::to_string(jmmr->devId));
		AN_print("groupId ->  "+std::to_string(jmmr->groupId));
		AN_print("devType ->  "+std::to_string(jmmr->devType));
		AN_print("ESP addr->  "+std::to_string(jmmr->esp32Addr));
		AN_print("addrRm1 ->  "+std::to_string(jmmr->rebMod[0].address));
		AN_print("addrRm2 ->  "+std::to_string(jmmr->rebMod[1].address));
		AN_print("mc1     ->  "+std::to_string(jmmr->rebMod[0].mc));
		AN_print("mc2     ->  "+std::to_string(jmmr->rebMod[1].mc));
		AN_print("mask1   ->  "+std::to_string(jmmr->rebMod[0].mask));
		AN_print("mask2   ->  "+std::to_string(jmmr->rebMod[1].mask));	
		AN_print("pwr1    ->  "+std::to_string(jmmr->rebMod[0].pwr));	
		AN_print("pwr2    ->  "+std::to_string(jmmr->rebMod[1].pwr));	
}

void AN_shiftDataArr::printMsg(_MSG_PACK *msg){
	AN_print(" --- Message  --- ");
	AN_print("cmd       -> "+std::to_string(msg->cmd)   );
	AN_print("sender    -> "+std::to_string(msg->sender)   );
	AN_print("response  -> "+std::to_string(msg->response)   );
	AN_print("direction -> "+std::to_string(msg->direction)   );
	AN_print("dev_id    -> "+std::to_string(msg->devId)   );
	AN_print("group_id  -> "+std::to_string(msg->groupId)   );
	AN_print("dev_type  -> "+std::to_string(msg->devType)   );
	AN_print("dev_range -> "+std::to_string(msg->devRange)   );
	AN_print("ad_esp    -> "+std::to_string(msg->addrEsp32)   );
	AN_print("ad_rm1    -> "+std::to_string(msg->addrRm1)   );
	AN_print("ad_rm2    -> "+std::to_string(msg->addrRm2)   );
	AN_print("mc1       -> "+std::to_string(msg->modCode1)   );
	AN_print("mc2       -> "+std::to_string(msg->modCode2)   );
	AN_print("msk1      -> "+std::to_string(msg->mask1)   );
	AN_print("msk2      -> "+std::to_string(msg->mask2)   );
	AN_print("pwr1      -> "+std::to_string(msg->pwr1)   );
	AN_print("pwr2      -> "+std::to_string(msg->pwr1)   );
	AN_print("txt_len   -> "+std::to_string(msg->txtLen)   );
	AN_print("txt       -> "+std::string(msg->txt)   );	
}

void AN_shiftDataArr::printJmmrList(){
    AN_print("--------printJmmrList--------");


		for(int i=0; i<G_jmmrsList.size(); i++){
			printJmmrData(&G_jmmrsList[i]);		
		}

}



