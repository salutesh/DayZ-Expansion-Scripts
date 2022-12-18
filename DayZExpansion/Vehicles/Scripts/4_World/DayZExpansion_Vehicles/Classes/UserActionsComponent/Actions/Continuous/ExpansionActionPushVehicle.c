class CAContinuousExpansionPushVehicle: CAContinuousBase
{			
	override bool IsContinuousAction()
	{
		return true;
	}

	override int Execute(ActionData action_data)
	{
		return UA_PROCESSING;
	}
};

class ExpansionActionPushVehicleCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousExpansionPushVehicle();
	}

	override void InitActionComponent()
	{
		super.InitActionComponent();

		CarScript car = CarScript.Cast(m_ActionData.m_Target.GetObject());
		if (!car)
		{
			return;
		}

		car.m_Expansion_IsBeingPushed = true;
	}

	override void ProgressActionComponent()
	{
		super.ProgressActionComponent();
		
		PlayerBase player = m_ActionData.m_Player;
		CarScript car = CarScript.Cast(m_ActionData.m_Target.GetObject());
		if (!car)
		{
			return;
		}
		
		vector rayStart = GetGame().GetCurrentCameraPosition();
		vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 5.0;
		
		vector hitPosition;
		vector hitNormal;
		float hitFraction;
		Object hitObject;
			
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.VEHICLE | PhxInteractionLayers.VEHICLE_NOTERRAIN;
		bool hit = DayZPhysics.RayCastBullet(rayStart, rayEnd, collisionLayerMask, player, hitObject, hitPosition, hitNormal, hitFraction);
		if (!hit)
		{
			return;
		}
		
		vector normal = hitNormal;
		normal[1] = 0;
		float len = normal.Normalize();
		if (len < 0.25)
		{
			return;
		}
		
		vector box[2];
		car.GetCollisionBox(box);
		
		vector direction = vector.Direction(rayStart, rayEnd);
		direction.Normalize();
		
		float sideAmt = vector.Dot(direction, car.GetDirection());
		sideAmt = 1.0 - Math.AbsFloat(sideAmt);
		sideAmt *= sideAmt;
		
		float pushAmount = 20.0;
		
		vector forceAmt = vector.Dot(direction, -normal) * pushAmount * sideAmt;

		vector force = direction * forceAmt * dBodyGetMass(car);

		float mass = dBodyGetMass(car);
		float invMass = 1.0 / mass;
		
		float dt = player.GetDeltaT();
		
		Shape.CreateSphere(0xFFFF0000, ShapeFlags.ONCE|ShapeFlags.NOZBUFFER|ShapeFlags.NOOUTLINE, hitPosition, 0.1);
		Shape.Create(ShapeType.LINE, 0xFFFF0000, ShapeFlags.ONCE|ShapeFlags.NOZBUFFER|ShapeFlags.NOOUTLINE, hitPosition, hitPosition + (force * invMass));

		dBodyApplyImpulseAt(car, force * dt, hitPosition);

		SetAligning(hitPosition + (normal * forceAmt), -normal);
	}

	override void EndActionComponent()
	{
		super.EndActionComponent();

		ResetAligning();

		CarScript car = CarScript.Cast(m_ActionData.m_Target.GetObject());
		if (!car)
		{
			return;
		}
		
		car.m_Expansion_IsBeingPushed = false;
	}
};

#ifdef DIAG
[RegisterAction(ExpansionActionPushVehicle)]
#endif
class ExpansionActionPushVehicle : ActionContinuousBase
{
	void ExpansionActionPushVehicle()
	{
		m_CallbackClass = ExpansionActionPushVehicleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EAT;
		m_Text = "Push";	
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (GetGame().IsMultiplayer())
			return false;

		return CarScript.Cast(target.GetObject()) != null;
	}
};
