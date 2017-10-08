/*
  Command pattern
*/
/*************************
  2.1 入力の設定
**************************/
void InputHandler::handleInput() {
  if (isPressed(BUTTON_X))       jump();
  else if (isPressed(BUTTON_Y))  fireGun();
  else if (isPressed(BUTTON_A))  swapWeapon();
  else if (isPressed(BUTTON_B))  lurchIneffenctively();
}

//  入力とコマンド（アクション）を分離
class Command {
public:
  virtual ~Command() {}
  virtual void execute() = 0;
};

class JumpCommand : public Command {
public:
  virtual void execute() { jump(); }
};

class FireCommand : public Command {
public:
  virtual void execute() { fireGun(); }
};

class InputHandler {
public:
  void handleInput();
  
  // 以下、コマンドをボタンに割り当てるメソッドなど
  
private:
  Command* buttonX_;
  Command* buttonY_;
  Command* buttonA_;
  Command* buttonB_;
};

void InputHndler::handleInput() {
  if (isPressed(BUTTON_X))      buttonX_->execute();
  else if (isPressed(BUTTON_Y)) buttonY_->execute();
  else if (isPressed(BUTTON_A)) buttonA_->execute();
  else if (isPressed(BUTTON_B)) buttonB_->execute();
}

/*************************
  2.2 アクターへの指示
**************************/

class Command {
public:
  virtual ~Command() {}
  virtual void execute(GameActor& actor) = 0;
};

class JumpCommand : public Command {
  virtual void execute(GameActor& actor) {
    actor.jump();
  }
};

Command* InputHandler::handleInput() {
  if (isPressed(BUTTON_X)) return buttonX_;
  if (isPressed(BUTTON_Y)) return buttonY_;
  if (isPressed(BUTTON_A)) return buttonA_;
  if (isPressed(BUTTON_B)) return buttonB_;
  
  return NULL;
}

Command* command = inputHandler.handleInput();
if (command) {
  command->execute(actor);
}

/*************************
  2.3 「取り消し」と「再実行」
**************************/
class MoveUnitCommand : public Command {
public:
  MoveUnitCommand(Unit* unit, int x, int y)
  : unit_(unit), x_(x), y_(y)
  { }
  
  virtual void execute() {
    unit_->moveTo(x_, y_);
  }

private:
  Unit* unit_;
  int x_;
  int y_;
};

Command* handleInput() {
  Unit* unit = getSelectedUnit();
  
  if (isPressed(BUTTON_UP)) {
    int destY = unit->y() - 1;
    return new MoveUnitCommand(unit, unit->x(), destY);
  }
  
  if (isPressed(BUTTON_DOWN)) {
    int destY = unit->y() + 1;
    return new MoveUnitCommand(unit, unit->x(), destY);
  }
  
  // ...
  
  return NULL;
}

class Command {
public:
  virtual ~Command() { }
  virtual void execute() = 0;
  virtual void undo() = 0;
};

class MoveUnitCommand : public Command {
public:
  MoveUnitCommand(Unit* unit, int x, int y)
  : unit_(unit), x_(x), y_(y), xBefore_(0), yBefore_(0)
  { }
  
  virtual void execute() {
    xBefore_ = unit_->x();
    yBefore_ = unit_->y();
    unit_->moveTo(x_, y_);
  }
  
  virtual void undo() {
    unit_->moveTo(xBefore_, yBefore_);
  }
private:
  Unit* unit_;
  int x_, y_;
  int xBefore_, yBefore_;
};