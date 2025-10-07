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

void GameOfLifeStep(std::vector<int> grid, std::vector<std::tuple<int, int>*> AliveCells, int sizeX, int sizeY, int nbIteration) {
    std::vector<std::tuple<int, int>*> gridCoo;
    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            grid.push_back(0);
            gridCoo.push_back(new std::tuple<int, int>(x, y));
        }
    }


    for (int i = 0; i < AliveCells.size(); i++)
    {
        grid[std::get<0>(*AliveCells[i]) * sizeY + std::get<1>(*AliveCells[i])] = 1;
        
    }
     
    for (int i = 0; i < nbIteration; i++)
    {
        for (int z = 0; z < grid.size(); z++)
        {
            int nbVoisin = 0;
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    if ((std::get<0>(*gridCoo[i]) + x) * sizeY + (std::get<1>(*gridCoo[i]) + y) < grid.size())
                    {
                        if (grid[(std::get<0>(*gridCoo[i]) + x) * sizeY + (std::get<1>(*gridCoo[i]) + y)] == 1)
                        {
                            nbVoisin++;
                        }
                    }
                }
            }

            if (grid[z] == 0 && nbVoisin == 3)
            {
                grid[z] = 1;
            }
            else if (grid[z] == 1 && (nbVoisin == 2 || nbVoisin == 3))
            {
                grid[z] == 0;
            }
            else
            {
                grid[z] = 1;
            }
        }

        for (int a = 0; a < sizeX; a++)
        {
            for (int b = 0; b < sizeX; b++)
            {
                std::cout << grid[a * sizeY + b];
            }
                std::cout << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {

    FTimedFunction TimedFunction = { 2.0f, GameOfLifeStep };

    const int MaxIteration = 10;
    int NbrIteration = 0;
    bool IsFloodFillFinished = false;

    auto TimeNow = std::chrono::high_resolution_clock::now();
    while (NbrIteration <= MaxIteration || IsFloodFillFinished) {
        std::chrono::steady_clock::time_point NewTimeNow = std::chrono::steady_clock::now();
        std::chrono::duration<float> ElapsedTime = NewTimeNow - TimeNow;
        TimeNow = NewTimeNow;

        TimedFunction.SubstractTime(ElapsedTime.count());
        if (TimedFunction.IsFinished()) {

            (TimedFunction.GetCallBack())();
            TimedFunction.Restart();

            NbrIteration++;
        }
    }

    return 0;
}