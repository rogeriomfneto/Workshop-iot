net = require('net');

var clients = [];

var server = net.createServer(function (socket) {

  socket.name = socket.remoteAddress + ":" + socket.remotePort;
  var buffer = "";

  clients.push(socket);

  socket.write("Bem vindo " + socket.name + "\n");
  broadcast(socket.name + " entrou no chat.\n", socket);

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
    broadcast(socket.name + " saiu do chat.\n");
  });

  socket.on('error', function (err) {
    clients.splice(clients.indexOf(socket), 1);
  });
  
  function broadcast(message, sender) {
    clients.forEach(function (client) {
      client.write(message);
    });
  }
});

server.timeout = 0;
server.listen(8000);
