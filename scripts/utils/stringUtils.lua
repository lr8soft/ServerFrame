
stringUtils = {}

function stringUtils.ToStringEx(value)
    if type(value)=='table' then
       return stringUtils.TableToStr(value)
    elseif type(value)=='string' then
        return "\""..value.."\""
    else
       return tostring(value)
    end
end

function stringUtils.TableToStr(t)
    if t == nil then return "" end
    local retstr= "{"

    local i = 1
    for key,value in pairs(t) do
        local signal = ","
        if i==1 then
            signal = ""
        end

        if key == i then
            retstr = retstr..signal..stringUtils.ToStringEx(value)
        else
            if type(key)=='number' or type(key) == 'string' then
                retstr = retstr..signal..''..stringUtils.ToStringEx(key).."="..stringUtils.ToStringEx(value)
            else
                if type(key)=='userdata' then
                    retstr = retstr..signal.."*s"..stringUtils.TableToStr(getmetatable(key)).."*e".."="..stringUtils.ToStringEx(value)
                else
                    retstr = retstr..signal..key.."="..stringUtils.ToStringEx(value)
                end
            end
        end

        i = i+1
    end

    retstr = retstr.."}"
    return retstr
end

return stringUtils