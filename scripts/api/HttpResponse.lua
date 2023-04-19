HttpResponse = {}
function HttpResponse:New()
    local object = {}
    setmetatable(object, self)
    self.__index = self

    self.status = 200
    self.content = ""
    self.headers = {
        ["Content-Type"] = "text/html; charset=utf-8"
    }
    return object
end

function HttpResponse:setContent(content)
    self.content = content
end

function HttpResponse:setHeaderItem(key, value)
    self.headers[key] = value
end

function HttpResponse:setStatus(code)
    self.status = code
end

return HttpResponse