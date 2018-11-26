function update()
    local dir = vec2(0, 0)
	local ent = GetNearest(ENT_ENEMY)
	if KeyDown(KEY_W) then
		dir.y = -1
	end
	if KeyDown(KEY_S) then
		dir.y = 1
	end
	if KeyDown(KEY_A) then
		dir.x = -1
	end
	if KeyDown(KEY_D) then
		dir.x = 1
	end
	if KeyDown(KEY_T) then
		local fire = GetNearest(ENT_FIRE)
		local angle = vec2(1,0):angle(GetPos(fire) - GetPos())
		StartAction(ACTION_THROW, ent, angle)
	end
	SetVel(dir * GetMaxVel())
	--[[if ent then
		local pos = GetPos()
		local otherPos = GetPos(ent)
		dir = (otherPos - pos):norm()
	
		local fire = GetNearest(ENT_FIRE)
		local angle = pos:angle(GetPos(fire) - pos)
	
		StartAction(ACTION_THROW, ent, angle)

		SetVel(dir * GetMaxVel())
	end--]]
end
