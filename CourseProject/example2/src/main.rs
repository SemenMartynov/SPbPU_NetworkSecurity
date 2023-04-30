// Import necessary packages and modules
use actix_files::NamedFile;
use actix_web::{
    middleware, web, App, Error, HttpRequest, HttpResponse, HttpServer, Responder,
};
use actix_web_actors::ws;
use env_logger;

// Import local server module
mod server;
use server::MyWebSocket;

// Serve index.html asynchronously
async fn serve_index() -> impl Responder {
    NamedFile::open_async("./static/index.html").await.unwrap()
}

// Serve WebSocket asynchronously
async fn serve_websocket(req: HttpRequest, stream: web::Payload) -> Result<HttpResponse, Error> {
    ws::start(MyWebSocket::new(), &req, stream)
}

// Entry point of the application with async main
#[actix_web::main]
async fn main() -> std::io::Result<()> {
    // Initialize logger using environment variables, default level: "info"
    env_logger::init_from_env(env_logger::Env::new().default_filter_or("info"));

    // Log a message about the server starting
    log::info!("starting HTTP server at http://localhost:8080");

    // Create and configure the HTTP server
    HttpServer::new(|| {
        App::new()
            .route("/", web::get().to(serve_index)) // Serve index.html
            .route("/ws", web::get().to(serve_websocket)) // Serve WebSocket
            .wrap(middleware::Logger::default()) // Add logging middleware
    })
    .workers(2) // Set the number of worker threads
    .bind("127.0.0.1:8080")? // Bind server to address
    .run() // Start the server
    .await // Wait for the server to finish gracefully
}