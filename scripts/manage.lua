
url = {
    login = function(request)
        print("LOGIN!")
        return { status = "operation_success", result = "login_success" }
    end,
    regist = function(request)
        print("REGIST!")
        return { status = "operation_success" }
    end
}