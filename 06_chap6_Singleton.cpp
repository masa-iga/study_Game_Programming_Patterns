/*
 *  Singleton
 *      Ensure a class has one instance, and provide a global point of access to it.
*/

/*
 *  6.1.1   クラスのインスタンスを１つに制限する
 *
 *  6.1.2   グローバルなアクセスポイントの提供
 */

class FileSystem
{
public:
    static FileSystem& instance()
    {
        if (instance_ == NULL)
        {
            instance_ = new FileSystem();
        }
        return *instance_;
    }

private:
    FileSystem() {}
    static FileSystem* instance_;
};

/*
 * 最近の新しい実装では次のようになる
 * C+11では、ローカルなstatic変数の初期化子は、並列実行環境であっても１回しか実行されない
 * よってスレッド・セーフ
 */
class FileSystem
{
public:
    static FileSystem& instance()
    {
        static FileSystem *instance = new FileSystem();
        return *instance;
    }

private:
    FileSystem() {}
};


/*
 * One of the way to avoid raicing is to use mutex lock.
 * Refer to Singleton in AOSP.
 * http://androidxref.com/8.0.0_r4/xref/system/core/include/utils/Singleton.h
 * 
 * static TYPE& getInstance() {
 *     Mutex::Autolock _l(sLock);
 *     TYPE* instance = sInstance;
 *     if (instance == 0) {
 *         instance = new TYPE();
 *         sInstance = instance;
 *     }
 *     return *instance;
 * }
 */



/*
 * 6.2  使用する理由
 */
/*
 * - 使わなければインスタンスを生成しない
 * - 初期化は実行時に行われる = 初期化時には必要な変数情報は揃っているはず
 * - シングルトンのサブクラス化も可能。以下にコード
 */
// Base class
class FileSystem
{
public:
	virtual ~FileSystem() {}
	virtual char* read(char* path) = 0;
	virtual void write(char* path, char* text) = 0;
};

// Sub class
class PS4FileSystem : public FileSystem
{
public:
	virtual char* read(char* path)
	{
		;
	}

	virtual void write(char* path, char* text)
	{
		;
	}
};

// Sub class
class WiiFileSystem : public FileSystem
{
Pubblic:
	Virtual char* read(char* path)
	{
		;
	}

	virtual void write(char* path, char* text)
	{
		;
	}
};

// Make it Singleton
class FileSystem
{
public:
	static FileSystem& instance();

	virtual ~FileSystem() {}
	virtual char* read(char* path) = 0;
	virtual void write(char* path, char* text) = 0;

protected:
	FileSystem() {}
};

FileSystem& FileSystem::instance()
{
#if PLATFORM == PLAYSTATION3
	static FileSystem *instance = new PS4FileSystem();
#elif PLATFORM == WII
	static FileSystem *instance = new WiiFileSystem();
#endif

	return *instance;
}



/*
 * 6.3	使用をためらう理由
 */

/*
 * 6.3.1	グローバル変数の問題
 * 		グローバル変数で問題が発生した場合、膨大なコードベースの中を誰が使っているか探す羽目になる
 * 		コードの結合が助長される
 * 		並列実行の障害になる - デッドロックや競合条件などのスレッド同期問題を起こしうる
 */

/*
 * 6.3.2	問題の過剰解決
 * 		シングルトンにより以下が行える
 * 			1. クラスのインスタンスを１つに制限する
 * 			2. グローバルアクセスポイントの提供
 *
 * 		シングルトンパターンを採用するのはほぼ常に２の理由のため
 * 		１が邪魔になることがある。様々なモジュールがロギングするときなど
 */
Log::instance().write("Some event.");

/*
 * 6.3.3	遅延初期化による制御困難
 * 		ゲームでは遅延初期化に頼らない
 * 			1. 遅延初期化が発生した時に、初期化に時間(数百msec)がかかるとUXに影響を与える
 * 			2. フラグメンテーションを避けるためにヒープのメモリ配置制御をする必要があるため
 * 				初期化タイミングをしっておきたい
 */
class FileSystem
{
public:
	static FileSystem& instance() { return instance_; }

private:
	FileSystem() {}
	static FileSystem instance_;
};

/*
 * これで遅延初期化の問題は解決できる。しかし、いくつかの性質を犠牲にする
 * 	- static変数では、ポリモーフィズムが利用できない
 * 	- クラスはstatic変数の初期化の際に生成可能でなければならない
 * 	- インスタンスが使用されないときにも使用メモリを開放することができない
 */



/*
 * 6.4  代替手段
 * 6.4.1    クラスの必要性
 * 6.4.2    インスタンスの生成を１個に制限
 *  シングルトンはグローバルアクセスポイントまで提供してしまうのでアーキの脆弱性を招く
 *  次は動的にインスタンスの単一性を保証する例
 */
class FileSystem
{
public:
    FileSystem()
    {
        asser(!instantiated_);
        instantiated_ = true;
    }

    ~FileSystem() { instantiated_ = false; }

private:
    static bool instantiated_;
};

bool FileSystem::instantiated_ = false;

/*
 * 6.4.3    インスタンスへの便利なアクセスを提供
 *  コーディングの原則は「変数のスコープは、処理に支障のない範囲で可能なかぎり狭くする」
 *  オブジェクトのスコープが小さければ小さいほど、そのオブジェクトを使ったコードを書いている時に
 *  頭に入れておかなければならない箇所が減少します
 *
 *  - 引数渡し
 *  - 基底クラスから取得する
 *      ゲームアーキテクチャの多くは浅く広い継承階層であり、２階層だけということもよくある
 *      その場合、基底クラスGameObjectを利用して必要な情報にアクセスできるようにすればよい
 */
class GameObject
{
protected:
    Log& log() { return log_; }

private:
    static Log& log_;
};

class Enemy : public GameObject
{
    void doSomething()
    {
        log().write(" I can log!");
    }
};

/*
 * - 他のグローバルなデータから取得する
 *   たいていのコードベースにはグローバルにアクセス可能なオブジェクトが２つや３つある
 *      例えば、ゲーム全体の状態を表すGameやWorldといった１つしかないオブジェクト
 *   そのような既存のクラスに追加の状態を含めてしまう
 */
class Game
{
public:
    static Game& instance() { return instance_; }

    Log&        log() { return *log_; }
    FileSystem  fileSystem() { return *files_; }
    AudioPlayer& audioPlayer() { return *audio_; }

    // log_その他を設定する関数

private:
    static Game instance_;
    Log         *log_;
    FileSystem  *files_;
    AudioPlayer *audio;
};

Game::instnce().getAudioPlayer().play(LOUD_BANG);

/*
 * - 他の選択肢として、サービスロケータから取得する方法もある
 */
