#include <iostream>
#include <unistd.h>

#include <chrono>
#include <thread>

const int sizeX = 10;
const int sizeY = 10;

struct Element{
  int x;
  int y;
};

struct FieldElement{
    bool hasApple;
    bool hasSnake;
    bool hasHead;
};

class Field{
    private:
        Element apple;

    public:
        FieldElement field[sizeX][sizeY];
        void clear(){
            for(int i = 0; i < sizeX; i++){for(int j = 0; j < sizeY; j++){this->field[i][j].hasSnake = false; this->field[i][j].hasApple = false; this->field[i][j].hasHead = false;}}
        }

        void spawnApple(){
            this->clear();

            int x = rand() % sizeX;
            int y = rand() % sizeY;

            this->field[x][y].hasApple = true;

            this->apple.x = x;
            this->apple.y = y;
        }

        Element getApple(){
            return this->apple;
        }

        void showField(){
            for(int i = 0; i < sizeY; i++){
                for(int j = 0; j < sizeX; j++){
                    std::cout << (this->field[j][i].hasSnake ? (this->field[j][i].hasHead ? "•" : "*") : this->field[j][i].hasApple ? "A" : " ") << " ";
                }

                std::cout << std::endl;
            }
        }
};

class Snake{
private:
  int dx, dy, size = 0;
  Element body[sizeX * sizeY];
  Field* field;

public:
  Snake(Field* f){
    this->dx = 1;
    this->dy = 0;

    this->body[0].x = sizeX / 2;
    this->body[0].y = sizeY / 2;

    this->field = f;
  }

  void setDir(int x, int y){
      if(x == 0 && y == 0){this->dx = !this->dx; this->dy = !this->dy; return;}

      this->dx = x;
      this->dy = y;
  }

  bool move(){
    Element* b = this->body;

    Element p = b[0];
    b[0].x += this->dx;
    b[0].y += this->dy;

    b[0].x = b[0].x < 0 ? sizeX : b[0].x > sizeX ? 0 : b[0].x;
    b[0].y = b[0].y < 0 ? sizeY : b[0].y > sizeY ? 0 : b[0].y;

    this->field->field[b[0].x][b[0].y].hasHead = true;
    this->field->field[p.x][p.y].hasHead = false;

    this->field->field[b[0].x][b[0].y].hasSnake = true;

    for(int i = 1; i <= this->size; i++){
      Element prevLoop = b[i];

      b[i].x = p.x;
      b[i].y = p.y;

      this->field->field[p.x][p.y].hasSnake = true;
      p = prevLoop;
    }

    int x = p.x < 0 ? 0 : p.x;
    int y = p.y < 0 ? 0 : p.y;

    this->field->field[x][y].hasSnake = false;

    for(int i = 0; i < this->size; i++){
        for(int j = 0; j < this->size; j++){
            if(i == j){continue;}

            if(b[i].x == b[j].x && b[i].y == b[j].y){return false;}
        }
    }
    return true;
  }

  bool eat(){
    if(this->size >= sizeX * sizeY){return false;}
    this->size++;

    if(!this->move()){return false;}

    this->field->spawnApple();

    return true;
  } // todo

  bool collidesWithApple(){
      Element hPos = this->body[0];

      std::cout << hPos.x << " " << hPos.y << std::endl;

      if(this->field->field[hPos.x ][hPos.y].hasApple && (hPos.x != 0 && hPos.y != sizeY)){return true;}
      return false;
  }

  int getSize(){return this->size;}

  void show(){
    std::cout << "size: " << this->size << std::endl;
  }
};

void snakeEEE(){
  Field field;
  Snake snake(&field);
  field.spawnApple();

  snake.setDir(0, 1);

  int hardness = 1;

  std::cout << "Подробный гайд 2022: вводите w, a, s, d чтоб двигаться (именно в ловеркейсе!) (ввод на ENTER)" << std::endl;
  char dir;

  while(1){
    std::cin >> dir;
      switch(dir){
          case 'w':
              snake.setDir(0, -1);
              break;
          case 's':
              snake.setDir(0, 1);
              break;
          case 'a':
              snake.setDir(-1, 0);
              break;
          case 'd':
              snake.setDir(1, 0);
              break;
          default:
              snake.setDir(0, 0);
              break;
      }

      if(!snake.move()){break;}
      if(snake.collidesWithApple()){
          std::cout << "collide!" << std::endl;
          if(!snake.eat()){break;}
      }
      field.showField();
      std::this_thread::sleep_for(std::chrono::milliseconds(hardness * 100));
  }

  std::cout << "GAME OVER! FINAL SCORE: " << snake.getSize() << std::endl;
}

int main(){
    std::thread t(snakeEEE);

    t.join();
    return 0;
}
