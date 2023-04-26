---jwt token里的base64url
base64url = {}

function base64url.encodeB64(source)
    local b = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
    return ((source:gsub('.', function(x)
        local r, b = '', x:byte()
        for i = 8, 1, -1 do r = r .. (b % 2 ^ i - b % 2 ^ (i - 1) > 0 and '1' or '0') end
        return r;
    end) .. '0000'):gsub('%d%d%d?%d?%d?%d?', function(x)
        if (#x < 6) then return '' end
        local c = 0
        for i = 1, 6 do c = c + (x:sub(i, i) == '1' and 2 ^ (6 - i) or 0) end
        return b:sub(c + 1, c + 1)
    end) .. ({ '', '==', '=' })[#source % 3 + 1])
end

function base64url.decodeB64(source)
    local b = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
    source = string.gsub(source, '[^' .. b .. '=]', '')
    return (source:gsub('.', function(x)
        if (x == '=') then return '' end
        local r, f = '', (b:find(x) - 1)
        for i = 6, 1, -1 do r = r .. (f % 2 ^ i - f % 2 ^ (i - 1) > 0 and '1' or '0') end
        return r;
    end):gsub('%d%d%d?%d?%d?%d?%d?%d?', function(x)
        if (#x ~= 8) then return '' end
        local c = 0
        for i = 1, 8 do c = c + (x:sub(i, i) == '1' and 2 ^ (8 - i) or 0) end
        return string.char(c)
    end))
end

function base64url.encode(source)
    local base64 = base64url.encodeB64(source)
    local url = base64:gsub("+", "-"):gsub("/", "_"):gsub("=", "")
    return url
end

function base64url.decode(source)
    local base64 = source:gsub("-", "+"):gsub("_", "/")
    local padding = #base64 % 4
    if padding > 0 then
        base64 = base64 .. string.rep("=", 4 - padding)
    end
    local str = base64url.decodeB64(base64)
    return str
end

return base64url