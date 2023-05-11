local userModule = require("user")

manage = {
    url = {
        user = userModule
    }
}

-- 代理访问
function manage.callUrlMethod(urlStr, req)
    local func = _G
    for w in string.gmatch(urlStr, "[%w_]+") do
        func = func[w]
    end

    if type(func) ~= "function" then
        error("Invalid function name: " .. urlStr)
    end

    print("Method:", req.METHOD)
    print("Request Host:", req.HEADER.Host)

    return func(req)
end


return manage