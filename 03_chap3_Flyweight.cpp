/*
Flyweight
  共有を利用することで、多数の細かいオブジェクトを効率良く処理する
  オブジェクトの個数があまりに多い時に利用する
  オブジェクトのデータを状況非依存なものと、そのインスタンスに固有なものに分けて考える
*/
/**************************
  3.1 木があってこその森
**************************/
class Tree {
  Mesh mesh_;
  Texture bark_;
  Texture leaves_;
  Vector position_;
  double height_;
  double thickness_;
  Color barkTint_;
  Color leafTint_;
};

class TreeModel {
  Mesh mesh_;
  Texture bark_;
  Texture leaves_;
};

class Tree {
private:
  TreeModel* model_;
  
  Vector position_;
  double height_;
  double tickness_;
  Color barkTint_;
  Color leafTint_;
};

/**************************
  3.4 木を生やす場所
**************************/
/*
  いけてない例
*/
enum Terrain {
  TERRAIN_GRASS,
  TERRAIN_HILL,
  TERRAIN_RIVER,
  ...
}

class World {
private:
  Terrain tiles_[WIDTH][HEIGHT];
};

int World::getMovementCost(int x, int y) {
  switch (tiles_[x][y]) {
    case TERRAIN_GRASS: return 1;
    case TERRAIN_HILL: return 3;
    case TERRAIN_RIVER: return 2;
    ...
  }
}

bool World::isWater(int x, int y) {
  switch (tiles_[x][y]) {
    case TERRAIN_GRASS: return false;
    case TERRAIN_HILL: return false;
    case TERRAIN_RIVER: return true;
    ...
  }
}

/*
移動コストと水系地形のフラグは地形に関するデータと考えられますが、上記ではコードに埋め込んでしまっている。さらに悪い事に、
単一の地形タイプのデータがいろいろなメソッド内に出てきてます。これら全部をまとめてカプセル化しておくほうがずっと良いでしょう。

ゲーム世界のすべてのタイルごとにそのインスタンスを持つようなことはしたくない。そのタイルの位置に固有のものは実際にはその中にはない。
フライウェイトパターンの用語で言えば、地形のすべての状態は「内在的」または「状況非依存」。そうだとすれば、各地形タイプのオブジェクトを
２つ以上持つ理由はない。世界を列挙型やTerrainオブジェクトの格子にするのではなく、Terrainオブジェクトへのポインタの格子にする
*/

class Terrain {
public:
  Terrain(int moveCost, bool isWater, Texture texture) :
    moveCost_(moveCost), isWater_(isWater), texture_(texture)
    { }
  
  int getMoveCost() const { return moveCost_; }
  bool isWater() const { return isWater_; }
  const Texture& getTexture() const { return texture; }

private:
  int moveCost_;
  bool isWater_;
  Texture texture_;
};

class World {
private:
  Terrain* tiles_[WIDTH][HEIGHT];
  ...
};

class World {
public:
  World() :
    grassTerrain_(1, false, GRASS_TEXURE),
    hillTerrain_(3, false, HILL_TEXURE),
    riverTerrain_(2, true, RIVER_TEXTURE)
  { }
  
private:
  Terrain grassTerrain_;
  Terrain hillTerrain_;
  Terrain riverTerrain_;
  ...
};

/*
  地表の描画
*/
void World::generateTerrain() {
  for (int x=0; x < WIDTH; x++) {
    for (int y=0; y < HEIGHT; y++) {
      if (random(10) == 0) {
        tiles_[x][y] = &hillTerrain_;
      } else {
        tiles_[x][y] = &grassTerrain_;
      }
    }
  }
  
  int x = random(WIDTH);
  for (int y=0; y < HEIGHT; y++) {
    tiles_[x][y] = &riverTerrain_;
  }
}

const Terrain& World::getTile(int x, int y) {
  return *tiles_[x][y];
}

int cost = world.getTile(2, 3).getMovementCost();
