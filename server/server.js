var net = require('net');

var clients = [];

var server = net.createServer(function(socket) {
    // Adiciona ele na lista
    clients.push(socket);
    console.log("Novo cliente");

    // Quando um clientes desconectar, tiramos ele da lista
    socket.on('close', function () {
        clients.splice(clients.indexOf(socket), 1);
    });

    socket.on('data', function (data) {
        console.log(data);
        for (i in clients)
            clients[i].write(data);
    })

    socket.on('error', function(err){
        console.log(err);
        socket.end();
    });
});

server.on('error', function(err){
    console.log(err);
});


server.listen(8000, function() {
    console.log("Servidor iot na porta 8000");
});
