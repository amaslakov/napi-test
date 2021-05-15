#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <cstdlib>
#include <chrono>

static std::mutex barrier;
std::deque<std::vector<double>> myDeque;

std::vector<double> get_random_bundle()
{
  std::vector<double> bundle;
  for (int i = 0; i < 5; ++i)
  {
    bundle.push_back((double)rand() / RAND_MAX);
  }
  return bundle;
}

void simulation_thread()
{
  barrier.lock();
  std::cout << std::this_thread::get_id() << " started" << std::endl;
  myDeque.push_back(get_random_bundle());
  barrier.unlock();
}

int main()
{
  std::vector<std::thread> threads;
  int i;

  for (i = 0; i < 8; ++i)
  {
    threads.push_back(std::thread(&simulation_thread));
  }
  for (auto &t : threads)
  {
    t.join();
  }

  std::vector<double> temp_vector;
  std::cout.precision(3);
  int size = myDeque.size();
  for (i = 0; i < size; ++i)
  {
    temp_vector = myDeque.front();
    myDeque.pop_front();
    std::cout << "[";
    for (int j = 0; j < temp_vector.size(); ++j)
    {
      std::cout << std::fixed << temp_vector[j] << ", ";
    }
    std::cout << "]" << std::endl;
  }

  return 0;
}