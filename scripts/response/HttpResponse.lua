HttpResponse = {}

function HttpResponse:New()
    local object = {}
    setmetatable(object, self)
    self.__index = self

    object.status = 200
    object.content = ""
    object.headers = {
        ["Content-Type"] = "text/html; charset=utf-8"
    }
    return object
end

function HttpResponse:setContent(content)
    self.content = content
end

function HttpResponse:setStatus(code)
    self.status = code
end

function HttpResponse:setHeader(key, value)
    self.headers[key] = value
end

return HttpResponse