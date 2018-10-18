
net = require('net');

var clients = [];

var server = net.createServer(function (socket) {

  socket.name = socket.remoteAddress + ":" + socket.remotePort 

  clients.push(socket);

  socket.write("Welcome " + socket.name + "\n");
  broadcast(socket.name + " joined the chat\n", socket);

  socket.on('data', function (data) {
    broadcast(socket.name + "> " + data, socket);
  });

  socket.on('end', function () {
    clients.splice(clients.indexOf(socket), 1);
    broadcast(socket.name + " left the chat.\n");
  });
  
  function broadcast(message, sender) {
    clients.forEach(function (client) {
      if (client === sender) return;
      client.write(message);
    });
    process.stdout.write(message)
  }

});

server.timeout = 0;
server.listen(8000);
