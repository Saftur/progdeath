/**
 * @file enemylua.h
 * @author Arthur Bouvier
 * @course GAM100F18
 * Copyright © 2018 DigiPen (USA) Corporation
 */
#pragma once

const char *enemyScript = "\
function update() \n\
    local dir = vec2() \n\
    nearest = GetNearest(ENT_PLAYER) \n\
    if nearest then \n\
        local pos = GetPos() \n\
        local otherPos = GetPos(nearest) \n\
        dir = (otherPos - pos):norm() \n\
		local fire = GetNearest(ENT_FIRE) \n\
        if fire then \n\
            local angle = AngleTo(nearest, fire) \n\
            StartAction(ACTION_THROW, nearest, angle) \n\
            --[[local currPos = GetPos() \n\
            local firePos = GetPos(fire) \n\
            local vecToFire = (firePos - currPos) \n\
            local dist = vecToFire:length() \n\
            vecToFire = vecToFire:norm() \n\
            print(dist) \n\
            print(GetSize() + GetSize(fire) + 10) \n\
            if dist < GetSize() + GetSize(fire) + 10 then \n\
                dir = vec2(-vecToFire.x, vecToFire.y) \n\
            end --]] \n\
        end \n\
    end \n\
    SetVel(dir * GetMaxVel()) \n\
end \
";
