function update()
    local dir = vec2()
    nearest = GetNearest(ENT_PLAYER)
    if nearest then
        local pos = GetPos()
        local otherPos = GetPos(nearest)
        dir = (otherPos - pos):norm()
    end
    SetVel(dir * GetMaxVel())
end
