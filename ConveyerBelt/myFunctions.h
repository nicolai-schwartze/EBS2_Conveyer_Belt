
#ifndef MYFUNCTIONS_H_
#define MYFUNCTIONS_H_
	void StopToChainIdle();
	void StopToServiceIdle();
	//Service
	void ServiceToStop();
	void ServiceIdleToRampRising();
	void ServiceRampRising();
	void ServiceRampRisingToConst();
	void ServiceRampConstToRampFall();
	void ServiceRampFall();
	void ServiceRampFallingToIdle();

	void ModifyTRToModifyTT();
	void ModifyTTToModifyTF();
	void ModifyTFToServiceIdle();
	


	
	
	
	
	
	
	//Chain
	void ChainToStop();
	void ChainIdleToChainSlowRecive();
	void ChainRampRising();
	void ChainSlowReciveToChainRampRising();
	void ChainRampRisingToChainRampConst();
	void ChainRampConstToChainRampFall();
	void ChainRampFall();
	void ChainRampFallToWaiting();
	void WaitingToChainSlowSend();
	void ChainSlowSendToChainIdle();
	void Waiting();
		
	bool myConditionTrue();

#endif // MYFUNCTIONS_H_
