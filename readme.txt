--- Summary ---
Game Name:    Programmable Deathmatch
High Concept: A 2d top-down programmable action-based deathmatch.
Team Name:    Yes
Team ID:      
Team members:
 - Arthur Bouvier (a.bouvier)
 - Connor Meyers (connor.meyers)
Instructors:   Justin Chambers
Copyright 2018 DigiPen (USA) Corporation

--- Build ---
Click the build button in Visual Studio.

--- How To Play ---

Editor:

Drag & drop CodeBlocks from the left side of the screen to start writing code.
Delete CodeBlocks by dragging them back into the list. To use variables, use the
block with a blank empty box, and type the variable name.

Code run on update loop should be put in a function named "update()".

Here are a list of functions you can use:

SetVel(vel) - Set velocity to given vec2.
SetDir(dir) - Set direction angle player is looking in.

StartAction(action, ...) - Start action with given data.

AngleTo(entity OR pos)              - Get direction angle to given Entity or position.
AngleTo(ent1 OR pos1, ent2 OR pos2) - Get direction angle from given Entity/pos
                                      to other given Entity/pos.

GetNearby(type OR types)  - Get list of nearby Entities of given type or types.
GetNearest(type OR types) - Get nearest Entity of given type or types.

GetMaxVel()       - Get player's maximum velocity.
GetPos(entity)    - Get position of Entity (default: self).
GetHp(entity)     - Get hp of Entity (default: self).
GetAction(entity) - Get current action of Entity (default: self).
GetTypes(entity)  - Get list of Entity's types (default: self).
IsType(entity)    - Check if Entity has given type (default: self).
GetTypeName(type) - Get type name of given type

List of actions:

ACTION_ATTACK              - Start attacking with current weapon.
ACTION_BLOCK               - Become invincible for a certain amount of time.
ACTION_THROW (entity, dir) - Throw given Entity in given direction angle.

--- Game Controls ---

Game:
 - Ctrl-M: Menu
 - Ctrl-W (Cheat): Win instantly
 - Ctrl-L (Cheat): Lose instantly

Editor:
 - Ctrl-M: Menu
 - Ctrl-S: Save

--- Credits ---
President: Claude Comair
Instructors: Justin Chambers
Developers:
 - Arthur Bouvier (Programmer)
 - Connor Meyers (Programmer)
Third-Party Libraries:
 - Lua: Copyright 2018 Lua.org, PUC-Rio.

--- For Consideration ---
 - Flexible Engine (Processing Engine project)
 - Lua script interfacing (entluafuncs.c)
    - vec2_t library adaptation (luavec2.c)
 - CodeBlock snapping system (CodeBlock filter)

