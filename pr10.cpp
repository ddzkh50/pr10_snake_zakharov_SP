#include <cstdlib>
#include <iostream>
#include <clocale>
#include <conio.h>
#include <fstream>
#include <windows.h>

//■
// 0 - поле
// 1 - змейка
// 2 - границы
// 3 - еда

struct Snake{
    int i;
    int j;
    Snake* next = nullptr;
};
struct Food{
    int i;
    int j;
};

const int none = 0;
const int up = 1;
const int down = 2;
const int left = 3;
const int right = 4;

class SnakeGameList{
    private:
        int lastTailI = 0;
        int lastTailJ = 0;

        Snake* head = nullptr;
        Snake* tail = nullptr;

        int score = 0;
        int spawnTimer = 0;

        static const int max_food = 10;
        int foodCount = 0;

    public:
        void setScore(int value){
            score = value;
        }

        void deleteSnake(){
            Snake* current = head;

            while(current != nullptr){
                Snake* next = current->next;
                delete current;
                current = next;
            }
            head = nullptr;
            tail = nullptr;
        }

        void setSnakePos(int i, int j){
            Snake* temp = new Snake();
            temp->i = i;
            temp->j = j;

            if (head == nullptr){
                head = temp;
                tail = temp;
            }
            else{
                tail->next = temp;
                tail = temp;
            }
        }

        void setFoodCount(int value){
            foodCount = value;
        }

        void setFoodPos(int index, int i, int j){
            if (index >= 0 && index < max_food){
                foods[index].i = i;
                foods[index].j = j;
            }
        }

        int getSnakeLenght(){
            Snake* current = head;
            int lenght = 0;
            while(current != nullptr){
                lenght++;
                current = current->next;
            }
            return lenght;
        }

        Food foods[max_food];

        int getFoodCount() {
            return foodCount;
        }

        int getScore(){
            return score;
        }

        void addScore(){
            score++;
        }

        void addBody() {
            if (tail == nullptr) return;

            Snake* newBody = new Snake();

            newBody->i = lastTailI;
            newBody->j = lastTailJ;

            tail->next = newBody;
            tail = newBody;
        }

        bool isSnakePosition(int i, int j) {
            Snake* current = head;

            while (current != nullptr) {
                if (current->i == i && current->j == j)
                    return true;

                current = current->next;
            }

            return false;
        }

        bool isFoodPosition(int i, int j) {
            for (int k = 0; k < foodCount; k++) {
                if (foods[k].i == i && foods[k].j == j)
                    return true;
            }

            return false;
        }

        void spawnFood() {
            if (foodCount >= max_food)
                return;

            int i, j;

            do {
                i = rand() % 12 + 2;
                j = rand() % 12 + 2;

            }while (isSnakePosition(i, j) ||
                     isFoodPosition(i, j));

            foods[foodCount].i = i;
            foods[foodCount].j = j;

            foodCount++;
        }

        void tick() {
            int value = 500 - score * 10;

            if (value < 100)
                value = 100;

            spawnTimer++;

            if (spawnTimer >= 5) {
                spawnFood();
                spawnTimer = 0;
            }

            Sleep(value);
        }

        void initSnake(int objects) {
            if (objects <= 0) return;

            head = new Snake();
            tail = head;

            for (int i = 1; i < objects; i++) {
                addBody();
            }
        }
        Snake* getHead(){
            return head;
        }

        void snakeMove(Snake* current, int i, int j){
            if (tail != nullptr) {
                    lastTailI = tail->i;
                    lastTailJ = tail->j;
                }

            int tempI, tempJ;

            while (current != nullptr){
                tempI = current->i;
                tempJ = current->j;

                current->i = i;
                current->j = j;

                i = tempI;
                j = tempJ;

                current = current->next;
            }
        }

