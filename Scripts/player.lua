function update()
    local dir = vec2()
	if GetHp() < 75 then
        print(ENT_TREE)
        print(ENT_APPLE)
		local target = GetNearest({ENT_TREE, ENT_APPLE})
		if target then
			local pos = GetPos()
			local otherPos = GetPos(target)
			if IsType(target, ENT_TREE) and (pos - otherPos):length() < GetSize() + GetSize(target) then
				SetDir(AngleTo(target))
				StartAction(ACTION_ATTACK)
			else
				dir = (otherPos - pos):norm()
			end
		end
	else
		local nearest = GetNearest(ENT_ENEMY)
		if nearest then
			local pos = GetPos()
			local otherPos = GetPos(nearest)
			dir = (otherPos - pos):norm()
			local fire = GetNearest(ENT_FIRE)
			if fire then
				local angle = AngleTo(nearest, fire)
				if (pos - otherPos):length() < GetSize() + GetSize(nearest) + 10 then
					StartAction(ACTION_THROW, nearest, angle)
				end
				if (pos - GetPos(fire)):length() < GetSize() * 2 + GetSize(fire) then
					dir = (pos - GetPos(fire)):norm()
				end
			end

			SetDir(AngleTo(nearest))
			StartAction(ACTION_ATTACK)
		end
	end
    SetVel(dir * GetMaxVel())
end
