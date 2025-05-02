#include "list.h" /* Seu list.h original */
#include <lauxlib.h>
#include <lua.h>
#include <stdlib.h>
#include <string.h>

// Imprime todos os elementos da lista
static int l_list_print_all(lua_State *L)
{
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");
    ListElmt *element = list_head(list);
    int index = 1;

    while (element != NULL)
    {
        const char *str = (const char *)list_data(element);
        printf("[%d] %s\n", index, str ? str : "(null)");
        element = list_next(element);
        index++;
    }

    return 0; // não retorna nada para Lua
}

/* Função de destruição da lista */
static int l_list_gc(lua_State *L)
{
    List *list = (List *)lua_touserdata(L, 1);
    if (list != NULL)
    {
        list_destroy(list);
    }
    return 0;
}

/* Cria uma nova lista */
static int l_list_new(lua_State *L)
{
    List *list = (List *)lua_newuserdata(L, sizeof(List));
    list_init(list, NULL); /* sem função de comparação, por enquanto */
    luaL_getmetatable(L, "ListMeta");
    lua_setmetatable(L, -2);
    return 1; /* retorna o userdata */
}

/* Insere um elemento na lista */
static int l_list_insert(lua_State *L)
{
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");
    const char *str = luaL_checkstring(L, 2);

    /* Duplicar o conteúdo */
    char *copy = strdup(str);
    if (!copy)
    {
        return luaL_error(L, "out of memory");
    }

    if (list_ins_next(list, NULL, copy) != 0)
    {
        free(copy);
        return luaL_error(L, "failed to insert");
    }

    return 0; /* nada a retornar */
}

/* Retorna o tamanho da lista */
static int l_list_size(lua_State *L)
{
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");
    lua_pushinteger(L, list_size(list));
    return 1;
}

/* Define as funções disponíveis */
static const luaL_Reg listlib[] = {
    {"new", l_list_new},
    {"insert", l_list_insert},
    {"size", l_list_size},
    {"print_all", l_list_print_all}, // <-- adiciona aqui
    {NULL, NULL}};

/* Função chamada ao dar require */
int luaopen_list(lua_State *L)
{
    /* Cria uma metatable para o tipo */
    luaL_newmetatable(L, "ListMeta");

    /* Define __gc (garbage collector) */
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, l_list_gc);
    lua_settable(L, -3);

    luaL_newlib(L, listlib);
    return 1;
}