        void moveUp() {
            if (head == nullptr) return;

            int i = head->i;
            int j = head->j;

            head->i -= 1;

            snakeMove(head->next, i, j);
        }
        void moveLeft(){
            if (head == nullptr) return;

            int i = head->i;
            int j = head->j;

            head->j -= 1;

            snakeMove(head->next, i, j);
        }
        void moveRight(){
            if (head == nullptr) return;

            int i = head->i;
            int j = head->j;

            head->j += 1;

            snakeMove(head->next, i, j);
        }
        void moveDown(){
            if (head == nullptr) return;

            int i = head->i;
            int j = head->j;

            head->i += 1;

            snakeMove(head->next, i, j);
        }

        bool check() {
            if (head == nullptr) return false;

            Snake* current = head;
            Snake* body;

            for (int k = 0; k < foodCount; k++) {
                if (head->i == foods[k].i &&
                    head->j == foods[k].j) {

                    addScore();
                    addBody();

                    foods[k] = foods[foodCount - 1];
                    foodCount--;

                    break;
                }
            }

            while (current != nullptr) {
                if (current->i < 2 || current->i > 13
                    || current->j < 2 || current->j > 13)
                    return false;

                body = current->next;

                while (body != nullptr) {
                    if (current->i == body->i &&
                        current->j == body->j)
                        return false;

                    body = body->next;
                }

                current = current->next;
            }
            return true;
        }
};

void applyMove(SnakeGameList& snake,void (SnakeGameList::*move)()){
    if (move != nullptr){
        (snake.*move)();
    }
}

void limit_playground(int (*arr)[15], int size){
    for (int i = 0; i < size; i++) {
        arr[0][i] = 2;
        arr[size-1][i] = 2;
        arr[i][0] = 2;
        arr[i][size-1] = 2;
    }
}

void drawPlayground(int playground[15][15], SnakeGameList& snake) {
    std::cout << "Your Score: " << snake.getScore() << "\n";
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (playground[i][j] == 0)
                std::cout << "  ";
            else if (playground[i][j] == 1)
                std::cout << "■ ";
            else if (playground[i][j] == 3)
                std::cout << "o ";
            else if (playground[i][j] == 2)
                std::cout << "■ ";
        }
        std::cout << "\n";
    }
}

void updatePlayground(int playground[15][15], SnakeGameList& snake) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            playground[i][j] = 0;
        }
    }

    limit_playground(playground, 15);

    Snake* current = snake.getHead();

    for (int k = 0; k < snake.getFoodCount(); k++) {
        Food food = snake.foods[k];

        playground[food.i][food.j] = 3;
    }

    while (current != nullptr) {
        playground[current->i][current->j] = 1;
        current = current->next;
    }

    system("cls");
    drawPlayground(playground, snake);
}


bool saveGame(SnakeGameList& snake, int direction) {
    std::ofstream file("snake_save.bin", std::ios::binary);

    if (!file.is_open())
        return false;

    int score = snake.getScore();
    int length = snake.getSnakeLenght();
    int foodCount = snake.getFoodCount();
    int dir = direction;
    file.write(reinterpret_cast<char*>(&score), sizeof(score));
    file.write(reinterpret_cast<char*>(&dir), sizeof(dir));
    file.write(reinterpret_cast<char*>(&length), sizeof(length));
    file.write(reinterpret_cast<char*>(&foodCount), sizeof(foodCount));

    Snake* current = snake.getHead();
    while (current != nullptr) {
        file.write(reinterpret_cast<char*>(&current->i), sizeof(int));
        file.write(reinterpret_cast<char*>(&current->j), sizeof(int));

        current = current->next;
    }
    for (int i = 0; i < foodCount; i++) {
        file.write(reinterpret_cast<char*>(&snake.foods[i].i), sizeof(int));
        file.write(reinterpret_cast<char*>(&snake.foods[i].j), sizeof(int));
    }

    file.close();
    return !file;
}

