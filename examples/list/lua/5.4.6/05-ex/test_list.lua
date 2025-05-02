local list = require("list")

local mylist = list.new()

list.insert(mylist, "first")
list.insert(mylist, "second")
list.insert(mylist, "third")
list.insert(mylist, "fourth")

print("Lista original:")
list.print_all(mylist)
print("Tamanho:", list.size(mylist))

print("\nRemovendo primeiro elemento...")
list.remove(mylist)
print("Lista após remoção:")
list.print_all(mylist)
print("Tamanho:", list.size(mylist))

print("\nLimpando toda a lista...")
list.clear(mylist)
print("Lista após clear:")
list.print_all(mylist)
print("Tamanho:", list.size(mylist))
