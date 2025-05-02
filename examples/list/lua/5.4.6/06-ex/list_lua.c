#include "list.h" /* Seu list.h original */
#include <lauxlib.h>
#include <lua.h>
#include <stdlib.h>
#include <string.h>

// Obtém o elemento na posição dada (índice começando em 1)
static int l_list_get(lua_State *L)
{
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");
    int index = luaL_checkinteger(L, 2);

    if (index < 1 || index > list_size(list))
    {
        return luaL_error(L, "index out of bounds");
    }

    ListElmt *element = list_head(list);
    for (int i = 1; i < index; ++i)
    {
        element = list_next(element);
    }

    const char *str = (const char *)list_data(element);
    if (str == NULL)
    {
        lua_pushnil(L);
    }
    else
    {
        lua_pushstring(L, str);
    }

    return 1; // retorna o elemento
}

// Remove todos os elementos da lista
static int l_list_clear(lua_State *L)
{
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");

    void *data;
    while (list_size(list) > 0)
    {
        if (list_rem_next(list, NULL, &data) == 0)
        {
            free(data); // libera a string
        }
        else
        {
            return luaL_error(L, "failed to remove element");
        }
    }

    return 0; // sucesso
}

// Remove o primeiro elemento da lista
static int l_list_remove(lua_State *L)
{
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");

    if (list_size(list) == 0)
    {
        return luaL_error(L, "cannot remove from empty list");
    }

    void *data;
    if (list_rem_next(list, NULL, &data) != 0)
    {
        return luaL_error(L, "failed to remove element");
    }

    // Liberar o dado, pois fizemos strdup ao inserir
    free(data);

    return 0; // sucesso
}

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
static const luaL_Reg listlib[] = {{"new", l_list_new},
                                   {"insert", l_list_insert},
                                   {"size", l_list_size},
                                   {"print_all", l_list_print_all},
                                   {"remove", l_list_remove},
                                   {"clear", l_list_clear},
                                   {"get", l_list_get}, // <-- adiciona aqui
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
