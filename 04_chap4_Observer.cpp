/*
	Observer
	オブジェクト間に一対多の依存関係を定義する
	これにより、あるオブジェクトが状態を変えた時に、依存関係にあるすべてのオブジェクトに
	自動的にその変化が知らされ、必要な更新が行われるようにする
*/

/*********************
	4.1 達成の認定
*********************/
/* 誰か気にかけているかどうかは知らないけど、こいつは落っこちちゃったよ。あとは好きにしてね */
void Physics::updateEntity(Entity& entity) {
	bool wasOnSurface = entity.isOnSurface();
	entity.accelerate(GRAVITY);
	entity.update();
	if (wasOnSurface && !entity.isOnSurface()) {
		notify(entity, EVENT_START_FALL);
	}
}

/*********************
	4.2 実装
*********************/
// 4.2.1 Observer
class Observer {
public:
	virtual ~Observer();
	virtual void onNotify(const Entity& entity, Event event) = 0;
};

class Achievements : public Observer {
public:
	virtual void onNotify(const Entity& entity, Event event) {
		switch (event) {
			case EVENT_ENTITY_FELL:
				if (entify.isHero() && heroIsOnBridege_) {
					unlock(ACHIEVEMENT_FELL_OFF_BRIDGE);
				}
				break;
			default: break;
		}
	}

private:
	void unlock(Achievement achievement) {
		// まだ与えられていない場合は達成バッジを与える
	}
	bool heroIsOnBridege_;
};


// 4.2.2 Subject
class Subject {
private:
	Observer* observers_[MAX_OBSERVERS];
	int numObservers_;
};