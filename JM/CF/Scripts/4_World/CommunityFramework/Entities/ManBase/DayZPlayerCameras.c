
modded class DayZPlayerCameras
{
	static override void RegisterCameras( DayZPlayerType pType )
	{
		JMAnimRegister.Register( pType );

		pType.ResetCameraCreators();

		//! 
		//! this registers camera ids for camera script class implementation
		//! which camera is used at the time is specified by DayzPlayerImplement.CamraHandler which returns id 
		//!
		pType.RegisterCameraCreator(DAYZCAMERA_1ST, DayZPlayerCamera1stPerson);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC, DayZPlayerCamera3rdPersonErc);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_SPR, DayZPlayerCamera3rdPersonErcSpr);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_RAISED, DayZPlayerCamera3rdPersonErcRaised);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_CRO, DayZPlayerCamera3rdPersonCrouch);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_CRO_RAISED, DayZPlayerCamera3rdPersonCrouchRaised);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_PRO, DayZPlayerCamera3rdPersonProne);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_PRO_RAISED, DayZPlayerCamera3rdPersonProneRaised);
		pType.RegisterCameraCreator(DAYZCAMERA_IRONSIGHTS, DayZPlayerCameraIronsights);
		pType.RegisterCameraCreator(DAYZCAMERA_OPTICS, DayZPlayerCameraOptics);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_RAISED_MELEE, DayZPlayerCamera3rdPersonErcRaisedMelee);
		pType.RegisterCameraCreator(DAYZCAMERA_1ST_UNCONSCIOUS, DayZPlayerCamera1stPersonUnconscious);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_JUMP, DayZPlayerCamera3rdPersonJump);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_CLIMB, DayZPlayerCamera3rdPersonClimb);
		
		pType.RegisterCameraCreator(DAYZCAMERA_1ST_VEHICLE, DayZPlayerCamera1stPersonVehicle);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_VEHICLE, DayZPlayerCamera3rdPersonVehicle);

		
		//! Blend times when changing cameras
		//! 3rd person erected camera transition
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_CRO, 0.4, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_PRO, 0.8, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_PRO_RAISED, 0.8, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_ERC_RAISED, 0.2, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_ERC_RAISED_MELEE, 0.2, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_ERC_SPR, 0.6, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_3RD_ERC_SPR, 0.7, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_3RD_PRO_RAISED, 0.8, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_ERC, 0.3, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_CRO, 0.3, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_PRO, 0.5, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_ERC_RAISED, 0.3, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_SPR, DAYZCAMERA_3RD_JUMP, 0.1, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_JUMP, DAYZCAMERA_3RD_ERC_SPR, 0.2, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_JUMP, 0.05, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_JUMP, DAYZCAMERA_3RD_ERC, 0.3, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_3RD_CLIMB, 0.3, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_CLIMB, DAYZCAMERA_3RD_ERC, 0.3, false);

		//! 3rd person crouch camera transitions
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_ERC, 0.4, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_PRO, 0.6, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_CRO_RAISED, 0.2, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_3RD_ERC_SPR, 0.6, false);
		
		//! 3rd person prone camera transitions
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_ERC, 0.9, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_CRO, 0.7, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_PRO_RAISED, 0.2, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_3RD_ERC_SPR, 0.6, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_CRO, 0.7, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_CRO_RAISED, 0.7, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_ERC, 0.9, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_3RD_ERC_RAISED, 0.9, false);
		
		//! vehicle cameras
		RegisterTransitionTime(DAYZCAMERA_3RD_VEHICLE, DAYZCAMERA_3RD_ERC, 0.7, false);

		//! ironsights camera transitions
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO_RAISED, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
		RegisterTransitionTime(DAYZCAMERA_1ST, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_01, true);
		
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_IRONSIGHTS, TIME_CAMERACHANGE_02, true);
		
		//! optics camera transitions
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_03, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO_RAISED, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_03, true);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_03, true);
		
		RegisterTransitionTime(DAYZCAMERA_1ST, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true); 	//handheld optics only
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);	//handheld optics only
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);	//handheld optics only
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO, DAYZCAMERA_OPTICS, TIME_CAMERACHANGE_02, true);	//handheld optics only
		
		//Uncon camera transitions
		RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_3RD_PRO, 1.0, false);
		RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_1ST, 0.1, false);
		RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_1ST_VEHICLE, 0.5, false);
		RegisterTransitionTime(DAYZCAMERA_1ST_UNCONSCIOUS, DAYZCAMERA_3RD_VEHICLE, 1.0, false);
		
		//! register function to handle camera changes 
		pType.RegisterCameraOnChangeFnStatic(DayZPlayerCameras, "OnCameraChange");
	};
};