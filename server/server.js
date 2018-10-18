
net = require('net');

var clients = [];

var server = net.createServer(function (socket) {

  socket.name = socket.remoteAddress + ":" + socket.remotePort;
  var buffer = "";


  clients.push(socket);

  socket.write("Welcome " + socket.name + "\n");
  broadcast(socket.name + " joined the chat\n", socket);

  socket.on('data', function (data) {
    buffer = buffer + data;

    while (buffer.indexOf('\n') != -1) {
        var str = buffer.substr(0, buffer.indexOf('\n')+1);
        buffer = buffer.substr(buffer.indexOf('\n')+1);
        broadcast(socket.name + "> " + str, socket);
    }
  });

  socket.on('end', function () {
    clients.splice(clients.indexOf(socket), 1);
    broadcast(socket.name + " left the chat.\n");
  });

  socket.on('error', function (err) {
    clients.splice(clients.indexOf(socket), 1);
    console.log(err);
  });
  
  function broadcast(message, sender) {
    clients.forEach(function (client) {
      client.write(message);
    });
    process.stdout.write(message)
  }

});

server.timeout = 0;
server.listen(8000);
