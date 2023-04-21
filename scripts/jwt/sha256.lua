
function getSHA256(str)
    local resty_sha256 = require "resty.sha256"
    local restystr= require "resty.string"
    local sha256 = resty_sha256:new()
    sha256:update(str)
    local digest = sha256:final()
    return string.upper(restystr.to_hex(digest))
end