bool loadGame(SnakeGameList& snake, int& direction) {
    std::ifstream file("snake_save.bin", std::ios::binary);

    if (!file.is_open())
        return false;

    int score, dir, length, foodCount;

    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    file.read(reinterpret_cast<char*>(&dir), sizeof(dir));
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    file.read(reinterpret_cast<char*>(&foodCount), sizeof(foodCount));
    if (!file ||score < 0 || dir < 0 || dir > 4 ||
        length < 1 || length > 144 ||
        foodCount < 0 || foodCount > 10) {
        file.close();
        return false;
    }

    int* positions = new int[length * 2];
    Food loadedFood[10];

    for (int i = 0; i < length * 2; i++) {
        file.read(reinterpret_cast<char*>(&positions[i]), sizeof(int));
    }
    for (int i = 0; i < foodCount; i++) {
        file.read(reinterpret_cast<char*>(&loadedFood[i].i), sizeof(int));
        file.read(reinterpret_cast<char*>(&loadedFood[i].j), sizeof(int));
    }
    if (!file) {
        delete[] positions;
        file.close();
        return false;
    }

    file.close();
    snake.deleteSnake();
    snake.setScore(score);
    snake.setFoodCount(0);

    for (int i = 0; i < length; i++) {
        snake.setSnakePos(positions[i * 2], positions[i * 2 + 1]);
    }
    for (int i = 0; i < foodCount; i++) {
        snake.setFoodPos(i, loadedFood[i].i, loadedFood[i].j);
    }

    snake.setFoodCount(foodCount);
    direction = dir;

    delete[] positions;
    return true;
}

void startGame(SnakeGameList& snake, bool loadSaved) {
    int dir = none;

    if (loadSaved) {
        if (!loadGame(snake, dir)) {
            std::cout << "Load error\n";
            _getch();
            return;
        }
    }
    else {
        snake.deleteSnake();
        snake.setScore(0);
        snake.setFoodCount(0);

        snake.initSnake(3);
        snake.spawnFood();

        Snake* current = snake.getHead();

        for (int j = 5; j > 2; j--) {
            if (current != nullptr) {
                current->i = 5;
                current->j = j;
                current = current->next;
            }
        }
    }

    int playground[15][15] = {0};

    limit_playground(playground, 15);
    updatePlayground(playground, snake);

    void (SnakeGameList::*move)() = nullptr;

    if (dir == up)
        move = &SnakeGameList::moveUp;
    else if (dir == down)
        move = &SnakeGameList::moveDown;
    else if (dir == left)
        move = &SnakeGameList::moveLeft;
    else if (dir == right)
        move = &SnakeGameList::moveRight;

    while (snake.check()) {
        if (_kbhit()) {
            switch (_getch()) {
                case 119:
                    if (move != &SnakeGameList::moveDown) {
                        move = &SnakeGameList::moveUp;
                        dir = up;
                    }
                    break;
                case 115:
                    if (move != &SnakeGameList::moveUp){
                        move = &SnakeGameList::moveDown;
                        dir = down;
                    }
                    break;
                case 97:
                    if (move != &SnakeGameList::moveRight){
                        move = &SnakeGameList::moveLeft;
                        dir = left;
                    }
                    break;
                case 100:
                    if (move != &SnakeGameList::moveLeft){
                        move = &SnakeGameList::moveRight;
                        dir = right;
                    }
                    break;
                case 'P':
                    if (saveGame(snake, dir))
                        std::cout << "Game saved\n";
                    else
                        std::cout << "Error of save\n";
                    break;
            }
        }
        applyMove(snake, move);
        updatePlayground(playground, snake);

        snake.tick();
    }

    std::cout << "\nYou Lose\nPress any key to continue";
    _getch();
    return;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    SnakeGameList snake;
    int enter = 0;
    while(enter != 3){
        std::cout << "\n\n1. New game\n"
            << "2. Load game\n"
            << "3. Exit\n";
        std::cin >> enter;

        switch (enter) {
            case 1:{
                startGame(snake, false);
                break;
                }
            case 2:{
                startGame(snake, true);
                break;
                }
        }
        enter = 0;
    }
    snake.deleteSnake();
    return 0;
}
