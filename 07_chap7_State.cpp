/*
 * Chap. 7	State
 * 		Allow an object to alter its behavior when its internal state changes.
 * 		The object will appear to change its class.
 */

/*
 * 7.1	みんなそうだった
 * 	小さな横スクロール型のゲーム機の開発をしているとしましょう
 * 	私達の仕事は、ゲーム世界におけるプレーヤーのアバターであるヒロインの実装です
 * 	したがって、ユーザー入力にヒロインが反応しなければなりません。Bボタンを押せばジャンプです
 */
void Heroine::handleInput(Input input)
{
	if (input == PRESS_B)
	{
		yVelocity_ = JUMP_VELOCITY;
		setGraphics(IMAGE_JUMP);
	}
}

/*
 * バグを見つけましたか？
 * そう、「エアー・ジャンプ」ができてしまいます
 * 単純な解決策は真偽値フィールドisJumping_をヒロインに導入します
 */
void Heroin::handleInput(Input input)
{
	if (input == PRESS_B)
	{
		if (!isJumping_)
		{
			isJumping_ = true;
			// jump...
		}
	}
}

/*
 * 次に、ヒロインが地上にいるときにプレーヤーが「下」ボタンを押すとヒロインが屈み、
 * ボタンを話すと直立姿勢に戻るようにしましょう
 */
void Heroine::handleInput(Input input)
{
	if (input == PRESS_B)
	{
		// ジャンプしていなければジャンプ...
		if (!isJuming_)
		{
			isJumping_ = true;
		}
	}
	else if (input == PRESS_DOWN)
	{
		if (!isJumping_)
		{
			setGraphics(IMAGE_DOWN);
		}
	}
	else if (input == RELEASE_DOWN)
	{
		setGraphics(IMAGE_STAND);
	}
}

/*
 * ヒロインはジャンプの途中で直立姿勢になってしまいます。もう１つフラグが必要です。
 *    １．下を押して屈む
 *    ２．Bを押して屈んだ姿勢でジャンプ
 *    ３．空中で下を放す
 */
void Heroine::handleInput(Input input)
{
	if (input == PRESS_B)
	{
		// ジャンプしていなければジャンプ...
		if (!isJuming_ && !isDucking_)
		{
			isJumping_ = true;
		}
	}
	else if (input == PRESS_DOWN)
	{
		if (!isJumping_)
		{
			isDucking_ = true;
			setGraphics(IMAGE_DOWN);
		}
	}
	else if (input == RELEASE_DOWN)
	{
		if (isDucking_)
		{
			isDucking_ = false;
			setGraphics(IMAGE_STAND);
		}
	}
}

/*
 *	 明らかにやり方を間違えています。短いコードなのに手を入れるたびに何かを壊しています。
 */
/*
 *	7.2	解決手段としての有限状態機械
 */
/*
 *	7.3	列挙と分岐
 */
enum State
{
	STATE_STANDING,
	STATE_JUMPING,
	STATE_DUCKING,
	STATE_DIVING
}

void Heroine::handleInput(Input input)
{
	switch (state_)
	{
		case STATE_STANDING:
			if (input == PRESS_B)
			{
				state_ = STATE_JUMPING;
				yVelocity_ = JUMP_VELOCITY;
				setGraphics(IMAGE_JUMP);
			}
			else if (input == PRESS_DOWN)
			{
				state_ = STATE_DUCKING;
				setGraphics(IMAGE_DUCK);
			}
			break;

		// other state
	}
}

/*
 *	この単純な方法では解決しきれない問題が現れるかもしれません
 *	例えば、ヒロインがしばらく屈んでパワーをチャージし、特別なチャージ・アタックをする動きを追加したいとしたら？
 *	どれだけパワーをチャージしたかを保存することとする
 *	すでに毎フレーム呼び出されるupdate()があるとすれば、そこに次のようにコードを追加する
 */
void Heroine::update()
{
	if (state_ == STATE_DUCKING)
	{
		chargeTime_++;
		if (chargeTime_ > MAX_CHARGE)
		{
			superBomb();
		}
	}
}

/*
 *	屈み始めたときにはタイマーをリセットする必要があるので、handleInput()を修正する
 */
void Heroine::handkeInput(Input input)
{
	switch (state_)
	{
		case STATE_STANDING:
			if (input == PRESS_DOWN)
			{
				state_ = STATE_DUCKING;
				chargeTime_0 = 0;
				setGraphics(IMAGE_DUCK);
			}
			// handle other input
			break;

		// other state
	}
}
/*
 * このチャージ・アタックを追加するために、２つのメソッドを修正し、
 * HeroineにフィールドchargeTime_を追加しなければなりませんでした
 * こういったコードとデータのすべてが１箇所にまとめられているべき
 */



/*
 * 7.4 「状態」のパターン
 * 7.4.1 「状態」のインタフェース
 */
class HeroineState
{
public:
  virtual ~HeroineState() {}
  virtual void handleInput(Heroine& heroine,
      Input input) {}
  virtual void update(Heroine& heroine) {}
};



/*
 * 7.5 各状態のクラス
 */
class DuckingState : public HeroineState
{
public:
  DuckingState()
    : chargeTime_(0)
  {}

  virtual void handleInput(Heroine& heroine,
      Input input) {
    if (input == RELEASE_DOWN)
    {
      // 「立っている」状態へと変化
      heroine.setGraphics(IMAGE_STAND);
    }
  }

  virtual boid update(Heroine& heroine) {
    chargeTime_++;
    if (chargeTime_ > MAX_CHARGE)
    {
      heroine.superBomb();
    }
  }

private:
    int chargeTime_;
};



/*
 * 7.6 状態への委任
 *  次に、Heroineに現在状態へのポインタを設けます。
 *  大きなswitch文がなくなったので、代わりに状態state_に委任します。
 */
class Heroine
{
public:
  virtual void handleInput(Input input)
  {
    state_->handleInput(*this, input);
  }

  virtual void update() { state_->update(*this); }

  // other methods

private:
  HeroineState* state_;
};


/*
 * 7.7 状態のオブジェクトの存在場所
 *  状態を変えるにはstate_に新しいポインタを代入する必要がある。
 *  そのオブジェクトはどこにあるか？
 *  一般的な解決法は２つある
 */
 
