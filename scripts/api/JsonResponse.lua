JsonResponse = {}

function JsonResponse:New()
    local object = {}
    setmetatable(object, self)
    self.__index = self

    object.status = 200
    object.content = ""
    object.type = "application/json; charset=utf-8"

    return object
end

function JsonResponse:setContent(content)
    self.content = content
end

function JsonResponse:setType(type)
    self.type = type
end

function JsonResponse:setStatus(code)
    self.status = code
end

return JsonResponse