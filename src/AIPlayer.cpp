#include "AIPlayer.hpp"
#include <limits>
#include <random>
#include <algorithm>
#include <map>
#include <chrono>
#include <set>

AIPlayer::AIPlayer(Owner color, Difficulty difficulty)
    : color(color), difficulty(difficulty) {}



std::vector<std::pair<int,int>> AIPlayer::chooseMove(Board& board) {
    switch (difficulty) {
        case Difficulty::easy:
            return greedyMove(board, Owner::Blue);
        case Difficulty::medium:
            return miniMax(board);   
        case Difficulty::hard:
            return monteTreeSearch(board);
        default:
            return greedyMove(board, Owner::Blue);
    }
}



std::vector<std::pair<int,int>> AIPlayer::greedyMove(Board& board, Owner currentColor) {

    struct ScoredMove {
        int r, c, score;
    };

    std::vector<ScoredMove> candidates;
    std::vector<std::pair<int,int>> fallback;

    // Detect if this color has any cells on board
    bool hasAnyCells = false;
    for (int r = 0; r < board.getRows(); ++r) {
        for (int c = 0; c < board.getCol(); ++c) {
            if (board.getCell(r,c).getOwner() == currentColor) {
                hasAnyCells = true;
                break;
            }
        }
        if (hasAnyCells) break;
    }

    for (int r = 0; r < board.getRows(); ++r) {
        for (int c = 0; c < board.getCol(); ++c) {

            if (board.getCell(r,c).isAlive()) continue;

            fallback.emplace_back(r,c); 

            int adj = (currentColor == Owner::Blue)
                ? board.getNumBlueNeighbors(r,c)
                : board.getNumRedNeighbors(r,c);

            int growth = (currentColor == Owner::Blue)
                ? board.countEmptyNeighborsWithBlueAdjacency(r,c)
                : board.countEmptyNeighborsWithRedAdjacency(r,c);

            int score = adj * 12 + growth * 4;

            if (!hasAnyCells)
                score += 5;  

            candidates.push_back({r,c,score});
        }
    }

   

    std::sort(candidates.begin(), candidates.end(),
        [](const auto& a, const auto& b) {
            return a.score > b.score;
        });

    std::vector<std::pair<int,int>> moves;
    for (int i = 0; i < std::min(5, (int)candidates.size()); ++i)
        moves.emplace_back(candidates[i].r, candidates[i].c);

    return moves;
}



std::vector<std::pair<int,int>> AIPlayer::miniMax(Board& board) {
    std::mt19937 rng(std::random_device{}());
    auto bundles = generateMoveBundles(board);
    int bestScore = static_cast<int>(-1e9);
    std::vector<std::pair<int,int>> best;

    for (auto& bundle : bundles) {
        Board sim = board;

        for (auto& [r,c] : bundle) {
            sim.toggleBlueCell(r,c);
            sim.placeBlueCell();
        }

        auto redMoves = greedyMove(sim, Owner::Red);
        std::shuffle(redMoves.begin(), redMoves.end(), rng);
        redMoves.resize(5);
        for (auto& [r,c] : redMoves) {
            sim.toggleRedCell(r,c);
            sim.placeRedCell();
        }

        sim.stepBoard();
        int score = evaluatePosition(sim);

        if (score > bestScore) {
            bestScore = score;
            best = bundle;
        }
    }

    return best;
}

