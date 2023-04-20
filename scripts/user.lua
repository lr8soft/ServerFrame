local JsonResponse = require("response/JsonResponse")
local HttpResponse = require("response/HttpResponse")
local strUtils = require("utils/StringUtils")
local base64 = require("jwt/Base64")
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

    strResult = strUtils.TableToStr(result)
    strEncode = base64.encode(strResult)
    print("encode:", strEncode)
    print("decode:", base64.decode(strEncode))
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