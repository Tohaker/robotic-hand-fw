void handleRoot() {
  server.send(200, "application/json", "{ \"status\": \"operational\" }");
}

void handleNotFound() {
  String message = "{\n";
  message += "\"uri\": ";
  message += "\"" + server.uri() + "\"";
  message += "\n\"method\": ";
  message += "\"" + (server.method() == HTTP_GET) ? "GET\"" : "POST\"";
  message += "\n\"arguments\": ";
  message += "\"" + String(server.args()) + "\"\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += "\"" + server.argName(i) + "\": \"" + server.arg(i) + "\"\n";
  }
  server.send(404, "application/json", message);
}

void handleFingerMovement() {
  int fingerParam = finger;
  bool holdParam = false;

  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "fingers") {
      fingerParam = (server.arg(i)).toInt();
    } else if (server.argName(i) == "hold") {
      if (server.arg(i) == "true") {
          holdParam = true;
      }
    }
  }

  finger = bitRead(fingerParam, 0) | bitRead(fingerParam, 1) << 1 | bitRead(fingerParam, 2) << 2 | bitRead(fingerParam, 3) << 3;
  int fingerList[] = {bitRead(fingerParam, 0), bitRead(fingerParam, 1), bitRead(fingerParam, 2), bitRead(fingerParam, 3)};
  if (holdParam)
    hold = 1;
  else
    hold = 0;

  checkData();
  server.send(200, "application/json", constructJSON(fingerList, holdParam));
}

String constructJSON(int* fingers, bool hold) {
  String fList = "";
  for (int i = 0; i < 4; i++) {
    if (i != 3)
      fList += (String(fingers[i]) + ", ");
    else
      fList += String(fingers[i]);
  }
  
  String response = String("{\n\"fingers_moved\": " + fList + "\n\"held\": " + String(hold) + "}");
  return response;
}