std::vector<std::vector<std::pair<int,int>>>
AIPlayer::generateMoveBundles(Board& board) {

    std::vector<std::vector<std::pair<int,int>>> bundles;
    std::mt19937 rng(std::random_device{}());

    auto legal = getLegalMoves(board);

    // Prefer moves adjacent to existing Blue cells
    std::vector<std::pair<int,int>> seeds;
    for (auto& m : legal) {
        if (board.getNumBlueNeighbors(m.first, m.second) > 0)
            seeds.push_back(m);
    }

    if (seeds.empty())
        seeds = legal;

    std::shuffle(seeds.begin(), seeds.end(), rng);

    for (auto& seed : seeds) {
        std::vector<std::pair<int,int>> bundle;
        bundle.push_back(seed);

        std::set<std::pair<int,int>> used{seed};

        // Grow a connected cluster
        while (bundle.size() < 5) {
            std::vector<std::pair<int,int>> frontier;

            for (auto& [r,c] : bundle) {
                for (int dr=-1; dr<=1; ++dr)
                    for (int dc=-1; dc<=1; ++dc) {
                        int nr = r+dr, nc = c+dc;
                        if (board.isOnBoard(nr,nc,board.getRows(),board.getCol()) &&
                            !board.getCell(nr,nc).isAlive() &&
                            !used.count({nr,nc})) {
                            frontier.emplace_back(nr,nc);
                        }
                    }
            }

            if (frontier.empty()) break;

            std::shuffle(frontier.begin(), frontier.end(), rng);
            bundle.push_back(frontier.front());
            used.insert(frontier.front());
        }

        if (bundle.size() == 5)
            bundles.push_back(bundle);

        if (bundles.size() >= 10)
            break;
    }

    return bundles;
}



std::vector<std::pair<int,int>> AIPlayer::monteTreeSearch(Board& board) {

    auto bundles = generateMoveBundles(board);

    auto start = std::chrono::high_resolution_clock::now();
    constexpr int TIME_BUDGET_MS = 25;

    double bestScore = -1e9;
    std::vector<std::pair<int,int>> best;

    std::mt19937 rng(std::random_device{}());

    for (auto& bundle : bundles) {
        double total = 0;
        int sims = 0;

        while (true) {
            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count()
                > TIME_BUDGET_MS)
                break;

            Board sim = board;

            for (auto& [r,c] : bundle) {
                sim.toggleBlueCell(r,c);
                sim.placeBlueCell();
            }

            auto redMoves = greedyMove(sim, Owner::Red);
            std::shuffle(redMoves.begin(), redMoves.end(), rng);

            for (int i = 0; i < std::min(5, (int)redMoves.size()); ++i) {
                sim.toggleRedCell(redMoves[i].first, redMoves[i].second);
                sim.placeRedCell();
            }

            // Short rollout
            for (int g = 0; g < 4; ++g)
                sim.stepBoard();

            total += evaluatePosition(sim);
            sims++;
        }

        if (sims > 0 && total / sims > bestScore) {
            bestScore = total / sims;
            best = bundle;
        }
    }

    return best;
}



std::vector<std::pair<int,int>> AIPlayer::getLegalMoves(Board& board) {
    std::vector<std::pair<int,int>> moves;

    for (int r = 0; r < board.getRows(); ++r) {
        for (int c = 0; c < board.getCol(); ++c) {
            if (!board.getCell(r,c).isAlive()) {
                moves.emplace_back(r,c);
            }
        }
    }

    std::shuffle(moves.begin(), moves.end(), std::mt19937{std::random_device{}()});
    if (moves.size() > 20)
        moves.resize(20);

    return moves;
}

int AIPlayer::evaluatePosition(Board& board) {
    int score = 0;

    for (int r = 0; r < board.getRows(); ++r) {
        for (int c = 0; c < board.getCol(); ++c) {
            if (!board.getCell(r,c).isAlive()) continue;

            int n = board.getNumBlueNeighbors(r,c);

            if (board.getCell(r,c).getOwner() == Owner::Blue) {
                if (n == 2 || n == 3) score += 10;
                else score -= 12;

                score += board.countEmptyNeighborsWithBlueAdjacency(r,c) * 2;
            } else {
                if (n == 2 || n == 3) score -= 8;
            }
        }
    }

    score += board.countBlueClusters() * 15;

    return score;
}

int AIPlayer::countEdgeCells(Board& board) {
    int score = 0;
    int rows = board.getRows();
    int cols = board.getCol();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board.getCell(r,c).getOwner() == Owner::Blue &&
               (r == 0 || c == 0 || r == rows-1 || c == cols-1)) {
                score++;
            }
        }
    }
    return score;
}


