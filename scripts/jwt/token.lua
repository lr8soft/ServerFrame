
local base64 = require("jwt/base64")
local strUtils = require("utils/stringUtils")

token = {}

function token.GetHeaderToken()
    local header = {
        alg = "HS256",
        typ = "JWT"
    }
    local strHeader = strUtils.TableToStr(header)
    local strEncodeHeader = base64.encode(strHeader)
    return strEncodeHeader
end

function token.GetPayLoadToken(payLoadTable)
    local strPayload = strUtils.TableToStr(payLoadTable)
    local strEncodePayload = base64.encode(strPayload)
    return strEncodePayload
end

function token.GetSignatureToken(headerToken, payloadToken)
    local signature = headerToken .. "." .. payloadToken
    local strEncodeSignature = base64.encode(signature)
    return strEncodeSignature
end

function token.GetToken(payLoadTable)
    local headerToken = token.GetHeaderToken()
    local payloadToken = token.GetPayLoadToken(payLoadTable)
    local signatureToken = token.GetSignatureToken(headerToken, payloadToken)
    local token = headerToken .. "." .. payloadToken .. "." .. signatureToken
    return token
end


return token