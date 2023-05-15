local userModule = require("app/user")
httpsServer = {
    isHttps = true,
    port = 443,
    address = "0.0.0.0",
    url = {
        user = userModule
    }
}

return httpsServer