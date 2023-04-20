local jresponse = require("api/JsonResponse")
user = {}

user.login = function(request)
    print("LOGIN!")
    response = jresponse:New()
    result = { 
        status = "operation_success", 
        array = {"aaa", "bbb"},
        table = {
            subArr = {111, 222},
            name = "table1"
        },
        request = request
    }
    response:setContent(result)
    return response
end
user.regist = function(request)
    print("REGIST!")
    response = jresponse:New()
    response:setContent({ status = "operation_success", request = request })
    return response
end

return user