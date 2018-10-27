/**
 * @file luautil.c
 * @author Arthur Bouvier (a.bouvier)
 * @date 10/26/18
 * Lua Utilities implementation
 */
#include "luautil.h"

/**
 * @brief Print a Lua error
 * @param L   Lua state
 * @param fmt Error format string
 * @param ... Things to print
 */
void luaU_error(lua_State *L, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

/**
 * @brief Call a Lua function
 * @param L    Lua state
 * @param func Lua function name
 * @param sig  Function signature.
 *             ie. "dd>d" means takes two doubles, returns a double.
 *             Specifiers are d->double, i->int, and s->string
 * @param ...  Arguments to pass in, and pointers to store results in
 */
void luaU_call(lua_State *L, const char *func, const char *sig, ...) {
    va_list vl;
    int narg, nres;  /* number of arguments and results */
  
    va_start(vl, sig);
    lua_getglobal(L, func);  /* get function */
  
    /* push arguments */
    narg = 0;
    while (*sig) {  /* push arguments */
        switch (*sig++) {
        case 'd':  /* double argument */
            lua_pushnumber(L, va_arg(vl, double));
            break;
  
        case 'i':  /* int argument */
            lua_pushnumber(L, va_arg(vl, int));
            break;
  
        case 's':  /* string argument */
            lua_pushstring(L, va_arg(vl, char *));
            break;
  
        case '>':
            goto endwhile;
  
        default:
            luaU_error(L, "invalid option (%c)", *(sig - 1));
        }
        narg++;
        luaL_checkstack(L, 1, "too many arguments");
    } endwhile:
  
    /* do the call */
    nres = strlen(sig);  /* number of expected results */
    if (lua_pcall(L, narg, nres, 0) != 0)  /* do the call */
        luaU_error(L, "error running function `%s': %s", func, lua_tostring(L, -1));
  
    /* retrieve results */
    nres = -nres;  /* stack index of first result */
    while (*sig) {  /* get results */
        switch (*sig++) {
  
        case 'd':  /* double result */
            if (!lua_isnumber(L, nres))
                luaU_error(L, "wrong result type");
            *va_arg(vl, double *) = lua_tonumber(L, nres);
            break;
  
        case 'i':  /* int result */
            if (!lua_isnumber(L, nres))
                luaU_error(L, "wrong result type");
            *va_arg(vl, int *) = (int)lua_tonumber(L, nres);
            break;
  
        case 's':  /* string result */
            if (!lua_isstring(L, nres))
                luaU_error(L, "wrong result type");
            *va_arg(vl, const char **) = lua_tostring(L, nres);
            break;
  
        default:
            luaU_error(L, "invalid option (%c)", *(sig - 1));
        }
        nres++;
    }
    va_end(vl);
}
