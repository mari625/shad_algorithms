#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

struct Player {
    long power;
    int index;
};

struct Team {
    int index_begin = 0;
    int index_end = 0;
    int64_t sum = 0;
    long max_power = 0;
};

struct Answer {
    int64_t sum = 0;
    std::vector<int> players;
};

bool PlayerComparator(Player first, Player second) {
    return first.power < second.power;
}

Team MoveTeam(Team& team, std::vector<Player>& players, int number_of_players,
              long current_power) {
    while (team.index_begin < number_of_players - 1 &&
           team.index_begin <= team.index_end &&
           players[team.index_begin].power +
                   players[team.index_begin + 1].power <
               current_power) {
        team.sum -= static_cast<int64_t>(players[team.index_begin].power);
        team.index_begin += 1;
    }

    team.max_power = players[team.index_begin].power;
    if (team.index_begin < number_of_players - 1) {
        team.max_power += players[team.index_begin + 1].power;
    }

    return team;
}

void CognitiveRelease(int number_of_players, std::vector<Player>& players,
                      Team& leading_team, Team& last_team, int index) {
    long current_power = players[index].power;
    if (current_power <= leading_team.max_power) {
        leading_team.sum += static_cast<int64_t>(current_power);
        if (index == number_of_players - 1) {
            leading_team.index_end = number_of_players - 1;
        }
    } else {
        leading_team.index_end = index - 1;
        if (last_team.sum < leading_team.sum) {
            last_team.index_begin = leading_team.index_begin;
            last_team.index_end = index;
            last_team.sum = leading_team.sum;
            last_team =
                MoveTeam(last_team, players, number_of_players, current_power);
            last_team.sum += static_cast<int64_t>(current_power);
        } else {
            leading_team = MoveTeam(leading_team, players, number_of_players,
                                    current_power);
            leading_team.sum += static_cast<int64_t>(current_power);
        }
    }
}

Answer Select(int number_of_players, std::vector<Player>* players_ptr) {
    std::vector<Player> players = *players_ptr;
    std::sort(players.begin(), players.end(), PlayerComparator);

    std::vector<int> selected_players;
    Answer answer;

    if (number_of_players <= 2) {
        for (int i = 0; i < number_of_players; ++i) {
            selected_players.push_back(players[i].index + 1);
            answer.sum += static_cast<int64_t>(players[i].power);
        }

        std::sort(selected_players.begin(), selected_players.end());
        answer.players = selected_players;
        return answer;
    }

    Team first_team = Team{0, 2,
                           static_cast<int64_t>(players[0].power) +
                               static_cast<int64_t>(players[1].power),
                           players[0].power + players[1].power};
    Team second_team;

    for (int i = 2; i < std::ssize(players); ++i) {
        if (second_team.index_begin <= first_team.index_begin) {
            CognitiveRelease(number_of_players, players, first_team,  // NOLINT
                             second_team, i);
        } else {
            CognitiveRelease(number_of_players, players, second_team,
                             first_team, i);
        }
    }

    Team best_team;

    if (first_team.sum > second_team.sum) {
        best_team = first_team;
    } else {
        best_team = second_team;
    }

    for (int i = best_team.index_begin; i <= best_team.index_end; ++i) {
        selected_players.push_back(players[i].index + 1);
    }

    std::sort(selected_players.begin(), selected_players.end());
    answer.players = selected_players;
    answer.sum = best_team.sum;

    return answer;
}

int main() {
    int number_of_candidates = 0;
    std::vector<Player> candidates;

    std::cin >> number_of_candidates;

    for (int i = 0; i < number_of_candidates; ++i) {
        int element = 0;
        std::cin >> element;
        candidates.push_back(Player{element, i});
    }

    Answer answer = Select(number_of_candidates, &candidates);

    std::cout << answer.sum << '\n';

    for (int i = 0; i < std::ssize(answer.players); ++i) {
        std::cout << answer.players[i] << " ";
    }
    std::cout << '\n';

    return 0;
}