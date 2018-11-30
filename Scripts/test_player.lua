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
    if (dir.x ~= 0 or dir.y ~= 0) then
        SetDir(math.atan2(-dir.y, dir.x) / math.pi * 180)
    end
	if KeyDown(KEY_T) then
		local fire = GetNearest(ENT_FIRE)
		local angle = AngleTo(ent, fire)
		StartAction(ACTION_THROW, ent, angle)
	end
    if KeyDown(KEY_E) then
        StartAction(ACTION_ATTACK)
    end
	SetVel(dir * GetMaxVel())

end
