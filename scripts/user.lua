local utils = require("test")
user = {}

user.login = function(request)
    print("LOGIN!")
    result = { status = "operation_success",
               result = "login_success",
               data = {
                   id = 233333,
                   name = "lrsoft",
                   value = 1234.456
               },
               request = request
    }
    return result
end
user.regist = function(request)
    print("REGIST!")
    return { status = "operation_success", request = request }
end

user.util = utils

return user