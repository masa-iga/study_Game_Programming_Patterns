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