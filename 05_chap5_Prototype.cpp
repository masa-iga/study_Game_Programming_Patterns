/*
	Prototype:
		あるインスタンスをプロトタイプとして使うことで、作成するオブジェクトの種類を特定する。
		そのプロトタイプをコピーすることで、新しいオブジェクトを作成する
*/
/*
	5.1 デザインパターン「プロトタイプ」
*/

class Monster {
	// 中身
};

class Ghost : public Monster {};
class Demon : public Monster {};
class Sorcerer : public Monster {};

class Spawner {
public:
	virtual ~Spawner() {}
	virtual Monster* spawnMonster() = 0;
};

class GhostSpawner : public Spawner() {
public:
	virtual Monster* spawnMonster() {
		return new Ghost;
	}
};

class DemonSpawner : public Spawner {
public:
	virtual Monster* spawnMonster() {
		return new Demon;
	}
};

/*
	上記はよくないコード
		たくさんのクラスがあり、決まり文句の処理があり、同じ処理があり、重複あり、、、
	それをプロトタイプパターンが解決する
		鍵となる着想は、オブジェクトは自分自身に似たオブジェクトを生成できるということ
*/

class Monster {
public:
	virtual ~Monster() {}
	virtual Monster* clone() = 0;
	// その他のコード
};

class Ghost : public Monster {
public:
	Ghost(int health, int speed)
	: health_(health),
	  speed_(speed)
	  {}

	 virtual Monster* clone() {
	 	return new Ghost(health_, speed_);
	 }

private:
	int health_;
	int speed_;
};

class Spawner {
public:
	Spawner(Monster* prototype)
	: prototype_(prototype)
	{}

	Monster* spawnMonster() {
		return prototype_->clone();
	}

private:
	Monster* prototype_;
}

Monster* ghostPrototype = new Ghost(15, 3);
Spawner* ghostSpawner = new Spawner(ghostPrototype);

/*
	このパターンの良いところは、プロトタイプのクラスの複製を作成するだけでなく、状態も複製するというところです
*/

/*
	5.1.1 このパターンの働き
*/
/*
	問題点
		１．　未だに各々の怪物クラスにclone()を実装する必要はある
		２．　複製をshallowにするかdeepにするか	
*/

/*
	5.1.2	怪物生成関数
*/
/*
	各Monster用に別々の生成クラスを作成するのではなく、次のように生成関数を作るのでもよいのです
*/
Monster* spawnGhost() {
	return new Ghost();
}

/*
	この場合、怪物生成クラスSpawnerは、関数へのポインタを１つ持つだけでよい
*/
typedef Monster* (*SpawnCallback)();

class Spawner {
public:
	Spawner(SpawnCallback spawn)
	: spawn_(spawn)
	{}

	Monser* spawnMonster() { return spawn_(); }

private:
	SpawnCallback spawn_;
};

/*
	幽霊の生成関数を作るには、次のようにする
*/
Spanwer* ghostSpawner = new Spawner(spawnGhost);
