static bool s_ExpansionPhysicsStructure_Enabled = false;

class ExpansionPhysicsStructure : ItemBase
{
	vector m_Transform[4];

	float m_PosX;
	float m_PosY;
	float m_PosZ;

	Object m_BakedMapObject;

	void ExpansionPhysicsStructure()
	{
		SetEventMask(EntityEvent.CONTACT);

		RegisterNetSyncVariableFloat("m_PosX", 0, 0, 8);
		RegisterNetSyncVariableFloat("m_PosY", 0, 0, 8);
		RegisterNetSyncVariableFloat("m_PosZ", 0, 0, 8);
	}

	void ~ExpansionPhysicsStructure()
	{
		OnDestroy();
	}

	void Create(Object bakedObject, EntityAI other, vector position, Contact data)
	{
		m_BakedMapObject = bakedObject;
		
		m_PosX = m_BakedMapObject.GetPosition()[0];
		m_PosY = m_BakedMapObject.GetPosition()[1];
		m_PosZ = m_BakedMapObject.GetPosition()[2];

		vector boundingBox[2];
		GetCollisionBox(boundingBox);
		float height = -boundingBox[0][1];

		SetPosition(m_BakedMapObject.GetPosition());
		vector ori = m_BakedMapObject.GetOrientation();
		SetOrientation(Vector(180.0 - ori[0], ori[1], ori[2]));
		GetTransform(m_Transform);

		// Call 'AdjustPosition' here only
		AdjustPosition(g_Game.SurfaceY(m_Transform[3][0], m_Transform[3][2]) + height, vector.Zero);

		SetTransform(m_Transform);
		SetAffectPathgraph(false, false);

		ExpansionCreateDynamicFromContact(other, position, data);

		OnCreate();

		if (g_Game.IsMultiplayer())
		{
			SetSynchDirty();
		}
	}

	void AdjustPosition(float yPos, vector position)
	{
		float yDiff = yPos - m_Transform[3][1];
		if (yDiff < 0)
			return;

		vector adjust = Vector(0, yDiff, 0);

		m_Transform[3] = m_Transform[3] + adjust;

		//vector dir = position * adjust;

		//Math3D.YawPitchRollMatrix();
	}

	void OnCreate()
	{
		if (!m_BakedMapObject)
			return;

		CF_ObjectManager.HideMapObject(m_BakedMapObject, true);
	}

	void OnDestroy()
	{
		if (!m_BakedMapObject)
			return;

		CF_ObjectManager.UnhideMapObject(m_BakedMapObject, true);
	}

	string ExpansionGetPhysicsType()
	{
		string type = GetType();
		type.Replace("ExpansionPhysics_", "");
		return type;
	}

	override bool Expansion_CarContactActivates()
	{
		return true;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_BakedMapObject)
			return;

		IEntity entity;

		float minDistance = 1.0;

		array<Object> objects = new array<Object>;
		vector position = Vector(m_PosX, m_PosY, m_PosZ);
		g_Game.GetObjectsAtPosition3D(position, minDistance, objects, NULL);
		for (int i = 0; i < objects.Count(); i++)
		{
			if (objects[i].GetType() == ExpansionGetPhysicsType())
			{
				float distance = vector.DistanceSq(GetPosition(), position);
				if (distance < minDistance)
				{
					minDistance = distance;
					entity = objects[i];
				}
			}
		}

		if (Class.CastTo(m_BakedMapObject, entity))
		{
			OnCreate();
		}
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		auto item = ItemBase.Cast(other);
		if (item && !dBodyIsDynamic(item) && item.Expansion_CarContactActivates() && !item.GetHierarchyParent())
		{
			item.ExpansionCreateDynamicFromContact(this, extra.Position, extra);
		}

		auto bakedObject = ExpansionBakedMapObject.Cast(other);
		if (bakedObject)
		{
			bakedObject.Create(this, extra.Position, extra);
		}

		auto staticObject = ExpansionStaticMapObject.Cast(other);
		if (staticObject)
		{
			staticObject.Create(this, extra.Position, extra);
		}
	}
};
