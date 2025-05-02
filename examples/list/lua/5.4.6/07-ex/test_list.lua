local list = require("list")

local mylist = list.new()

list.insert(mylist, "sun")
list.insert(mylist, "moon")
list.insert(mylist, "stars")

print("Elementos via iterador:")
for value in list.iter(mylist) do
    print("-", value)
end

print("Agora limpando a lista...")
list.clear(mylist)

print("Iterando lista vazia:")
for value in list.iter(mylist) do
    print("Você não deveria ver isso:", value)
end
