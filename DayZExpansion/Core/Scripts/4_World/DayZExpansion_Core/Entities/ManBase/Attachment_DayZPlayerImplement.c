/**
 * Attachment_DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplement
{
	Object m_ExAttachmentObject;
	ExpansionPlayerLink m_ExPlayerLinkType;
	ExpansionPlayerLink m_ExPlayerLinkTypeFrameDelayed;
	bool m_ExIsAwaitingServerLink;
	bool m_ExPerformClimbAttach;
	bool m_ExPerformClimb;

	bool m_ExForceUnlink;

	bool m_IsAttached;

	ref SHumanCommandClimbResult m_ExClimbResult;
	int m_ExClimbType;

	float m_ExAttachmentRadius;

	bool m_ExIsPreparingVehicle;
	bool m_ExPerformVehicleGetIn;

	int m_ExLastCheckedTimeStamp;

	vector m_ExTransformPlayer[4];
	vector m_ExTransformTarget[4];
	vector m_ExTransformLocal[4];

	ref array<ref RaycastRVResult> m_ExRaycastResults = new array<ref RaycastRVResult>();
	ExpansionPlayerRaycastResult m_ExRaycastResultType;
	ref RaycastRVParams m_ExRaycastRVParams = new RaycastRVParams("0 0 0", "0 0 0", this);
	int m_ExCurrentCommandID;

	void DayZPlayerImplement()
	{
		//SetEventMask(EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE);
		//SetEventMask(EntityEvent.FRAME | EntityEvent.POSTFRAME);

		RegisterNetSyncVariableBool("m_IsAttached");

		m_ExClimbResult = new SHumanCommandClimbResult;
	}

	override void DeferredInit()
	{
		super.DeferredInit();

#ifdef CF_DebugUI
		CF_Debug.Create(this);
#endif
	}

	void ~DayZPlayerImplement()
	{
#ifdef CF_DebugUI
		CF_Debug.Destroy(this);
#endif
	}

	bool CallExpansionClimbCode()
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

		Object attchObh;
		if (!Object.CastTo(attchObh, m_ExClimbResult.m_ClimbStandPointParent))
			return false;

		if (!ExpansionAttachmentHelper.CanAttachTo(this, attchObh))
			return false;

		if (attchObh == GetParent())
			return false;

		return true;
	}

	void OnClimbStart(int climbType)
	{
		auto trace = CF_Trace_1(EXTrace.PLAYER, this).Add(climbType);

		//if (m_ExPlayerLinkType != ExpansionPlayerLink.NONE || m_ExIsAwaitingServerLink || m_ExClimbType != -1)
		//{
		//	return;
		//}

		m_ExClimbType = climbType;

		Object.CastTo(m_ExAttachmentObject, m_ExClimbResult.m_ClimbStandPointParent);

		if (!ExpansionAttachmentHelper.CanAttachTo(this, m_ExAttachmentObject))
		{
			trace.Info("Do not need to attach to object " + m_ExAttachmentObject);

			m_ExAttachmentObject = NULL;

			StartCommand_Climb(m_ExClimbResult, m_ExClimbType);

			m_ExClimbType = -1;

			return;
		}

		if (m_ExAttachmentObject == GetParent())
		{
			trace.Info("Can climb because we are already attached " + m_ExAttachmentObject);

			StartCommand_Climb(m_ExClimbResult, m_ExClimbType);

			m_ExClimbType = -1;

			return;
		}

		if (GetParent())
		{
			trace.Info("Unlink from parent " + GetParent());

			UnlinkFromLocalSpace();

			m_IsAttached = false;

			m_ExIsAwaitingServerLink = true;

			if (GetGame().IsServer())
			{
				DayZPlayerSyncJunctures.ExpansionSendPerformClimb(this, false, true);
			}
		}
		else
		{
			trace.Info("Will perform attach, then climb");
			
			m_ExPerformClimbAttach = true;

			m_ExIsAwaitingServerLink = true;
		}
	}

	override bool CanClimb(int climbType, SHumanCommandClimbResult climbRes)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

		if (m_ExPlayerLinkType != ExpansionPlayerLink.NONE)
			return false;

		return super.CanClimb(climbType, climbRes);
	}

	/**
	 * @brief When linking to an already attached vehicle
	 */
	void Expansion_PrepareGettingInVehicle()
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

		if (!Class.CastTo(m_ExAttachmentObject, GetParent()))
		{
			CF_Log.Error("'Expansion_PrepareGettingInVehicle' called when not parented to anything");
			return;
		}

		m_ExIsPreparingVehicle = true;
		m_ExPerformVehicleGetIn = false;

		m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

		UnlinkFromLocalSpace();

		m_IsAttached = false;

		if (GetGame().IsServer())
		{
			trace.Info("Sending to client");
			DayZPlayerSyncJunctures.ExpansionSendGetInTransportUnlink(this);
		}
	}

	Object Expansion_GetAttachmentObject()
	{
		return m_ExAttachmentObject;
	}

	void Expansion_GettingOutVehicle()
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

		if (!Class.CastTo(m_ExAttachmentObject, GetParent()))
		{
			CF_Log.Error("'Expansion_GettingOutVehicle' called when not parented to anything");
		}
	}

	bool CanPerformVehicleGetIn()
	{
		return m_ExPerformVehicleGetIn;
	}

	bool IsPreparingVehicle()
	{
		return m_ExIsPreparingVehicle;
	}

	void EndVehiclePrep()
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

		m_ExIsPreparingVehicle = false;
		m_ExPerformVehicleGetIn = false;

		if (GetGame().IsClient())
		{
			trace.Info("Sending to server");

			ScriptInputUserData ctx = new ScriptInputUserData;
			ctx.Write(EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK);

			ctx.Send();
		}
	}

	override bool IsInTransport()
	{
		if (GetCommand_Vehicle())
			return true;

		return false;
	}

	override bool HandleDeath(int pCurrentCommandID)
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE && m_DeathAnimType != -2)
		{
			HumanCommandVehicle hcv = GetCommand_Vehicle();
			Transport transport = hcv.GetTransport();
			if (!transport) //! We died in a wreck, it no longer exists
			{
				if (m_DeathAnimType != -2 && g_Game.GetMissionState() == g_Game.MISSION_STATE_GAME)
				{
					int type = GetTypeOfDeath(pCurrentCommandID);

					DayZPlayerCommandDeathCallback callback = DayZPlayerCommandDeathCallback.Cast(StartCommand_Death(type, m_DeathHitDir, DayZPlayerCommandDeathCallback));
					callback.m_pPlayer = PlayerBase.Cast(this);

					ResetDeathStartTime();
					GetGame().GetWorld().SetVoiceOn(false);

					return true;
				}

				return false;
			}
		}

		return super.HandleDeath(pCurrentCommandID);
	}

	bool IsAttached()
	{
		return m_IsAttached;
	}

	void AttachTo(IEntity target, bool already_validated = false)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

		m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

		if (IsMissionClient() && Class.CastTo(m_ExAttachmentObject, target))
		{
			m_ExPlayerLinkType = ExpansionPlayerLink.ATTACH;

			// vector size[2];
			// float radius = target.ClippingInfo( size );

			if (already_validated || ExpansionAttachmentHelper.CanAttachTo(this, m_ExAttachmentObject))
			{
				vector size[2];
				m_ExAttachmentRadius = m_ExAttachmentObject.ClippingInfo(size);

				trace.Info("success");
			}
			else
			{
				m_ExAttachmentObject = NULL;
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

				trace.Info("failure");
			}
		}

		if (m_ExAttachmentObject)
		{
			m_IsAttached = true;
		}
	}

	void Detach()
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

		m_ExPlayerLinkType = ExpansionPlayerLink.NONE;
		m_IsAttached = false;

		if (IsMissionClient())
		{
			m_ExPlayerLinkType = ExpansionPlayerLink.DETACH;
		}
	}

	override bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		CF_Trace trace;

		if (super.OnInputUserDataProcess(userDataType, ctx))
		{
			return true;
		}

		if (userDataType == EXPANSION_INPUT_UDT_PLAYER_LINK)
		{
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("EXPANSION_INPUT_UDT_PLAYER_LINK");

			if (!ctx.Read(m_ExPlayerLinkType))
			{
				m_ExPlayerLinkType == ExpansionPlayerLink.NONE;

				CF_Log.Error("[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read task type for EXPANSION_INPUT_UDT_PLAYER_LINK");
			}

			if (m_ExPlayerLinkType == ExpansionPlayerLink.ATTACH)
			{
				if (!ctx.Read(m_ExAttachmentObject))
				{
					m_ExAttachmentObject = null;

					CF_Log.Error("[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read parent object for EXPANSION_INPUT_UDT_PLAYER_LINK");
				}

				if (m_ExAttachmentObject == null || ExpansionAttachmentHelper.CanAttachTo(this, m_ExAttachmentObject) == false)
				{
					trace.Info("Force Reject");

					m_ExAttachmentObject = null;
					m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

					DayZPlayerSyncJunctures.ExpansionSendForceUnlink(this);
				}

				trace.Info("m_ExAttachmentObject: " + m_ExAttachmentObject);
				trace.Info("m_ExPlayerLinkType: " + m_ExPlayerLinkType);
			}

			return true;
		}

		if (userDataType == EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK)
		{
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK");

			m_ExPerformVehicleGetIn = true;

			return true;
		}

		if (userDataType == EXPANSION_INPUT_UDT_PERFORM_CLIMB)
		{
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("EXPANSION_INPUT_UDT_PERFORM_CLIMB");

			m_ExPerformClimb = true;
			return true;
		}

		return false;
	}

	override void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		CF_Trace trace;

		super.OnSyncJuncture(pJunctureID, pCtx);

		switch (pJunctureID)
		{
		case DayZPlayerSyncJunctures.EXPANSION_SJ_NEXT_LINK:
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("EXPANSION_SJ_NEXT_LINK");
			DayZPlayerSyncJunctures.ExpansionReadNextLink(pCtx);

			m_ExIsAwaitingServerLink = false;
			break;
		case DayZPlayerSyncJunctures.EXPANSION_SJ_PERFORM_CLIMB:
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("EXPANSION_SJ_PERFORM_CLIMB");

			DayZPlayerSyncJunctures.ExpansionReadPerformClimb(pCtx, m_ExPerformClimb, m_ExPerformClimbAttach);
			break;
		case DayZPlayerSyncJunctures.EXPANSION_SJ_GET_IN_TRANSPORT_UNLINK:
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("EXPANSION_SJ_GET_IN_TRANSPORT_UNLINK");

			DayZPlayerSyncJunctures.ExpansionReadGetInTransportUnlink(pCtx);

			m_ExPerformVehicleGetIn = true;
			break;
		case DayZPlayerSyncJunctures.EXPANSION_SJ_FORCE_UNLINK:
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("EXPANSION_SJ_FORCE_UNLINK");

			DayZPlayerSyncJunctures.ExpansionReadForceUnlink(pCtx);

			m_ExForceUnlink = true;
			m_ExIsAwaitingServerLink = false;
			break;
		}
	}

	ExpansionPlayerRaycastResult AttachRaycastCheck(int pCurrentCommandID)
	{
		IEntity parent = GetParent();
		HumanCommandLadder ladderCommand = GetCommand_Ladder();
		HumanCommandSwim swimCommand = GetCommand_Swim();
		HumanCommandFall fallCommand = GetCommand_Fall();

		if (m_ExPerformVehicleGetIn || m_ExIsPreparingVehicle || IsInTransport() || pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE)
			return ExpansionPlayerRaycastResult.FALSE;

		if (swimCommand && parent)
			return ExpansionPlayerRaycastResult.DETACH;

		if (swimCommand)
			return ExpansionPlayerRaycastResult.FALSE;

		if (fallCommand && parent)
			return ExpansionPlayerRaycastResult.DETECT_FALL;

		if (fallCommand)
			return ExpansionPlayerRaycastResult.FALSE;

		if (ladderCommand)
			return ExpansionPlayerRaycastResult.FALSE;

		return ExpansionPlayerRaycastResult.DETECT;
	}

	override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (pCurrentCommandFinished)
		{
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS)
			{
				if ((m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_VEHICLE) && (m_TransportCache != NULL))
				{
					int crew_index = m_TransportCache.CrewMemberIndex(this);
					int seat = m_TransportCache.GetSeatAnimationType(crew_index);
					StartCommand_Vehicle(m_TransportCache, crew_index, seat, true);
					m_TransportCache = NULL;
					return true;
				}
			}

			if (PhysicsIsFalling(true))
			{
				StartCommand_Fall(0);
				m_FallYDiff = GetPosition()[1];

				return true;
			}

			if (m_Swimming.m_bWasSwimming)
			{
				StartCommand_Swim();

				return true;
			}

			StartCommand_Move();

			return true;
		}

		if (super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished))
			return true;

		return false;
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		m_ExCurrentCommandID = pCurrentCommandID;

		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);

		Expansion_RunAttachment(pDt, pCurrentCommandID, pCurrentCommandFinished, false);
	}

	/**
	 * @brief Handles the attachment to and from objects for players
	 * 
	 * @param pDt The time delta since this function was last called
	 * @param pCurrentCommandID The current running command (see @DayZPlayerConstants)
	 * @param pCurrentCommandFinished Whether the running command was finished or not
	 * @param pPerformOnServer If the attachment logic should be performed on the server or client. Setting to true for player controlled entities is not tested and will potentially cause issues. 
	 */
	void Expansion_RunAttachment(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished, bool pPerformOnServer)
	{
		CF_Trace trace;

		if (m_ExForceUnlink && (IsMissionClient() || pPerformOnServer))
		{
			if (m_ExAttachmentObject == GetParent() && m_ExAttachmentObject)
			{
				UnlinkFromLocalSpace();
				m_IsAttached = false;

				OnExpansionDetachFrom(m_ExAttachmentObject);

				//m_ExAttachmentObject.Update();

				m_ExAttachmentObject = NULL;
			}

			m_ExForceUnlink = false;
		}

		if (GetGame().IsClient() && !ScriptInputUserData.CanStoreInputUserData())
		{
			return;
		}

		if (m_ExPerformClimb)
		{
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("PerformClimb");

			StartCommand_Climb(m_ExClimbResult, m_ExClimbType);

			m_ExClimbType = -1;

			m_ExPerformClimb = false;

			if (!pPerformOnServer)
			{
				trace.Info("pPerformOnServer: " + pPerformOnServer);

				if (GetGame().IsServer())
				{
					DayZPlayerSyncJunctures.ExpansionSendNextLink(this);
				}
				else
				{
					ScriptInputUserData ctx1 = new ScriptInputUserData;
					ctx1.Write(EXPANSION_INPUT_UDT_PERFORM_CLIMB);

					ctx1.Send();
				}
			}

			return;
		}

		if (m_ExPerformClimbAttach)
		{
			trace = CF_Trace_1(EXTrace.PLAYER, this).Add("PerformClimbAttach");

			m_ExPerformClimbAttach = false;

			trace.Info("Attaching onto " + m_ExAttachmentObject);

			if (!dBodyIsActive(m_ExAttachmentObject) && IsMissionHost())
			{
				trace.Info("is not active and is server");

				if (m_ExAttachmentObject.IsInherited(CarScript))
				{
					trace.Info("is car");

					//! Allow climbing onto inactive vehicles (this is the only way to get into the Merlin from ground)
					dBodyActive(m_ExAttachmentObject, ActiveState.ACTIVE);
				}
				else
				{
					trace.Info("is not car");

					if (!pPerformOnServer)
					{
						DayZPlayerSyncJunctures.ExpansionSendForceUnlink(this);
					}

					return;
				}
			}

			GetTransform(m_ExTransformPlayer);
			m_ExAttachmentObject.GetTransform(m_ExTransformTarget);
			Math3D.MatrixInvMultiply4(m_ExTransformTarget, m_ExTransformPlayer, m_ExTransformLocal);

			LinkToLocalSpaceOf(m_ExAttachmentObject, m_ExTransformLocal);

			m_IsAttached = true;

			m_ExAttachmentObject.Update();

			OnExpansionAttachTo(m_ExAttachmentObject, m_ExTransformLocal);

			if (pPerformOnServer || GetGame().IsServer())
			{
				DayZPlayerSyncJunctures.ExpansionSendPerformClimb(this, true, false);
			}
			else
			{
				m_ExIsAwaitingServerLink = true;
			}

			return;
		}

		if (IsPreparingVehicle())
		{
			return;
		}
		
		Object parent = m_ExAttachmentObject;

		if ((IsMissionClient() || pPerformOnServer) && m_ExPlayerLinkType == ExpansionPlayerLink.NONE && !m_ExIsAwaitingServerLink)
		{
			trace = CF_Trace_1(EXTrace.PLAYER_CONSTANT, this);

			// GetParent() and parent must match
			if (parent == GetParent())
			{
				m_ExRaycastResultType = AttachRaycastCheck(pCurrentCommandID);

				float castDistance = 1.0;
				float castRadius = 0.2;

				if (m_ExRaycastResultType == ExpansionPlayerRaycastResult.DETECT_FALL)
				{
					m_ExRaycastResultType = ExpansionPlayerRaycastResult.DETECT;

					if (m_ExAttachmentObject)
					{
						castDistance += 1.0 * m_ExAttachmentRadius;
						castRadius = 1.0;
					}
				}

				if (m_ExRaycastResultType == ExpansionPlayerRaycastResult.DETECT)
				{
#ifdef EXPANSIONMODVEHICLE
					if (m_ExAttachmentObject && m_ExAttachmentObject.IsInherited(ExpansionZodiacBoat))
						castRadius = 1;
#endif
					m_ExRaycastRVParams.radius = castRadius;
					m_ExRaycastRVParams.begPos = GetPosition() + Vector(0, 1.0, 0);
					m_ExRaycastRVParams.endPos = GetPosition() - Vector(0, castDistance, 0);
					m_ExRaycastRVParams.sorted = true;
					m_ExRaycastRVParams.type = ObjIntersectGeom;
					m_ExRaycastRVParams.flags = CollisionFlags.ALLOBJECTS;

					// Initiate the raycast
					if (DayZPhysics.RaycastRVProxy(m_ExRaycastRVParams, m_ExRaycastResults))
					{
						Object target = ExpansionAttachmentHelper.FindBestAttach(this, m_ExRaycastResults);

						if (target)
						{
							if (parent && parent != target) // Detach the player if the parent object is not the same as the target, we have to deal with attachment in next frame.
							{
								trace.Info("Current parent doesn't match target, detaching");

								Detach();
							}
							else if (!parent) // If the parent is null, verify the target and attach the player
							{
								trace.Info("Attaching to target");

								AttachTo(target);
							}
						}
						else if (parent && parent == m_ExAttachmentObject)
						{
							trace.Info("Successful raycast with parent, no target, detatching");

							Detach();
						}
					}
					else if (parent && parent == m_ExAttachmentObject) // Currently have a parent when the raycast failed, forcefully detach the player
					{
						trace.Info("Unsuccessfuly raycast with parent, detatching");
						Detach();
					}
				}
				else if (parent && m_ExRaycastResultType == ExpansionPlayerRaycastResult.DETACH)
				{
					trace.Info("State requests detach");

					Detach();
				}
			}
			else if (parent)
			{
				trace.Info("Parents don't match, detach");

				Detach();
			}

			delete trace;
		}

		m_ExPlayerLinkTypeFrameDelayed = m_ExPlayerLinkType;

		bool notRemote = GetInstanceType() != DayZPlayerInstanceType.INSTANCETYPE_REMOTE && GetInstanceType() != DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE;

		// Attachment code
		if (m_ExPlayerLinkType != ExpansionPlayerLink.NONE && notRemote)
		{
			trace = CF_Trace_1(EXTrace.PLAYER, this);
			trace.Add(m_ExPlayerLinkType, ExpansionPlayerLink);

			switch (m_ExPlayerLinkType)
			{
			case ExpansionPlayerLink.DETACH:
			{
				if (m_ExAttachmentObject == GetParent())
				{
					UnlinkFromLocalSpace();
				}

				if (m_ExAttachmentObject)
				{
					OnExpansionDetachFrom(m_ExAttachmentObject);
				}

				m_IsAttached = false;

				//m_ExAttachmentObject.Update();

				m_ExAttachmentObject = NULL;

				SetSynchDirty();
				break;
			}
			case ExpansionPlayerLink.ATTACH:
			{
				trace.Info("m_ExAttachmentObject: " + m_ExAttachmentObject);

				if (m_ExAttachmentObject.IsInherited(CarScript) && !dBodyIsActive(m_ExAttachmentObject))
				{
					//! Needed to (e.g.) allow climb attaching to Merlin without getting teleported to sea
					dBodyActive( m_ExAttachmentObject, ActiveState.ACTIVE );
				}

				GetTransform(m_ExTransformPlayer);
				m_ExAttachmentObject.GetTransform(m_ExTransformTarget);
				Math3D.MatrixInvMultiply4(m_ExTransformTarget, m_ExTransformPlayer, m_ExTransformLocal);

				LinkToLocalSpaceOf(m_ExAttachmentObject, m_ExTransformLocal);
				
				m_IsAttached = true;

				m_ExAttachmentObject.Update();

				OnExpansionAttachTo(m_ExAttachmentObject, m_ExTransformLocal);

				SetSynchDirty();
				break;
			}
			}

			// Send the user input to the server
			if (GetGame().IsClient())
			{
				trace.Info("Sending Context to Server");

				if (m_ExPlayerLinkType != ExpansionPlayerLink.NONE)
				{
					ScriptInputUserData ctx = new ScriptInputUserData;
					ctx.Write(EXPANSION_INPUT_UDT_PLAYER_LINK);
					ctx.Write(m_ExPlayerLinkType);

					if (m_ExPlayerLinkType == ExpansionPlayerLink.ATTACH)
						ctx.Write(m_ExAttachmentObject);

					ctx.Send();
				}

				trace.Info("Sent Context to Server");
			}
			else if (GetGame().IsMultiplayer())
			{
				DayZPlayerSyncJunctures.ExpansionSendNextLink(this);
			}
			else
			{
				m_ExIsAwaitingServerLink = false;
			}

			m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

			delete trace;
		}
	}

	void OnExpansionAttachTo(Object obj, vector transform[4])
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

