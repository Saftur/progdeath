function update()
    v = Vector2D.New({10, 12})
    print(v[1]..", "..v[2])

    nearby = GetNearby()
    for i,other in ipairs(nearby) do
        --print(tostring(k).." - "..tostring(v))
        print(tostring(other))
        local pos = GetPos()
        local otherPos = GetPos(other)
        local dir = {}
        print(pos[1]..", "..pos[2])
        print(otherPos[1]..", "..otherPos[2])
        dir[1] = (otherPos[1] - pos[1])
        dir[2] = (otherPos[2] - pos[2])
        print(dir[1]..", "..dir[2])
        local len = math.sqrt(dir[1]*dir[1] + dir[2]*dir[2])
        print(len)
        dir[1] = dir[1] / len * 10
        dir[2] = dir[2] / len * 10
        print(dir[1]..", "..dir[2])
        Move(dir)
    end
end