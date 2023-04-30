// Import necessary modules from std and external crates
use std::fs::File;
use std::io::BufReader;
use std::io::Read;
use std::time::{Duration, Instant};

use actix::prelude::*;
use actix_web::web::Bytes;
use actix_web_actors::ws;

// Configure constants for heartbeat (how often it sends) and client timeout
const HEARTBEAT_INTERVAL: Duration = Duration::from_secs(5);
const CLIENT_TIMEOUT: Duration = Duration::from_secs(10);

// WebSocket actor
pub struct MyWebSocket {
    hb: Instant, // Store the time when the last heartbeat message was received
}

// Implement MyWebSocket
impl MyWebSocket {
    // Constructor
    pub fn new() -> Self {
        Self { hb: Instant::now() }
    }

    // Schedule regular heartbeat messages, and stop the WebSocket connection if timeout
    fn schedule_heartbeat(&self, ctx: &mut <Self as Actor>::Context) {
        ctx.run_interval(HEARTBEAT_INTERVAL, |act, ctx| {
            if Instant::now().duration_since(act.hb) > CLIENT_TIMEOUT {
                ctx.stop();
                return;
            }

            ctx.ping(b"");
        });
    }
}

// Implement Actor for MyWebSocket
impl Actor for MyWebSocket {
    type Context = ws::WebsocketContext<Self>;

    // When the actor starts, schedule the heartbeat messages
    fn started(&mut self, ctx: &mut Self::Context) {
        self.schedule_heartbeat(ctx);
    }
}

// Implement StreamHandler with Result type for handling WebSocket events
impl StreamHandler<Result<ws::Message, ws::ProtocolError>> for MyWebSocket {
    // Handle incoming WebSocket messages (ping, pong, text, close)
    fn handle(&mut self, msg: Result<ws::Message, ws::ProtocolError>, ctx: &mut Self::Context) {
        match msg {
            Ok(ws::Message::Ping(msg)) => {
                self.hb = Instant::now(); // Update the heartbeat time
                ctx.pong(&msg); // Respond with a pong message
            }
            Ok(ws::Message::Pong(_)) => {
                self.hb = Instant::now(); // Update the heartbeat time
            }
            Ok(ws::Message::Text(_)) => {
                let file = File::open("./static/Sample10mb").unwrap();
                let mut reader = BufReader::new(file);
                let mut buffer = Vec::new();

                reader.read_to_end(&mut buffer).unwrap();
                ctx.binary(Bytes::from(buffer)); // Send the binary data from a file
            }
            Ok(ws::Message::Close(reason)) => {
                ctx.close(reason); // Close the WebSocket connection
                ctx.stop(); // Stop the WebSocket Actor
            }
            _ => ctx.stop(), // Stop the WebSocket Actor for other scenarios
        }
    }
}