#ifdef EXPANSIONMODVEHICLE
		CarScript car;
		if (Class.CastTo(car, obj))
		{
			car.Ex_AttachPlayer(this);
		}
#endif
	}

	void OnExpansionDetachFrom(Object obj)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, this);

#ifdef EXPANSIONMODVEHICLE
		CarScript car;
		if (Class.CastTo(car, obj))
		{
			car.Ex_DetachPlayer(this);
		}
#endif
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		//PlayerIdentity ident = GetIdentity();
		//instance.Add("Identity", ident);
		//if (ident)
		//{
		//	instance.Add("Ping Min", ident.GetPingMin());
		//	instance.Add("Ping Max", ident.GetPingMax());
		//	instance.Add("Ping Avg", ident.GetPingAvg());
		//}

		instance.Add(GetCommand_Script());

		instance.Add("Client", GetGame().IsClient());
		instance.Add("Can Store IUD", ScriptInputUserData.CanStoreInputUserData());
		instance.Add("Instance Type", GetInstanceType(), DayZPlayerInstanceType);

		instance.Add("Mission Client", IsMissionClient());
		instance.Add("Preparing Vehicle", IsPreparingVehicle());

		instance.Add("Perform Climb", m_ExPerformClimb);
		instance.Add("Perform Vehicle Get In", m_ExPerformVehicleGetIn);
		instance.Add("Preparing Vehicle", m_ExIsPreparingVehicle);
		instance.Add("Command Vehicle", GetCommand_Vehicle());
		instance.Add("Command Script", GetCommand_Script());
		instance.Add("Command Ladder", GetCommand_Ladder());
		instance.Add("Command Fall", GetCommand_Fall());
		instance.Add("Current CommandID", m_ExCurrentCommandID);
		instance.Add("Parent", GetParent());

		instance.Add("Awaiting Server Link", m_ExIsAwaitingServerLink);
		instance.Add("Raycast Result Type", m_ExRaycastResultType, ExpansionPlayerRaycastResult);
		instance.Add("Player Link Type", m_ExPlayerLinkTypeFrameDelayed, ExpansionPlayerLink);

		instance.Add(m_ExRaycastRVParams);

		instance.Add("Results", m_ExRaycastResults.Count());
		instance.Add(" BEST", ExpansionAttachmentHelper.FindBestAttach(this, m_ExRaycastResults));
		for (int i = 0; i < m_ExRaycastResults.Count(); i++)
		{
			instance.Add(" Object", m_ExRaycastResults[i].obj);
			instance.Add(" Parent", m_ExRaycastResults[i].parent);
			instance.Add(" Can Attach", ExpansionAttachmentHelper.CanAttachTo(this, m_ExRaycastResults[i].obj));
		}

		return true;
	}
#endif
};
