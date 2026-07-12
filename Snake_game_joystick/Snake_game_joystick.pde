import processing.serial.*;

Serial myPort;

int cols = 20;
int rows = 20;
int scl = 30;

ArrayList<PVector> snake = new ArrayList<PVector>();

PVector food;

int xdir = 1;
int ydir = 0;

int joyX = 512;
int joyY = 512;

boolean gameOver = false;

// SPEED CONTROL
int moveDelay = 200;   // bigger = slower
int lastMove = 0;

void setup() {

  size(600, 600);

  // CHECK AVAILABLE PORTS
  println(Serial.list());

  // CHANGE COM PORT IF NEEDED
  myPort = new Serial(this, "COM9", 9600);

  myPort.bufferUntil('\n');

  snake.add(new PVector(10, 10));

  pickLocation();
}

void draw() {

  background(0);

  // SHOW JOYSTICK VALUES
  fill(255);
  textSize(20);
  text("X: " + joyX, 20, 20);
  text("Y: " + joyY, 20, 50);

  if (gameOver) {

    fill(255, 0, 0);
    textSize(50);
    textAlign(CENTER);

    text("GAME OVER", width/2, height/2);

    return;
  }

  // CONTROL SNAKE
  controlSnake();

  // MOVE SNAKE AFTER DELAY
  if (millis() - lastMove > moveDelay) {

    moveSnake();

    lastMove = millis();
  }

  // DRAW FOOD
  fill(255, 0, 0);

  rect(food.x * scl, food.y * scl, scl, scl);

  // DRAW SNAKE
  fill(0, 255, 0);

  for (PVector s : snake) {

    rect(s.x * scl, s.y * scl, scl, scl);
  }
}

void moveSnake() {

  PVector head = snake.get(snake.size()-1).copy();

  head.x += xdir;
  head.y += ydir;

  // WALL COLLISION
  if (head.x < 0 || head.y < 0 || head.x >= cols || head.y >= rows) {

    gameOver = true;
  }

  // SELF COLLISION
  for (PVector s : snake) {

    if (head.x == s.x && head.y == s.y) {

      gameOver = true;
    }
  }

  snake.add(head);

  // FOOD COLLISION
  if (head.x == food.x && head.y == food.y) {

    pickLocation();

    // OPTIONAL: increase speed gradually
    if (moveDelay > 50) {
      moveDelay -= 5;
    }

  } else {

    snake.remove(0);
  }
}

void pickLocation() {

  int x = floor(random(cols));
  int y = floor(random(rows));

  food = new PVector(x, y);
}

void controlSnake() {

  // LEFT
  if (joyX < 300) {

    xdir = -1;
    ydir = 0;
  }

  // RIGHT
  else if (joyX > 700) {

    xdir = 1;
    ydir = 0;
  }

  // UP
  else if (joyY < 300) {

    xdir = 0;
    ydir = -1;
  }

  // DOWN
  else if (joyY > 700) {

    xdir = 0;
    ydir = 1;
  }
}

void serialEvent(Serial myPort) {

  String data = myPort.readStringUntil('\n');

  if (data != null) {

    data = trim(data);

    String values[] = split(data, ',');

    if (values.length == 2) {

      joyX = int(values[0]);
      joyY = int(values[1]);
    }
  }
}
