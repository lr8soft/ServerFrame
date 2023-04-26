
local base64url = require("jwt/base64url")
local strUtils = require("utils/stringUtils")
local settings = require("settings")

token = {}

function token.GetHeaderToken()
    local header = {
        alg = "HS256",
        typ = "JWT"
    }
    local strHeader = strUtils.TableToStr(header)
    local strEncodeHeader = base64url.encode(strHeader)
    return strEncodeHeader
end

function token.GetPayLoadToken(payLoadTable)
    local strPayload = strUtils.TableToStr(payLoadTable)
    local strEncodePayload = base64url.encode(strPayload)
    return strEncodePayload
end

function token.GetSignatureToken(headerToken, payloadToken)
    local signature = headerToken .. "." .. payloadToken
    local strEncodeSignature = base64url.encode(signature)
    return strEncodeSignature
end

function token.GetToken(payLoadTable)
    local headerToken = token.GetHeaderToken()
    local payloadToken = token.GetPayLoadToken(payLoadTable)
    ---根据server端的secret进行加密
    local signatureToken = SHA.hmac256(headerToken .. "." .. payloadToken, settings.secret)
    local token = headerToken .. "." .. payloadToken .. "." .. signatureToken
    return token
end


return token