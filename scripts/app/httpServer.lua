
httpServer = {
    port = 80,
    address = "0.0.0.0",
    url = {

    },
    -- 配置拦截器
    interceptor = function(req)
        print("interceptor")
        print("Method:", req.METHOD)
        print("Request Host:", req.HEADER.Host)
        print("Request Path:", req.PATH)
        print("Request Query:", req.QUERY)
        print("Request Body:", req.BODY)
        print("Request Header:", req.HEADER)
        print("Request Cookie:", req.COOKIE)
    end
}

return httpServer