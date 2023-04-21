local JsonResponse = require("response/JsonResponse")
local HttpResponse = require("response/HttpResponse")
local token = require("jwt/token")
user = {}

user.login = function(request)
    response = JsonResponse:New()
    result = { 
        status = "operation_success"
    }

    strResult = token.GetToken(result)
    print("jwt_token:", strResult)
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