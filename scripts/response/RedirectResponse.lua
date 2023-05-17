RedirectResponse = {}

function RedirectResponse:New(loc)
    local object = {}
    setmetatable(object, self)
    self.__index = self

    object.status = 301
    object.content = ""
    object.headers = {
        ["Location"] = loc,
        ["Content-Type"] = "application/json; charset=utf-8"
    }
    return object
end

function RedirectResponse:setContent(content)
    self.content = content
end

function RedirectResponse:setStatus(code)
    self.status = code
end

function RedirectResponse:setHeader(key, value)
    self.headers[key] = value
end

return RedirectResponse