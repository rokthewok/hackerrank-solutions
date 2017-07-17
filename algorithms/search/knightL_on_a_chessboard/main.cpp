#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <utility>

/**
 *    HackerRank - KnightL on a Chessboard
 *
 * Solution calls for either DFS or BFS. We will implement BFS here.
 * Could this also employ dynamic programming, since we are looking for
 * a minimal path without backtracking?
 */

struct Position {
  int prev_x;
  int prev_y;
  int x;
  int y;
  int moves;

  Position(int prev_x, int prev_y, int start_x, int start_y, int moves=0)
    : prev_x(prev_x), prev_y(prev_y), x(start_x), y(start_y), moves(moves) { }
};

bool operator<(const Position & p1, const Position & p2) {
  return p1.moves < p2.moves;
}

std::ostream & operator<<(std::ostream & out, const Position & p) {
  out << "{\"x\": " << p.x
      << ", \"y\": " << p.y
      << ", \"moves\": " << p.moves
      << "}";
 return out;
}

bool at_end(const Position & p, int board_size) {
  return p.x == board_size - 1 && p.y == board_size - 1;
}

bool in_bounds(const Position & p, int board_size) {
  return p.x >= 0 && p.x < board_size && p.y >= 0 && p.y < board_size;
}

bool cycling_movement(const Position & start, const Position & dest) {
  return start.prev_x == dest.x && start.prev_y == dest.y;
}

int find_shortest_path(int start_x, int start_y, int a, int b, int board_size) {
  std::vector<std::vector<bool>> visited_board(board_size,
                                               std::vector<bool>(board_size,
                                                                 false));
  std::queue<Position> positions;
  positions.push(Position(start_x, start_y, start_x, start_y, 0));
  std::vector<Position> result_set;
  std::vector<std::pair<int, int>> movements = {
                                                {a, b}, {a, -b},
                                                {-a, -b}, {-a, b},
                                                {b, a}, {b, -a},
                                                {-b, -a}, {-b, a}
                                               };
  while(!positions.empty()) {
    int current_size = positions.size();
    for(int i = 0; i < current_size; ++i) {
      Position start_p = positions.front();
      positions.pop();

      if(at_end(start_p, board_size)) {
        // this path reached the end; add it to the result set
        return start_p.moves;
      } else {
        // we can move in 8 possible directions
        for( auto move : movements) {
          Position p(start_p.x, start_p.y,
                     start_p.x + move.first,
                     start_p.y + move.second,
                     start_p.moves + 1);
          if(cycling_movement(start_p, p)) {
            // we're in a cycle; skip
            continue;
          }
          if(in_bounds(p, board_size) && !visited_board[p.x][p.y]) {
            visited_board[p.x][p.y] = true;
            positions.push(p);
          }
        }
      }
    }
  }

  // we couldn't find a path; return -1
  return -1;
}

int main(int argc, char ** argv) {
  int board_size = 0;
  std::cin >> board_size;

  for(int i = 1; i < board_size; ++i) {
    for(int j = 1; j < board_size; ++j) {
      if( j != 1) {
        std::cout << " ";
      }
      std::cout << find_shortest_path(0, 0, i, j, board_size);
    }
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}
