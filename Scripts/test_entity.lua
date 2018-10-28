function update()
    nearest = GetNearest(ET_PLAYER)
    if nearest then
        local pos = GetPos()
        local otherPos = GetPos(nearest)
        local dir = (otherPos - pos):norm() * 5
        Move(dir)
    end
end
