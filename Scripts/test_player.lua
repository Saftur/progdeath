function update()
    local moveDir = vec2(0,0)
    if KeyDown(KEY_W) then
        moveDir.y = moveDir.y - 10
    end
    if KeyDown(KEY_S) then
        moveDir.y = moveDir.y + 10
    end
    if KeyDown(KEY_A) then
        moveDir.x = moveDir.x - 10
    end
    if KeyDown(KEY_D) then
        moveDir.x = moveDir.x + 10
    end
    Move(moveDir)
end
