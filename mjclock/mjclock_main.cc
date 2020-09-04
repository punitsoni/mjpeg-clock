#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "absl/strings/str_format.h"
#include "absl/types/span.h"
#include "mongoose.h"
#include "test_video_gen.h"

int next_conn_id = 1;

const std::string kDefaultDocumentRoot = "./mjclock/static";
const std::string kMjpegFrameBoundary = "FRAME_BOUNDARY";

// class HttpConnection {
//  public:
//   HandleRequest();
//   HandleClose();
//   HandleTimerEvent();

//  private:
//   mg_connection *conn;
// }

struct ConnectionData {
  const int id;
  mg_connection *conn;
  ConnectionData(int conn_id) : id(conn_id) {}
};

void RunTaskAsync(mg_connection *conn, std::function<void(void)> task) {}

void SendMjpegFrame(mg_connection *conn, absl::Span<uint8_t> jpeg) {
  mg_printf(conn,
            "--%s\r\n"
            "Content-Type: image/jpeg\r\n"
            "Content-Length: %lu\r\n\r\n",
            kMjpegFrameBoundary.c_str(), jpeg.size());

  mg_send(conn, (const char *)&jpeg[0], jpeg.size());
  mg_printf(conn, "\r\n");
}

void http_handle_test_mjpeg(struct mg_connection *conn,
                            struct http_message *req) {
  double lastframe_time = mg_time();

  mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Cache-Control: no-cache\r\n"
            "Cache-Control: private\r\n"
            "Content-Type: multipart/x-mixed-replace; "
            "boundary=%s\r\n\r\n",
            kMjpegFrameBoundary.c_str());

  auto *gen = new TestVideoGenerator();
  absl::PrintF("sending frame %d\n", gen->next_frameid());
  std::vector<uint8_t> buffer = gen->GetNextFrame();

  SendMjpegFrame(conn, absl::Span<uint8_t>(buffer));

  // setup timer
  conn->user_data = (void *)gen;
  mg_set_timer(conn, lastframe_time + 0.030);  // 30 ms
}

void HandleHttpRequest(struct mg_connection *conn, struct http_message *req) {
  auto conn_data = (ConnectionData *)conn->user_data;
  const std::string request_method(req->method.p, req->method.len);
  const std::string request_uri(req->uri.p, req->uri.len);
  const std::string request_query(req->query_string.p, req->query_string.len);
  const std::string proto(req->proto.p, req->proto.len);
  absl::PrintF("--------------------------------\n");
  absl::PrintF("[HTTP Request conn_id=%d]\n", conn_data->id);
  absl::PrintF("method: %s\n", request_method);
  absl::PrintF("uri: %s\n", request_uri);
  absl::PrintF("query: %s\n", request_query);
  absl::PrintF("--------------------------------\n");

  if (request_uri == "/test_mjpeg") {
    return http_handle_test_mjpeg(conn, req);
  }

  // Serve static files.
  auto opts = mg_serve_http_opts();
  opts.document_root = kDefaultDocumentRoot.c_str();
  mg_serve_http(conn, req, opts);
}

void HandleTimerEvent(mg_connection *conn, double timestamp) {
  auto *gen = (TestVideoGenerator *)(conn->user_data);
  absl::PrintF("sending frame %d\n", gen->next_frameid());
  std::vector<uint8_t> buffer = gen->GetNextFrame();

  SendMjpegFrame(conn, absl::Span<uint8_t>(buffer));

  mg_set_timer(
      conn,
      timestamp + 0.03);  // Send us timer event again after 0.5 seconds
}

void MgEventHandler(mg_connection *conn, int ev, void *p) {
  switch (ev) {
    case MG_EV_HTTP_REQUEST: {
      return HandleHttpRequest(conn, (http_message *)p);
    }
    case MG_EV_SEND: {
      // absl::PrintF("MG_EV_SEND, conn=%p\n", conn);
      break;
    }
    case MG_EV_CLOSE: {
      auto *conn_data = (ConnectionData *)conn->user_data;
      absl::PrintF("MG_EV_CLOSE, conn=%p\n", conn);
      absl::PrintF("Close connection, conn_id=%d\n", conn_data->id);
      delete conn_data;
      break;
    }
    case MG_EV_ACCEPT: {
      absl::PrintF("MG_EV_ACCEPT, conn=%p\n", conn);
      auto *conn_data = new ConnectionData(next_conn_id++);
      conn->user_data = conn_data;
      absl::PrintF("New connection, conn_id=%d\n", conn_data->id);
      break;
    }
    case MG_EV_CONNECT: {
      absl::PrintF("MG_EV_CONNECT, conn=%p\n", conn);
      break;
    }
    case MG_EV_TIMER: {
      return HandleTimerEvent(conn, *(double *)p);
    }
  }
}

void func() {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);
  struct mg_connection *conn = mg_bind(&mgr, "8000", MgEventHandler);

  mg_set_protocol_http_websocket(conn);

  while (true) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
}

int main(int argc, char **argv) { func(); }
