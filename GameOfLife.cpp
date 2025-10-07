#include <chrono>
#include <functional>
#include <vector>
#include <tuple>
#include <iostream>

struct FTimedFunction {

    FTimedFunction() : Time(-1.f), MaxTime(-1.0f), CallBack(nullptr) {};
    FTimedFunction(float DesiredTime, const std::function<void(std::vector<int> grid, std::vector<std::tuple<int, int>*> AliveCells, int sizeX, int sizeY, int nbIteration)>& DesiredCallBack) : Time(DesiredTime), MaxTime(DesiredTime), CallBack(DesiredCallBack) {}

    void SubstractTime(float SubstractedTime) {
        Time = Time - SubstractedTime;
    }

    bool IsFinished() const {
        return Time <= 0;
    }

    void Restart() {
        Time = MaxTime;
    }

    auto GetCallBack() const {
        return CallBack;
    }
private:
    float Time;
    float MaxTime;
    std::function<void(std::vector<int> grid, std::vector<std::tuple<int, int>*> AliveCells, int sizeX, int sizeY, int nbIteration)> CallBack;
};

void GameOfLifeStep(std::vector<int> grid, std::vector<std::tuple<int, int>*> AliveCells, int sizeX, int sizeY, int nbIteration) 
{
    grid.resize(sizeX * sizeY, 0);

    for (auto cell : AliveCells)
    {
        grid[std::get<0>(*cell) * sizeY + std::get<1>(*cell)] = 1;
    }

    for (int iter = 0; iter < nbIteration; ++iter) 
    {
        std::vector<int> newGrid = grid;

        for (int x = 0; x < sizeX; ++x) 
        {
            for (int y = 0; y < sizeY; ++y) 
            {

                int nbVoisin = 0;
                for (int dx = -1; dx <= 1; ++dx) 
                {
                    for (int dy = -1; dy <= 1; ++dy) 
                    {
                        if (dx == 0 && dy == 0) { continue; }

                        int nx = x + dx, ny = y + dy;

                        if (nx >= 0 && nx < sizeX && ny >= 0 && ny < sizeY)
                        { nbVoisin += grid[nx * sizeY + ny]; }
                    }
                }

                int idx = x * sizeY + y;
                if (grid[idx] == 1 && (nbVoisin < 2 || nbVoisin > 3))
                { newGrid[idx] = 0; }

                else if (grid[idx] == 0 && nbVoisin == 3)
                { newGrid[idx] = 1; }
            }
        }

        grid = newGrid;

        for (int a = 0; a < sizeX; ++a) 
        {
            for (int b = 0; b < sizeY; ++b)
            {
                std::cout << grid[a * sizeY + b];
            }
            std::cout << '\n';
        }
        std::cout << "--------------------------\n";
    }
}


int main(int argc, char* argv[]) {

    FTimedFunction TimedFunction = { 2.0f, GameOfLifeStep };

    const int MaxIteration = 10;
    int NbrIteration = 0;
    bool IsFloodFillFinished = false;

    std::vector<std::tuple<int, int>*> aliveCells;
    auto TimeNow = std::chrono::high_resolution_clock::now();
    while (NbrIteration <= MaxIteration || IsFloodFillFinished) {
        std::chrono::steady_clock::time_point NewTimeNow = std::chrono::steady_clock::now();
        std::chrono::duration<float> ElapsedTime = NewTimeNow - TimeNow;
        TimeNow = NewTimeNow;

        TimedFunction.SubstractTime(ElapsedTime.count());
        if (TimedFunction.IsFinished()) {

            std::vector<int> grid;

            aliveCells.push_back(new std::tuple<int, int>(4, 5));
            aliveCells.push_back(new std::tuple<int, int>(5, 5));
            aliveCells.push_back(new std::tuple<int, int>(6, 5));

            int sizeX = 10;
            int sizeY = 10;
            int nbIteration = 10;

            (TimedFunction.GetCallBack())(grid, aliveCells, sizeX, sizeY, nbIteration);

            TimedFunction.Restart();

            NbrIteration++;
        }
    }

    
    for (auto cell : aliveCells)
        delete cell;

    return 0;
}