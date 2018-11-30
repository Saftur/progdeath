-- 869.0000, 285.0000
function update()
    local enemy = GetNearest(ENT_ENEMY)
    if enemy then
        local dir = (GetPos(enemy) - GetPos())
        dir = vec2.norm(dir)
        SetVel((dir * GetMaxVel()))

    end

end

