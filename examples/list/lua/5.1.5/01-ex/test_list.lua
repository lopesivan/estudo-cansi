-- lua_list.lua

local list = require("list")

local mylist = list.new()
list.insert(mylist, "hello")
list.insert(mylist, "world")
list.insert(mylist, "Miguel")
list.insert(mylist, "Snow")

print(list.size(mylist)) -- 4
