local list = require("list")

local mylist = list.new()

list.insert(mylist, "hello")
list.insert(mylist, "world")
list.insert(mylist, "Lua")
list.insert(mylist, "bindings")

print("Elementos da lista:")
list.print_all(mylist)

print("Tamanho da lista:", list.size(mylist))
