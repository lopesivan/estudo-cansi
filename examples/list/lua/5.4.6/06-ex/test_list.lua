local list = require("list")

local mylist = list.new()

list.insert(mylist, "dog")
list.insert(mylist, "cat")
list.insert(mylist, "mouse")

print("Lista atual:")
list.print_all(mylist)
print("Tamanho:", list.size(mylist))

print("\nElemento na posição 2:")
print(list.get(mylist, 2)) -- deve imprimir "cat"

print("\nElemento na posição 1:")
print(list.get(mylist, 1)) -- deve imprimir "dog"

print("\nRemovendo primeiro elemento...")
list.remove(mylist)

print("\nLista após remoção:")
list.print_all(mylist)

print("\nElemento agora na posição 1:")
print(list.get(mylist, 1)) -- agora deve imprimir "cat"
