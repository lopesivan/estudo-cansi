local frame = require("frame")

frame.init(10)

frame.register(function()
    print("Olá do frame 5")
end, 5)

frame.register(function()
    print("Olá do frame 2")
end, 2)

frame.register(function()
    print("Olá do frame 8")
end, 8)

frame.run()

