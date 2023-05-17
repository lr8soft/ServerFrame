local JsonResponse = require("response/JsonResponse")
local HttpResponse = require("response/HttpResponse")
local DB = require("model/db")
local token = require("jwt/token")
user = {}

user.login = function(request)
    response = JsonResponse:New()
    -- 检测是否post
    if request.METHOD ~= "POST" then
        response:setContent({ status = "operation_failed", info = "请求方法错误" })
        return response
    end
    -- 请求完整检测
    local username = request.POST["username"]
    local password = request.POST["password"]
    if username == nil or password == nil then
        response:setContent({ status = "operation_failed", info = "请求参数错误" })
        return response
    end
    print("user login username:", username)
    -- 找一下用户
    local user = DB.User.get
           :where({username = username})
           :first()
    -- 检测用户是否存在
    if user == nil then
        response:setContent({ status = "operation_failed", info = "用户不存在" })
        return response
    end
    -- 检测密码是否正确
    if user.password ~= password then
        response:setContent({ status = "operation_failed", info = "密码错误" })
        return response
    end

    local result = { 
        status = "operation_success",
        info = "登录成功"
    }

    
    local payload = {
        username = user.username,
        isLogin = true
    }
    -- 生成token
    local strResult = token.GetToken(payload)
    response:setHeader("jwt_token", strResult)
    response:setContent(result)
    return response
end

user.regist = function(request)
    response = JsonResponse:New()
    -- 请求完整检测
    local username = request.POST["username"]
    local password = request.POST["password"]
    if username == nil or password == nil then
        response:setContent({ status = "operation_failed", info = "请求参数错误" })
        return response
    end
    print("user try regist username:", username)

    local user = DB.User.get
               :where({username = username})
               :first()
    -- 检测用户是否存在
    if user ~= nil then
        response:setContent({ status = "operation_failed", info = "用户已存在" })
        return response
    end
    -- 创建用户
    local obj = DB.User({
        username = username,
        password = password,
        time_create = os.time()
    })
    obj:save()

    result = {
        status = "operation_success",
        info = "注册成功",
        user = {
            name = obj.username,
            id = obj.id
        }
    }
    response:setContent(result)
    return response
end

user.checktoken = function(request)
    response = JsonResponse:New()
    -- 请求完整检测
    local jwtToken = request.HEADER["jwt_token"]
    if jwtToken == nil then
        response:setContent({ status = "operation_failed", info = "token为空" })
        return response
    end

    resultText = nil
    if token.CheckValid(jwtToken) then
        resultText = "token有效"
    else
        resultText = "token无效"
    end

    result = {
        status = "operation_success",
        info = resultText,
        decode = token.decodePayload(jwtToken)
    }
    response:setContent(result)

    return response
end

return user