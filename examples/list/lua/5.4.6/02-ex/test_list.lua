-- Ajuste o path para encontrar o .so local
package.cpath = package.cpath .. ";./?.so"

-- Carrega o módulo list.so
local list = require("list")

-- Cria nova lista
local mylist = list.new()

-- Insere alguns elementos
list.insert(mylist, "hello")
list.insert(mylist, "world")
list.insert(mylist, "Lua")

-- Imprime o tamanho da lista
print("Tamanho da lista:", list.size(mylist))

-- Insere mais elementos
list.insert(mylist, "C bindings")
list.insert(mylist, "rocks!")

-- Imprime novamente o tamanho da lista
print("Tamanho atualizado:", list.size(mylist))

-- Fim do teste
print("Teste concluído.")
