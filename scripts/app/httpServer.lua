local redirectResponse = require("response/RedirectResponse")
httpServer = {
    port = 80,
    address = "0.0.0.0",
    -- 配置拦截器
    -- 该拦截器实现了http转发https请求的功能
    interceptor = function(req)
        local realPath = "https://" .. req.HEADER.Host .. req.HEADER.Uri
        print("HTTP Request redirect to " .. realPath)
        local response = redirectResponse:New(realPath)
        return response
    end
}

return httpServer