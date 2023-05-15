local httpApp = require("app/httpServer")
local httpsApp = require("app/httpsServer")

manage = {
    -- listener入口
    app = {
        httpServerDemo = httpApp,
        httpsServerDemo = httpsApp
    }
}

-- 代理访问
function manage.callUrlMethod(appStr, urlStr, req)
    local fullFunc = appStr .. ".url." .. urlStr
    local func = _G
    for w in string.gmatch(fullFunc, "[%w_]+") do
        func = func[w]
    end

    if type(func) ~= "function" then
        error("Invalid function name: " .. fullFunc)
    end

    -- 调用拦截器
    local interceptor = _G[appStr].interceptor
    local interceptorResponse = nil
    if interceptor ~= nil then
        interceptorResponse = interceptor(req)
    end

    -- 拦截器返回值不为空则直接返回
    if interceptorResponse ~= nil then
        return interceptorResponse
    end

    -- 没拦截的话就调用方法
    return func(req)
end


return manage