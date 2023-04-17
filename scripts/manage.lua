url = {
    login = function(request)
        print("LOGIN!")
        result = { status = "operation_success",
                   result = "login_success",
                   data = {
                       id = 233333,
                       name = "lrsoft",
                       value = 1234.456
                   }
        }
        return result
    end,
    regist = function(request)
        print("REGIST!")
        return { status = "operation_success" }
    end
}