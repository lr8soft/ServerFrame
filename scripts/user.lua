local JsonResponse = require("api/JsonResponse")
local HttpResponse = require("api/HttpResponse")
user = {}

user.login = function(request)
    response = JsonResponse:New()
    result = { 
        status = "operation_success", 
        array = {"aaa", "bbb"},
        table = {
            subArr = {111, 222},
            name = "table1"
        }
    }
    response:setContent(result)
    return response
end
user.regist = function(request)
    print("REGIST!")
    response = JsonResponse:New()
    response:setContent({ status = "operation_success", request = request })
    return response
end

user.test = function(request)
    rep = HttpResponse:New()
    rep:setContent("Hello World!")
    return rep
end

return user