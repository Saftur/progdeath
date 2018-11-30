function update()
    local dir = vec2()
    nearest = GetNearest(ENT_PLAYER)
    if nearest then
        local pos = GetPos()
        local otherPos = GetPos(nearest)
        dir = (otherPos - pos):norm()
		local fire = GetNearest(ENT_FIRE)
        if fire then
            local angle = AngleTo(nearest, fire)
            StartAction(ACTION_THROW, nearest, angle)
        end
        local currPos = GetPos()
        local firePos = GetPos(fire)
        local vecToFire = (firePos - currPos)
        local dist = vecToFire:length()
        vecToFire = vecToFire:norm()
        if dist < GetSize() + GetSize(fire) + 10 then
            local newDir1 = vec2(-vecToFire.x, vecToFire.y)
            local newDir2 = vec2(vecToFire.x, -vecToFire.y)
            if (newDir1 - dir):length() < (newDir2 - dir):length() then
                dir = newDir1
            else
                dir = newDir2
            end
        end
    end
    SetVel(dir * GetMaxVel())
end
