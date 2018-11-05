function update()
    local dir = vec2(0, 0)
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
    SetVel(dir * GetMaxVel())

	local ent = GetNearest(ENT_ENEMY)
	if ent then
		StartAction(ACTION_THROW, ent, 0)
	end
end
