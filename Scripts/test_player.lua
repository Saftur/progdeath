function update()
    local dir = vec2(0, 0)
		local ent = GetNearest(ENT_ENEMY)
    if KeyDown(KEY_W) then
        dir.y = dir.y - 1
    end
    if KeyDown(KEY_S) then
        dir.y = dir.y + 1
    end
    if KeyDown(KEY_A) then
        dir.x = dir.x - 1
    end
    if KeyDown(KEY_D) then
        dir.x = dir.x + 1
    end
	if KeyDown(KEY_T) then
	if ent then
		StartAction(ACTION_THROW, ent, 0)
	end
	end
    SetVel(dir * GetMaxVel())
end
