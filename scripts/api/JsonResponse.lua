
function JsonResponse:New()
    object = {}
    setmetatable(object, self)
    self.status = 200
    self.content = ""
    self.headers = {
        ["Content-Type"] = "application/json; charset=utf-8",
        ["Content-Length"] = 0
    }
    return object
end

function HttpResponse:setContent(content)
    self.content = content
    self.headers["Content-Length"] = string.len(content)
end

function HttpResponse:setHeaderItem(key, value)
    self.headers[key] = value
end

function HttpResponse:setStatus(code)
    self.status = code
end