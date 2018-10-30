/**
 * @file enemylua.h
 * @author Arthur Bouvier
 * @date 
 * 
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
    end \n\
    SetVel(dir * GetMaxVel()) \n\
end \
";
