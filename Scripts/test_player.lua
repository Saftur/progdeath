function update()
    local moveDir = {0,0}
    if KeyDown(KEY_W) then
        moveDir[2] = moveDir[2] - 10
    end
    if KeyDown(KEY_S) then
        moveDir[2] = moveDir[2] + 10
    end
    if KeyDown(KEY_A) then
        moveDir[1] = moveDir[1] - 10
    end
    if KeyDown(KEY_D) then
        moveDir[1] = moveDir[1] + 10
    end
    Move(moveDir)
end
