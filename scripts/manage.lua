local userModule = require("user")

url = {
    user = userModule,
    test = function(request)
        result = {
            text = "Hello ServerFrame",
            request = request
        }
        return result
    end
}
