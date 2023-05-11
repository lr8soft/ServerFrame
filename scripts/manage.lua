local userModule = require("user")

manage = {
    url = {
        user = userModule
    },
    -- 调用manage内部的方法
    callUrlMethod = function(str, tbl)
        local func = manage[str]
        if func then
            return func(tbl)
        else
            error("No method named " .. str .. " in manage table")
        end
    end
    
}