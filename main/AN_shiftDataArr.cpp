#include "../include/AN_shiftDataArr.h"
#include<string>
#include "AN_shiftDataArr.h"

AN_shiftDataArr::AN_shiftDataArr(/* args */)
{
}

AN_shiftDataArr::~AN_shiftDataArr()
{
}

void AN_shiftDataArr::loadMsgToJmrStt(_MSG_PACK *msg, _JMMR_STATE *jmmr, bool rmOnly){
	if(!rmOnly){
		jmmr->devId							= msg->devId;      
		jmmr->groupId						= msg->groupId; 
		jmmr->devType						= msg->devType; 		
		jmmr->devRange					= msg->devRange; 
		jmmr->esp32Addr 				= msg->addrEsp32; 
		jmmr->infoLen						= msg->txtLen;		
		jmmr->devBattStt   	    = msg->devBattStt ;	
		jmmr->devTemper   		  = msg->devTemper  ;

		memset(jmmr->info, 0, TXT_INFO_LEN);
		memccpy(jmmr->info, msg->txt, '\0', TXT_INFO_LEN);
		
		if(msg->addrRm1 && msg->addrRm1 < 128)jmmr->rebMod[0].address = msg->addrRm1;
		if(msg->addrRm2 && msg->addrRm2 < 128)jmmr->rebMod[1].address = msg->addrRm2;		
	}
 	
	jmmr->rebMod[0].mc      = msg->modCode1;
	jmmr->rebMod[1].mc      = msg->modCode2;
	jmmr->rebMod[0].mask    = msg->mask1;
	jmmr->rebMod[1].mask    = msg->mask2;
	jmmr->rebMod[0].pwr     = msg->pwr1;
	jmmr->rebMod[1].pwr     = msg->pwr2;
}




void AN_shiftDataArr::loadJmmrStateToMsg(_MSG_PACK *msg, _JMMR_STATE *jmmr){
	msg->devId      = jmmr->devId;        
	msg->groupId    = jmmr->groupId;             
	msg->devType    = jmmr->devType;  	
	msg->devRange   = jmmr->devRange;  	
	msg->addrEsp32  = jmmr->esp32Addr;  		

	msg->devBattStt = jmmr->devBattStt;	
	msg->devTemper  = jmmr->devTemper ;


	msg->addrRm1   	= jmmr->rebMod[0].address;
  msg->modCode1  	= jmmr->rebMod[0].mc;
	msg->mask1     	= jmmr->rebMod[0].mask;
	msg->pwr1      	= jmmr->rebMod[0].pwr;

	msg->addrRm2   	= jmmr->rebMod[1].address;
  msg->modCode2  	= jmmr->rebMod[1].mc;	
	msg->mask2     	= jmmr->rebMod[1].mask;
	msg->pwr2      	= jmmr->rebMod[1].pwr;         
}

void AN_shiftDataArr::copyJmmr(_JMMR_STATE *jmmr1, _JMMR_STATE *jmmr2, bool rmDataOnly){
  if(!rmDataOnly){
		jmmr1->devId   								= jmmr2->devId								;
		jmmr1->groupId 								= jmmr2->groupId							;
		jmmr1->devType 								= jmmr2->devType							;
		jmmr1->devRange								= jmmr2->devRange							;		
		jmmr1->esp32Addr 							= jmmr2->esp32Addr						;		
		jmmr1->devBattStt   	        = jmmr2->devBattStt           ;	
		jmmr1->devTemper   		        = jmmr2->devTemper            ;
	}

	jmmr1->infoLen 					  		= jmmr2->infoLen							  ;
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
		jmmr1->rebMod[i].infoLen 			= jmmr2->rebMod[i].infoLen		; 

		if(jmmr1->rebMod[i].infoLen){
			memset(jmmr1->rebMod[i].info, 0, TXT_INFO_LEN);	
			memccpy(jmmr1->rebMod[i].info, jmmr2->rebMod[i].info, '\0', TXT_INFO_LEN);	
		}
	}
}

void AN_shiftDataArr::printJmmrData(_JMMR_STATE *jmmr){
		Serial.println(" JMMR DATA ");
		Serial.println("devId   ->  "+String(jmmr->devId, HEX));
		Serial.println("groupId ->  "+String(jmmr->groupId));
		Serial.println("devType ->  "+String(jmmr->devType));
		Serial.println("devRange->  "+String(jmmr->devRange));		
		Serial.println("ESP addr->  "+String(jmmr->esp32Addr));
		Serial.println("addrRm1 ->  "+String(jmmr->rebMod[0].address));
		Serial.println("addrRm2 ->  "+String(jmmr->rebMod[1].address));
		Serial.println("mc1     ->  "+String(jmmr->rebMod[0].mc));
		Serial.println("mc2     ->  "+String(jmmr->rebMod[1].mc));
		Serial.println("mask1   ->  "+String(jmmr->rebMod[0].mask));
		Serial.println("mask2   ->  "+String(jmmr->rebMod[1].mask));	
		Serial.println("pwr1    ->  "+String(jmmr->rebMod[0].pwr));	
		Serial.println("pwr2    ->  "+String(jmmr->rebMod[1].pwr));	
		Serial.println("battStt ->  "+String(jmmr->devBattStt));	
		Serial.println("temper  ->  "+String(jmmr->devTemper));	



		// Serial.println(" -- info RM1 --");
		// if(jmmr->rebMod[0].infoLen)Serial.println(jmmr->rebMod[0].info);
		// Serial.println(" -- info RM2 --");
		// if(jmmr->rebMod[1].infoLen)Serial.println(jmmr->rebMod[1].info);
}

void AN_shiftDataArr::printMsg(_MSG_PACK *msg){
	Serial.println(" --- Message  --- ");
	Serial.println("dev_id    -> "+String(msg->devId)   );
	Serial.println("group_id  -> "+String(msg->groupId)   );
	Serial.println("dev_type  -> "+String(msg->devType)   );
	Serial.println("dev_range -> "+String(msg->devRange)   );
	Serial.println("ad_esp    -> "+String(msg->addrEsp32)   );
	Serial.println("battStt 	->  "+String(msg->devBattStt));	
	Serial.println("temper  	->  "+String(msg->devTemper));	
	Serial.println("ad_rm1    -> "+String(msg->addrRm1)   );
	Serial.println("ad_rm2    -> "+String(msg->addrRm2)   );
	Serial.println("mc1       -> "+String(msg->modCode1)   );	
	Serial.println("cmd       -> "+String(msg->cmd)   );
	Serial.println("sender    -> "+String(msg->sender)   );
	Serial.println("response  -> "+String(msg->response)   );
	Serial.println("direction -> "+String(msg->direction)   );
	Serial.println("mc2       -> "+String(msg->modCode2)   );
	Serial.println("msk1      -> "+String(msg->mask1)   );
	Serial.println("msk2      -> "+String(msg->mask2)   );
	Serial.println("pwr1      -> "+String(msg->pwr1)   );
	Serial.println("pwr2      -> "+String(msg->pwr2)   );
	Serial.println("txt_len   -> "+String(msg->txtLen)   );
	Serial.println("txt       -> "+String(msg->txt)   );	

}

void AN_shiftDataArr::printJmmrList(){
    Serial.println("--------printJmmrList--------");
		if(G_jmmrsList.size() == 0){
			Serial.println("  JmmrList is empty ");
		}else{
			for(int i=0; i<G_jmmrsList.size(); i++){
				printJmmrData(&G_jmmrsList[i]);		
			}			
		}
}



