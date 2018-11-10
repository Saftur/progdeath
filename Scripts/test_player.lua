function update()
    local dir = vec2(0, 0)
	local ent = GetNearest(ENT_ENEMY)
	if ent then
		local pos = GetPos()
		local otherPos = GetPos(ent)
		dir = (otherPos - pos):norm()
	
		local fire = GetNearest(ENT_FIRE)
		local angle = pos:angle(GetPos(fire) - pos)
	
		StartAction(ACTION_THROW, ent, angle)

		SetVel(dir * GetMaxVel())
	end
end
