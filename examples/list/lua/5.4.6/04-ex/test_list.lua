local list = require("list")

local mylist = list.new()

list.insert(mylist, "apple")
list.insert(mylist, "banana")
list.insert(mylist, "cherry")

print("Lista inicial:")
list.print_all(mylist)
print("Tamanho:", list.size(mylist))

print("\nRemovendo primeiro elemento...")
list.remove(mylist)

print("\nLista após remoção:")
list.print_all(mylist)
print("Tamanho:", list.size(mylist))
