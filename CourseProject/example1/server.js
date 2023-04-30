// Import required modules
const http = require('http');
const fs = require('fs');
const WebSocket = require('ws');

// Create a WebSocket Server instance with no server argument
const wss = new WebSocket.Server({ noServer: true });

// Set for keeping track of connected clients
const clients = new Set();

// Function for handling incoming HTTP requests and WebSocket upgrade requests
function accept(req, res) {
  // Check if the request is a WebSocket upgrade request on the "/ws" path
  if (req.url === '/ws' && req.headers.upgrade && req.headers.upgrade.toLowerCase() === 'websocket' && req.headers.connection.match(/\bupgrade\b/i)) {
    wss.handleUpgrade(req, req.socket, Buffer.alloc(0), onSocketConnect);
  } else if (req.url === '/') { // Serve the index.html file for requests to the root path
    fs.createReadStream('./index.html').pipe(res);
  } else { // Return a 404 Not Found response for other request URLs
    res.writeHead(404);
    res.end();
  }
}

// Function for handling new WebSocket connections
function onSocketConnect(ws) {
  clients.add(ws); // Add the connected WebSocket to the clients set
  console.log('New connection');

  ws.on('message', function (message) { // Event handler for incoming WebSocket messages
    console.log(`Received message: ${message}`);

    message = message.slice(0, 50); // Limit message length to 50 characters

    // Broadcast the message to all connected clients
    for (let client of clients) {
      client.send(message);
    }
  });

  ws.on('close', function () { // Event handler for closed WebSockets
    console.log('Connection closed');
    clients.delete(ws); // Remove the closed WebSocket from the clients set
  });
}

// Create an HTTP server that listens on port 8080 and calls the accept function for each request
http.createServer((req, res) => {
  accept(req, res);
}).listen(8080);

// Log to the console that the server has started on port 8080
console.log("Server started on port 8080");