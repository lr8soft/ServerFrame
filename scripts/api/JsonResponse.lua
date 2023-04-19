JsonResponse = {}

function JsonResponse:New()
    local object = {}
    setmetatable(object, self)
    self.__index = self

    self.status = 200
    self.content = ""
    self.headers = {
        ["Content-Type"] = "application/json; charset=utf-8"
    }
    return object
end

function JsonResponse:setContent(content)
    self.content = content
end

function JsonResponse:setHeaderItem(key, value)
    self.headers[key] = value
end

function JsonResponse:setStatus(code)
    self.status = code
end

return JsonResponse