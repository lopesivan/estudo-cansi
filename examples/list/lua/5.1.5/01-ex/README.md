- **Escrever bindings C para Lua**, usando a abordagem **(A)**: funções simples que operam sobre uma `List *` passada como *userdata*.
- Deixar a lista como um *tipo leve* (sem OO/metatables complexas).

---

### **Plano de ação**

- [x] Criar um novo arquivo `list_lua.c`.
- [x] No Lua, vamos fazer:
  ```lua
  local list = require("list")
  local mylist = list.new()
  list.insert(mylist, "hello")
  list.insert(mylist, "world")
  print(list.size(mylist)) -- 2
  ```
- [x] Usar o `list.h` e `list.c` originais como base, sem reescrever a lógica de lista.
- [x] O `List` será encapsulado como `userdata` normal.
- [x] Não vamos mexer ainda em iteração Lua (for), apenas operações básicas.
- [x] Gerar uma biblioteca compartilhada `.so` para carregar no Lua.

---

### **Vamos começar? Primeiro rascunho:**

**Arquivo:** `list_lua.c`

```c
#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include "list.h"  // Seu list.h original

// Função de destruição da lista
static int l_list_gc(lua_State *L) {
    List *list = (List *)lua_touserdata(L, 1);
    if (list != NULL) {
        list_destroy(list);
    }
    return 0;
}

// Cria uma nova lista
static int l_list_new(lua_State *L) {
    List *list = (List *)lua_newuserdata(L, sizeof(List));
    list_init(list, NULL); // sem função de comparação, por enquanto
    luaL_getmetatable(L, "ListMeta");
    lua_setmetatable(L, -2);
    return 1; // retorna o userdata
}

// Insere um elemento na lista
static int l_list_insert(lua_State *L) {
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");
    const char *str = luaL_checkstring(L, 2);

    // Duplicar o conteúdo
    char *copy = strdup(str);
    if (!copy) {
        return luaL_error(L, "out of memory");
    }

    if (list_ins_next(list, NULL, copy) != 0) {
        free(copy);
        return luaL_error(L, "failed to insert");
    }

    return 0; // nada a retornar
}

// Retorna o tamanho da lista
static int l_list_size(lua_State *L) {
    List *list = (List *)luaL_checkudata(L, 1, "ListMeta");
    lua_pushinteger(L, list_size(list));
    return 1;
}

// Define as funções disponíveis
static const luaL_Reg listlib[] = {
    {"new", l_list_new},
    {"insert", l_list_insert},
    {"size", l_list_size},
    {NULL, NULL}
};

// Função chamada ao dar require
int luaopen_list(lua_State *L) {
    // Cria uma metatable para o tipo
    luaL_newmetatable(L, "ListMeta");

    // Define __gc (garbage collector)
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, l_list_gc);
    lua_settable(L, -3);

    luaL_newlib(L, listlib);
    return 1;
}
```

---

### **Compilação**

Supondo que você esteja em Linux:

```bash
gcc -Wall -fPIC -shared -o list.so list_lua.c list.c -I/usr/include/lua5.4
```

(Ajuste o `-I` conforme a versão do seu Lua.)

Depois no Lua:

```lua
local list = require("list")

local mylist = list.new()
list.insert(mylist, "foo")
list.insert(mylist, "bar")
print(list.size(mylist))  -- deve imprimir 2
```

---

### **Notas técnicas**
- A lista guarda cópias (*strdup*) dos dados de entrada.
- O `__gc` garante que a lista será destruída corretamente quando o garbage collector do Lua liberar o objeto.
- Estamos expondo funções *de forma simples* (sem objetos Lua de alto nível).

---

### **Próximos passos sugeridos (se quiser evoluir depois)**
- Permitir remoção de elementos.
- Iteração Lua-friendly (`for i, v in list:elements()`).
- Deixar opcional se os dados precisam ser `strdup` ou não (ex.: valores mais complexos).
- Adicionar funções de busca (`find`, etc.).

---
