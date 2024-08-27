const express = require("express")
const {SerialPort} = require("serialport")
const {ReadlineParser} = require("@serialport/parser-readline")
const {Server} = require("socket.io")
const http = require("http")

const app = express()
const server = http.createServer(app)
const io = new Server(server)

app.use(express.json())

app.get("/", (req, res) => {
    res.sendFile(__dirname + "/views/index.html");
})

io.on("connection", (socket) => {
    console.log("Connected...");
    socket.on("disconnect", () => {
        console.log("Disconnected...");
    })
})

const port = new SerialPort({
    path: "COM3",
    baudRate: 19200
})

const parser = port.pipe(new ReadlineParser({delimiter: "\r\n"}))

parser.on("data", (result) => {
    console.log("Distances from ultrasonic -> ", result);
    io.emit("data", {data: result})
})

app.post("/arduinoApi", (req, res) => {
    const data = req.body.data

    port.write(data, (err) => {
        if(err) {
            console.log("err :", err);
            res.status(500).json({error: "write data error"})
        }

        console.log("Data send to arduino -> ", data);
        res.end()
    })
})

server.listen(3000, () => {
    console.log("Server running");
})

