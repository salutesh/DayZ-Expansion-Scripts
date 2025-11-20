/**
 * ExpansionPlayerPreview.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
class ExpansionPlayerPreview
{
	protected static const float POSITION_PLAYER_PREVIEW_W_THRESHOLD_MIN = 1.5;
	protected static const float POSITION_PLAYER_PREVIEW_W_THRESHOLD_MAX = 4;
	protected static const float POSITION_PLAYER_PREVIEW_Y_OFFSET = -0.125;
	protected static const float POSITION_PLAYER_PREVIEW_ANIMATION_DURATION = 0.5;

	protected ExpansionScriptViewBase m_ParentView;
	protected PlayerBase m_PlayerPreview;
	protected PlayerPreviewWidget m_PreviewWidget;

	protected vector m_CharacterOrientation;
	protected int m_CharacterRotationX;
	protected int m_CharacterRotationY;
	protected int m_CharacterScaleDelta;

	protected int m_SelectedItemSlot;
	protected float m_LastPositionW;
	protected float m_LastPositionY;
	protected int m_PlayerPosCurrentAnimID;
	protected bool m_PlayerPosAnimRunning;

	void ExpansionPlayerPreview(ExpansionScriptViewBase scriptView, PlayerPreviewWidget previewWidget)
	{
		m_ParentView = scriptView;
		m_PreviewWidget = previewWidget;
	}

	void ~ExpansionPlayerPreview()
	{
		if (m_PlayerPreview)
			g_Game.ObjectDelete(m_PlayerPreview);
	}

	void Update(string previewClassName)
	{
		Man player = g_Game.GetPlayer();
		
		if (!m_PlayerPreview)
		{
			//! Oh fml... of all the things I tried to get the player preview face texture to render,
			//! it turns out the preview entity needs to be close to the player's viewport. Go figure.
			//! Just spawn it behind the player camera so it won't be seen under normal circumstances.
			vector dir = g_Game.GetCurrentCameraDirection();
			dir.Normalize();
			vector pos = g_Game.GetCurrentCameraPosition() - dir * 0.5;
			pos[1] = player.GetPosition()[1];
			m_PlayerPreview = PlayerBase.Cast(g_Game.CreateObjectEx(player.GetType(), pos, ECE_LOCAL|ECE_NOLIFETIME));
			m_PlayerPreview.SetOrientation(player.GetOrientation());

			m_ParentView.UpdatePlayerPreviewObject(m_PlayerPreview);
		}
		else
		{
			//! Remove existing attachments on player preview
			array<ItemBase> previewAttachments = new array<ItemBase>;
			for (int i = 0; i < m_PlayerPreview.GetInventory().AttachmentCount(); i++)
			{
				ItemBase previewItem = ItemBase.Cast(m_PlayerPreview.GetInventory().GetAttachmentFromIndex(i));
				//! Removing the attachment here would change attachment count, so add it to a temp array for later removal
				if (previewItem)
					previewAttachments.Insert(previewItem);
			}
			foreach (ItemBase previewAttachment: previewAttachments)
			{
				m_PlayerPreview.GetInventory().LocalDestroyEntity(previewAttachment);
			}
		}

		//! Add selected item first to override any current player attachment on same slot
		EntityAI item = m_PlayerPreview.GetInventory().CreateAttachment(previewClassName);
		m_ParentView.SpawnPlayerPreviewAttachments(item);

		m_ParentView.SetCurrentPreviewObject(item);
		
		//! Get slot (for dynamic preview focus)
		m_SelectedItemSlot = -1;
		if (item)
		{
			InventoryLocation loc = new InventoryLocation;
			item.GetInventory().GetCurrentInventoryLocation(loc);
			if (loc)
				m_SelectedItemSlot = loc.GetSlot();
		}

		//! Add player's current attachments to player preview
		CreateAttachments(player, m_PlayerPreview);

		if (!m_LastPositionW)
			UpdateLastPosition(1.0);
		else
			m_LastPositionY = m_PreviewWidget.GetModelPosition()[1];

		AnimatePosition();
	}

	//! Create all attachments from src in dst
	void CreateAttachments(EntityAI src, EntityAI dst)
	{
		if (!src.GetInventory())
			return;

		for (int i = 0; i < src.GetInventory().AttachmentCount(); i++)
		{
			ItemBase srcAttachment = ItemBase.Cast(src.GetInventory().GetAttachmentFromIndex(i));
			if (srcAttachment)
			{
				//! Create attachment
				EntityAI dstAttachment;
				if (dst.IsInherited(Weapon_Base))
					dstAttachment = Weapon_Base.Cast(dst).ExpansionCreateInInventory(srcAttachment.GetType());
				else
					dstAttachment = dst.GetInventory().CreateAttachment(srcAttachment.GetType());
				if (dstAttachment)
					CreateAttachments(srcAttachment, dstAttachment);  //! Create attachments of attachment
			}
		}
	}

	void UpdateScale(int mouse_x, int mouse_y, bool is_dragging, int wheel)
	{
		float w, h, x, y;		
		m_PreviewWidget.GetPos(x, y);
		m_PreviewWidget.GetSize(w,h);
		w = w + (m_CharacterScaleDelta / 8);
		h = h + (m_CharacterScaleDelta / 8);
		if ( w > 0.5 && w < 6 )
		{
			m_PreviewWidget.SetSize(w, h);
	
			//! Align to center
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - (m_CharacterScaleDelta / 16);
			float new_y = y - (m_CharacterScaleDelta / 16);
			m_PreviewWidget.SetPos(new_x, new_y);

			UpdateLastPosition(w);
			if (!m_PlayerPosAnimRunning)
				UpdatePosition(m_LastPositionY);
		}
	}

	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_CharacterOrientation;
		o[2] = o[2] + (m_CharacterRotationY - mouse_y);
		o[1] = o[1] - (m_CharacterRotationX - mouse_x);
		
		m_PreviewWidget.SetModelOrientation(o);
		
		if (!is_dragging)
		{
			m_CharacterOrientation = o;
		}
	}

	float GetPositionTargetY()
	{
		//! Keep focus on item slot when zooming in
		bool isVestSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Vest");
		bool isBodySlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Body");
		bool isBackSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Back");
		bool isHipsSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Hips");
		bool isLegsSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Legs");
		bool isFeetSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Feet");
		bool isHeadgearSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Headgear");
		bool isMaskSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Mask");
		bool isEyewearSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Eyewear");
		bool isArmbandSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Armband");
		bool isGlovesSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Gloves");
		bool isTopSlot;
		//! https://feedback.bistudio.com/T173348
		if (isVestSlot || isBodySlot || isBackSlot || isArmbandSlot)
			isTopSlot = true;
		bool isHeadSlot;
		if (isHeadgearSlot || isMaskSlot || isEyewearSlot)
			isHeadSlot = true;

		float v = POSITION_PLAYER_PREVIEW_Y_OFFSET;
		if (isTopSlot)
			v = -0.4375;
		else if (isHeadSlot)
			v = -0.75;
		else if (isLegsSlot)
			v = 0.275;
		else if (isFeetSlot)
			v = 0.625;

		return v;
	}
	
	float GetPositionInterpolatedY(float w, float targetY)
	{
		//! Normalize to 0..1
		float normalized = ExpansionMath.LinearConversion(POSITION_PLAYER_PREVIEW_W_THRESHOLD_MIN, POSITION_PLAYER_PREVIEW_W_THRESHOLD_MAX, w);
		//! Smooth normalized value
		float smooth = ExpansionMath.SmoothStep(normalized, 2);
		//! Interpolate to targetY
		float y = ExpansionMath.LinearConversion(0, 1, smooth, POSITION_PLAYER_PREVIEW_Y_OFFSET, targetY);

		return y;
	}

	void UpdatePosition(float y)
	{
		m_PreviewWidget.SetModelPosition(Vector(0, y, 0));
	}

	void UpdateLastPosition(float w)
	{
		m_LastPositionW = w;
		if (!m_PlayerPosAnimRunning)
			m_LastPositionY = GetPositionInterpolatedY(w, GetPositionTargetY());
	}

	int AnimatePosition(int animID = 0, int frameNumber = 1)
	{
		if (!animID)
		{
			m_PlayerPosAnimRunning = true;
			m_PlayerPosCurrentAnimID++;
			animID = m_PlayerPosCurrentAnimID;
		}
		else if (m_PlayerPosCurrentAnimID != animID)
			return 0;

		float targetInterpolatedY = GetPositionInterpolatedY(m_LastPositionW, GetPositionTargetY());

		int fps = 60;
		float frames = fps * POSITION_PLAYER_PREVIEW_ANIMATION_DURATION;
		float step = (targetInterpolatedY - m_LastPositionY) / frames * frameNumber;
		float currentY = m_LastPositionY + step;

		float smooth;

		if (targetInterpolatedY != m_LastPositionY)  //! Prevent division by zero
			smooth = ExpansionMath.SmoothStep(currentY, 2, m_LastPositionY, targetInterpolatedY);
		else
			smooth = currentY;

		UpdatePosition(smooth);

		if (currentY != targetInterpolatedY && frameNumber < frames)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(AnimatePosition, Math.Round(1000 / fps), false, animID, frameNumber + 1);
		}
		else
		{
			m_PlayerPosAnimRunning = false;
			m_LastPositionY = currentY;
		}

		return animID;
	}

	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		g_Game.GetDragQueue().Call(this, "UpdateRotation");
		GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
		return true;
	}

	bool OnMouseWheel(Widget w, int x, int y, int wheel) 
	{
		g_Game.GetDragQueue().Call(this, "UpdateScale");
		m_CharacterScaleDelta = wheel;
		return false;
	}

	bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		m_CharacterOrientation = vector.Zero;
		
		m_PreviewWidget.SetModelPosition(Vector(0,0,0.5));
		m_PreviewWidget.SetModelOrientation(m_CharacterOrientation);

		m_PreviewWidget.SetSize(1.5, 1.5);

		//! Align to center 
		m_PreviewWidget.SetPos(-0.225, -0.225);
		
		return false;
	}
}
#endif
