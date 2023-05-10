
stringUtils = {}


function stringUtils.split(str, splitor)
    -- 将字符串str以splitor分割为多个子串并返回
    local subStrList = {}
    local splitorLen = string.len(splitor)
    local start = 1
    local pos = string.find(str, splitor, start, true)
    while pos ~= nil do
        local subStr = string.sub(str, start, pos - 1)
        table.insert(subStrList, subStr)
        start = pos + splitorLen
        pos = string.find(str, splitor, start, true)
    end

    local subStr = string.sub(str, start)
    table.insert(subStrList, subStr)

    return subStrList
end

return stringUtils