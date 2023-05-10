
local base64url = require("jwt/base64url")
local strUtils = require("utils/stringUtils")
local json = require("utils/json")
local settings = require("settings")

token = {}

function token.GetHeaderToken()
    local header = {
        alg = "HS256",
        typ = "JWT"
    }
    local strHeader = json.encode(header)
    local strEncodeHeader = base64url.encode(strHeader)
    return strEncodeHeader
end

function token.GetPayLoadToken(payLoadTable)
    local strPayload = json.encode(payLoadTable)
    local strEncodePayload = base64url.encode(strPayload)
    return strEncodePayload
end

function token.GetSignatureToken(headerToken, payloadToken)
    return SHA.hmac256(headerToken .. "." .. payloadToken, settings.secret)
end

function token.GetToken(payLoadTable)
    local headerToken = token.GetHeaderToken()
    local payloadToken = token.GetPayLoadToken(payLoadTable)
    ---根据server端的secret进行加密
    local signatureToken = token.GetSignatureToken(headerToken, payloadToken)
    local token = headerToken .. "." .. payloadToken .. "." .. signatureToken
    return token
end

function token.decodePayload(jwtToken)
    local tokenList = strUtils.split(jwtToken, ".")
    -- 检测jwtToken格式
    if #tokenList ~= 3 then
        print("token format error")
        return nil
    end

    local payloadToken = tokenList[2]
    return json.decode(base64url.decode(payloadToken))
end

function token.CheckValid(jwtToken)
    local tokenList = strUtils.split(jwtToken, ".")
    if #tokenList ~= 3 then
        print("token format error")
        return false
    end
    local headerToken = tokenList[1]
    local payloadToken = tokenList[2]
    local signatureToken = tokenList[3]
    local validationSignature = token.GetSignatureToken(headerToken, payloadToken)
    if validationSignature ~= signatureToken then
        return false
    end
    return true
end


return token