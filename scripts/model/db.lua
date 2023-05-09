DB = {
    DEBUG = true,
    new = true,
    backtrace = true,
    name = "database.db",
    type = "sqlite3"
}

local Table = require("orm.model")
local fields = require("orm.tools.fields")

tableInfo = {
    User = Table({
        __tablename__ = "user",
        username = fields.CharField({max_length = 100, unique = true}),
        password = fields.CharField({max_length = 50, unique = true}),
        time_create = fields.DateTimeField({null = true})
    })
}

return tableInfo