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
public:
	void addObserver(Observer* observer) {
		// 配列に追加する
	}

	void removeObserver(Observer* observer) {
		// 配列から削除する
	}

protected:
	void notify(const Entity& eintity, Event event) {
		for (int i=0; i < numObservers_; i++) {
			observers_[i]->onNotify(entity, event);
		}
	}

private:
	Observer* observers_[MAX_OBSERVERS];
	int numObservers_;
};

// 4.2.3 観測可能な物理エンジン
class Physics : public Subject {
public:
	void updateEntity(Entity& entity);
}


/*********************
	4.4 「動的メモリ確保の処理が多すぎる」
*********************/
// 4.4.1 連結オブザーバ
// これまでに見てきたコードでは、Subjectを監視している各ObserverへのポインタのリストをSubjectが持っている
// クラスObserver自体は、そのリストへの参照を持ってはいない
// Obsererにほんの小さな状態を持たせることにするならば、
// サブジェクトが保持するリストを「オブザーバそのものを使って構成することで、
// 割り当ての問題を解決できます。
class Subject {
public:
	Subject() : head_(NULL)
	{ }

	// method definition

private:
	Observer* head_;
};

class Observer {
	friend class Subject;

public:
	Observer() : next_(NULL)
	{ }

	// others

private:
	Observer* next_;
};

void Subject::addObserver(Observer* observer) {
	observer->next_ = head_;
	head_ = obserber;
}

void Subject::removeObserver(Observer* observer) {
	if (head_ == observer) {
		head_ = obserber->next_;
		observer->next_ = NULL;
		return;
	}

	Observer* current = head_;
	while (current != NULL) {
		if (current->next_ == observer) {
			current->next_ = obserber->next_;
			observer->next_ = NULL;
			return;
		}

		current = current->next_;
	}
}

void Subject::notify(const Entity& entity, Event event) {
	Observer* observer = head_;
	while (observer != NULL) {
		observer->onNotify(entity, event);
		observer = observer->next_;
	}
}