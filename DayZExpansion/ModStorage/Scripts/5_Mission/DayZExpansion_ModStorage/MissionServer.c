#ifdef EXPANSION_MODSTORAGE
modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		EXPrint("ModStorage v" + CF_ModStorage.VERSION);
	}

#ifdef EXPANSION_MODSTORAGE_DEBUG
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		Param1<string> stringParam;
		ClientPrepareEventParams clientPrepareParams;
		ClientNewEventParams newParams;
		ClientReadyEventParams readyParams;
		ClientRespawnEventParams clientRespawnParams;
		ClientReconnectEventParams reconnectParams;
		ClientDisconnectedEventParams discoParams;
		LogoutCancelEventParams logoutCancelParams;

		if (Class.CastTo(stringParam, params))
		{
			if (stringParam.param1.IndexOf("SCRIPT       :") != 0)
				EXPrint(ToString() + "::OnEvent " + stringParam.param1);
		}
		else if (Class.CastTo(clientPrepareParams, params))
			EXPrint(ToString() + "::OnEvent " + clientPrepareParams.param1 + " " + clientPrepareParams.param2 + " " + clientPrepareParams.param3 + " " + clientPrepareParams.param4 + " " + clientPrepareParams.param5);
		else if (Class.CastTo(newParams, params))
			EXPrint(ToString() + "::OnEvent " + newParams.param1 + " " + newParams.param2 + " " + newParams.param3);
		else if (Class.CastTo(readyParams, params))
			EXPrint(ToString() + "::OnEvent " + readyParams.param1 + " " + readyParams.param2);
		else if (Class.CastTo(clientRespawnParams, params))
			EXPrint(ToString() + "::OnEvent " + clientRespawnParams.param1 + " " + clientRespawnParams.param2);
		else if (Class.CastTo(reconnectParams, params))
			EXPrint(ToString() + "::OnEvent " + reconnectParams.param1 + " " + reconnectParams.param2);
		else if (Class.CastTo(discoParams, params))
			EXPrint(ToString() + "::OnEvent " + discoParams.param1 + " " + discoParams.param2 + " " + discoParams.param3 + " " + discoParams.param4);
		else if (Class.CastTo(logoutCancelParams, params))
			EXPrint(ToString() + "::OnEvent " + logoutCancelParams.param1);
		else
			EXPrint(ToString() + "::OnEvent " + params);

		switch (eventTypeId)
		{
			case ClientPrepareEventTypeID:
				EXPrint(ToString() + "::OnEvent ClientPrepareEventTypeID");
				break;

			case ClientNewEventTypeID:
				EXPrint(ToString() + "::OnEvent ClientNewEventTypeID");
				break;
			
			case ClientReadyEventTypeID:
				EXPrint(ToString() + "::OnEvent ClientReadyEventTypeID");
				break;
						
			case ClientRespawnEventTypeID:
				EXPrint(ToString() + "::OnEvent ClientRespawnEventTypeID");
				break;
				
			case ClientReconnectEventTypeID:
				EXPrint(ToString() + "::OnEvent ClientReconnectEventTypeID");
				break;
			
			case ClientDisconnectedEventTypeID:
				EXPrint(ToString() + "::OnEvent ClientDisconnectedEventTypeID");
				break;
				
			case LogoutCancelEventTypeID:
				EXPrint(ToString() + "::OnEvent LogoutCancelEventTypeID");
				break;
		}

		super.OnEvent(eventTypeId, params);
	}
#endif

	override void OnClientPrepareEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int preloadTimeout)
	{
		PlayerBase.Expansion_QueueIdentityId(identity.GetId());

		super.OnClientPrepareEvent(identity, useDB, pos, yaw, preloadTimeout);
	}
}
#endif
