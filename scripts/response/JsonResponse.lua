JsonResponse = {}

function JsonResponse:New()
    local object = {}
    setmetatable(object, self)
    self.__index = self

    object.status = 200
    object.content = ""
    object.headers = {
        ["Content-Type"] = "application/json; charset=utf-8"
    }
    return object
end

function JsonResponse:setContent(content)
    self.content = content
end

function JsonResponse:setStatus(code)
    self.status = code
end

function JsonResponse:setHeader(key, value)
    self.headers[key] = value
end

return JsonResponse