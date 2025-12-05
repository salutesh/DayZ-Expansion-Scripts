/**
 * ExpansionHumanCommandScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandScript : HumanCommandScript
{
	DayZPlayerImplement m_Player;
	ExpansionHumanST m_Table;
	HumanInputController m_Input;

	vector m_Transform[4];

	autoptr array<Shape> m_DebugShapes = new array<Shape>();

	private bool m_Look = true;
	private float m_LookLR;
	private float m_LookUD;

	private int m_CurrentStance = DayZPlayerConstants.STANCEIDX_ERECT;
	private int m_CurrentMovement = DayZPlayerConstants.MOVEMENT_IDLE;
	private float m_CurrentLeaning;

	void ExpansionHumanCommandScript(DayZPlayerImplement player, ExpansionHumanST table)
	{
		m_Player = player;
		m_Table = table;

		m_Input = player.GetInputController();
	}

	void ~ExpansionHumanCommandScript()
	{
		ClearDebugShapes();
	}

	void SetLookDirection(vector pDirection)
	{
		vector angles = pDirection.VectorToAngles();
		SetLookAnglesRel(ExpansionMath.RelAngle(angles[0]), ExpansionMath.RelAngle(angles[1]));
	}

	void SetLookAnglesRel(float lookLR, float lookUD)
	{
		m_LookLR = lookLR;
		m_LookUD = lookUD;

		//! https://feedback.bistudio.com/T173348
		if (Math.AbsFloat(m_LookLR) > 0.01 || Math.AbsFloat(m_LookUD) > 0.01)
			m_Look = true;
		else
			m_Look = false;
	}

	void SetAimAnglesRel(float aimLR, float aimUD) { }

	//! Helper functions
	void AnglesToQuat(vector angles, out float[] quat)
	{
		vector rotationMatrix[3];
		Math3D.YawPitchRollMatrix(angles, rotationMatrix);
		Math3D.MatrixToQuat(rotationMatrix, quat);
	}
	
	void PrePhys_SetAngles(vector angles)
	{
		float rotation[4];
		AnglesToQuat(angles, rotation);
		PrePhys_SetRotation(rotation);
	}

	void ClearDebugShapes()
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for (int dbg = 0; dbg < m_DebugShapes.Count(); ++dbg)
			m_DebugShapes[dbg].Destroy();

		m_DebugShapes.Clear();
#endif
	}

	void SetCurrentStance(int stance)
	{
		m_CurrentStance = stance;
	}

	override int GetCurrentStance()
	{
		return m_CurrentStance;
	}

	void SetCurrentMovement(int movement)
	{
		m_CurrentMovement = movement;
	}

	override int GetCurrentMovement()
	{
		return m_CurrentMovement;
	}

	void SetCurrentLeaning(float leaning)
	{
		m_CurrentLeaning = leaning;
	}

	override float GetCurrentLeaning()
	{
		return m_CurrentLeaning;
	}

	override void PreAnimUpdate(float pDt)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		ClearDebugShapes();
#endif

		m_Player.GetTransform(m_Transform);

		m_Table.SetLook(this, m_Look);

		switch (m_Player.GetInstanceType())
		{
			case DayZPlayerInstanceType.INSTANCETYPE_AI_SERVER:
			case DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE:
			case DayZPlayerInstanceType.INSTANCETYPE_AI_SINGLEPLAYER:
				m_Table.SetLookDirX(this, m_LookLR);
				m_Table.SetLookDirY(this, m_LookUD);
				break;

			default:
				HumanCommandWeapons hcw = m_Player.GetCommandModifier_Weapons();
				m_Table.SetLookDirX(this, hcw.GetBaseAimingAngleLR());
				m_Table.SetLookDirY(this, hcw.GetBaseAimingAngleUD());
				break;
		}
	}

	override void OnActivate()
	{
		ClearDebugShapes();
	}

	override void OnDeactivate()
	{
		ClearDebugShapes();
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		return true;
	}
#endif

	void DBGDrawLineMS(vector start, vector end, int color = 0x44FFFFFF)
	{
		vector pts[2];
		pts[0] = DBGFixDebugPosition(start);
		pts[1] = DBGFixDebugPosition(end);

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		DBGAddShape(Shape.CreateLines(color, ShapeFlags.TRANSP | ShapeFlags.NOZBUFFER, pts, 2));
#endif
	}

	void DBGDrawLine(vector start, vector end, int color = 0x44FFFFFF)
	{
		DBGDrawLineMS(start.InvMultiply4(m_Transform), end.InvMultiply4(m_Transform), color);
	}

	void DBGDrawSphereMS(vector position, float size, int color = 0x44FFFFFF)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		position = DBGFixDebugPosition(position);

		DBGAddShape(Shape.CreateSphere(color, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, position, size));
#endif
	}

	void DBGDrawSphere(vector position, float size, int color = 0x44FFFFFF)
	{
		DBGDrawSphereMS(position.InvMultiply4(m_Transform), size, color);
	}

	void DBGDrawLineDirectionMS(vector direction, int color = 0x44FFFFFF)
	{
		DBGDrawLineMS("0 0 0", direction, color);
	}

	void DBGDrawLineDirectionMS(vector start, vector direction, int color = 0x44FFFFFF)
	{
		DBGDrawLineMS(start, start + direction, color);
	}

	void DBGDrawLineDirection(vector start, vector direction, int color = 0x44FFFFFF)
	{
		DBGDrawLineDirectionMS(start.InvMultiply4(m_Transform), direction.InvMultiply3(m_Transform), color);
	}

	void DBGAddShape(Shape shape)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		m_DebugShapes.Insert(shape);
#endif
	}

	vector DBGFixDebugPosition(inout vector position)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		position = position.Multiply4(m_Transform);
#endif

		return position;
	}
};