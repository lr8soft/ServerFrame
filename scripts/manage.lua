local userModule = require("user")


manage = {
    url = {
        user = userModule
    },

    -- 调用manage内部的方法
    callUrlMethod = function(str, tbl)
        local func = _G
        for w in string.gmatch(str, "[%w_]+") do
            func = func[w]
        end
    
        if type(func) ~= "function" then
            error("Invalid function name: " .. str)
        end
    
        return func(tbl)
    end
    
}

return manage