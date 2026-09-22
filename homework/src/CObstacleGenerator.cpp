#include "CObstacleGenerator.hpp"
#include "CArena.hpp"
#include "Config.hpp"

std::vector<CObstacle> CObstacleGenerator::Generate(int count, const CArena& arena,
                                                      const std::vector<Vector2>& clearancePoints,
                                                      std::mt19937& rng)
{
  constexpr int MaxAttemptsPerObstacle = 200;

  const Rectangle bounds = arena.Bounds();
  std::uniform_real_distribution<float> sizeDist(Config::ObstacleMinSize, Config::ObstacleMaxSize);

  std::vector<CObstacle> obstacles;
  obstacles.reserve(count);

  for (int i = 0; i < count; ++i)
  {
    for (int attempt = 0; attempt < MaxAttemptsPerObstacle; ++attempt)
    {
      const float width = sizeDist(rng);
      const float height = sizeDist(rng);
      std::uniform_real_distribution<float> xDist(bounds.x, bounds.x + bounds.width - width);
      std::uniform_real_distribution<float> yDist(bounds.y, bounds.y + bounds.height - height);
      const CObstacle candidate(Rectangle{xDist(rng), yDist(rng), width, height});

      bool valid = true;
      for (const Vector2& point : clearancePoints)
      {
        if (candidate.DistanceTo(point) < Config::ObstacleClearanceFromSpawn)
        {
          valid = false;
          break;
        }
      }

      if (valid)
      {
        for (const CObstacle& existing : obstacles)
        {
          const float requiredGap = candidate.HalfThickness() + existing.HalfThickness();
          if (candidate.GapTo(existing) < requiredGap)
          {
            valid = false;
            break;
          }
        }
      }

      if (valid)
      {
        obstacles.push_back(candidate);
        break;
      }
    }
  }

  return obstacles;
}
