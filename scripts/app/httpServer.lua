local redirectResponse = require("app/RedirectResponse")
httpServer = {
    port = 80,
    address = "0.0.0.0",
    url = {

    },
    -- 配置拦截器
    -- 直接转发https请求
    interceptor = function(req)
        print("Request Host:", req.HEADER.Host)
        local realPath = "https://" .. req.HEADER.Host .. req.HEADER.Uri
        local response = redirectResponse:New(realPath)
        return response
    end
}

return httpServer